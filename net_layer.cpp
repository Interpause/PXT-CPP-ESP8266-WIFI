#include "net_layer.h"		
Request::Request(string m, string URL, string msg, uint64_t timeout, uint64_t resp_timeout, bool discard){
	method = m;
	body = msg;
	lifespan = (timeout==0)?HTTP_REQUEST_TIMEOUT:timeout;
	resp_time = (resp_timeout==0)?HTTP_RESPONSE_TIMEOUT:resp_timeout;
	time_started = 0;
	time_sent = 0;
	sent = false;
	seen = discard;
	req = "";
	slot = -1;
	id = uBit.systemTime();
	smatch urlMatch;
	if(regex_match(URL,urlMatch,URLREGEX)){
		host = urlMatch.str(1);
		port = (urlMatch.str(2).empty())?80:atol(urlMatch.str(2).c_str());
		URI = (urlMatch.str(3).empty())?"/":urlMatch.str(3);
		resp = "";
	}else{
		sent = true;
		resp = NET_URL_INVALID;
	}
}
void Request::send(int connectionSlot){
	//TODO: keep alive requests, forcing user to specify type for POST or PUT
	req = req + method + " " + URI + " HTTP/1.1" + ENDLINE; 
	req = req + "Host: " + host + ENDLINE;
	req = req + "Connection: close" + ENDLINE;
	for(map<string,string>::iterator it = headers.begin(); it != headers.end(); it++) req += ((*it).first+": "+(*it).second+ENDLINE);
	if(method == "POST" || method == "PUT"){
		req = req + "Content-Type: json/application" + ENDLINE;
		req = req + "Content-Length: " + itoa_w(body.length()) + ENDLINE;
		req = req + ENDLINE;
		req += body;
	}
	req = req + ENDLINE;
	slot = connectionSlot;
	time_started = uBit.systemTime();	
}
void Request::update(){
	if(req.empty()) return;
	if(!resp.empty()) return;
	if(sent && uBit.systemTime() - time_sent > resp_time) resp = NET_RESPONSE_TIMED_OUT;
	if(!sent && uBit.systemTime() - time_started > lifespan) resp = NET_REQUEST_TIMED_OUT;
	if(sent) return;
	if(Net_::connectSite(host,port,slot,"TCP")){
		Command_::waitforCmd(string("AT+CIPSEND=")+itoa_w(slot)+","+itoa_w(req.length()+2),vector<string>(1,">"), vector<string>(), 100);
		if(Command_::waitforCmd(req,Command_::nresps,vector<string>(),0) == "SEND OK") sent = true;
	}
}
void Request::setResponse(string msg){
	resp = msg;
}
string Request::getResponse(){
	if(!resp.empty()) seen = true;
	return resp;
}
void Request::setHeaders(map<string,string> head){
	headers = head;
}

vector<Request> request_queue[CONNECTIONSLOTS];
vector<Request> request_cache;
void handleRequestQueue(){
	for(uint8_t i = 0; i < CONNECTIONSLOTS; i++) request_queue[i].clear();
	while(!isInit){
		for(uint8_t i = 0; i < CONNECTIONSLOTS; i++){
			if(!request_queue[i].empty()){
				if(request_queue[i].front().req.empty()) request_queue[i].front().send(i);
				request_queue[i].front().update();
				if(!request_queue[i].front().resp.empty()){
					Net_::disconnectSite(i);
					request_cache.push_back(move(request_queue[i].front()));
					request_queue[i].erase(request_queue[i].begin());
				}
			}
		}
		fiber_sleep(LOOP_PAUSE);
	}
}
void handleRequestCache(){
	request_cache.clear();
	while(!isInit){
		for(vector<Request>::iterator it = request_cache.begin(); it != request_cache.end() && !isInit; it++) if((*it).seen) request_cache.erase(it);
		if(request_cache.size() > RESPCACHEMAX) request_cache.erase(request_cache.begin(),request_cache.begin()+(request_cache.size() - RESPCACHEMAX));
		fiber_sleep(LOOP_PAUSE);
	}
}

void initNetLayer(){
	create_fiber(handleRequestQueue);
	create_fiber(handleRequestCache);
}

uint64_t putRequestQueue(Request req){
	uint8_t smallest = 0;
	uint64_t cur = 999999;
	for(uint8_t i = 0; i < CONNECTIONSLOTS; i++){
		if(request_queue[i].size()<cur){
			cur = request_queue[i].size();
			smallest = i;
		}
	}
	request_queue[smallest].push_back(move(req));
	return request_queue[smallest].back().id;
}
namespace Net_{
	//%
	void sendRequest(StringData* method, StringData* uri, StringData* data = NULL){
		putRequestQueue(Request(string(ManagedString(method).toCharArray()), string(ManagedString(uri).toCharArray()), (data==NULL)?string(""):string(ManagedString(data).toCharArray()), 0, 0, true));
	}
	//%
	StringData* waitforRequest(StringData* method, StringData* uri, StringData* data = NULL){
		return retrieveResponse(sendKeptRequest(method,uri,data));

	}
	//%
	int sendKeptRequest(StringData* method, StringData* uri, StringData* data = NULL){
		return putRequestQueue(Request(string(ManagedString(method).toCharArray()), string(ManagedString(uri).toCharArray()), (data==NULL)?string(""):string(ManagedString(data).toCharArray()), 0, 0, false));
	}
	//%
	StringData* retrieveResponse(int id){
		bool inQueue = false;
		for(uint8_t i = 0; i < CONNECTIONSLOTS; i++) for(vector<Request>::iterator it = request_queue[i].begin(); it != request_queue[i].end() && !inQueue; it++) if((*it).id == id) inQueue = true;
		if(inQueue) while(!isInit){
			if(request_cache.back().id==id) return ManagedString(request_cache.back().getResponse().c_str()).leakData();
			fiber_sleep(LOOP_PAUSE);
		}
		return checkResponse(id);
	}
	//%
	StringData* checkResponse(int id){
		for(vector<Request>::iterator it = request_cache.begin(); it != request_cache.end(); it++) if((*it).id == id) return ManagedString((*it).getResponse().c_str()).leakData();
		return ManagedString("").leakData();
	}

	bool usedSlots[] = {false,false,false,false};
	string connectionNames[] = {"","","",""};
	bool connectSite(string url, uint16_t port, uint8_t slot, string protocol){
		disconnectSite(slot);
		string res = Command_::waitforCmd(string("AT+CIPSTART=")+itoa_w(slot)+",\""+protocol+"\",\""+url+"\","+itoa_w(port),Command_::nresps,vector<string>(),0);
		if(res != "ERROR" && res != CMD_TIMED_OUT){
			connectionNames[slot] = url;
			return true;
		}
		return false;
	}
	void disconnectSite(uint8_t slot){
		if(!usedSlots[slot]) return;
		if(connectionNames[slot].empty()) return;
		Command_::waitforCmd(string("AT+CIPCLOSE=")+itoa_w(slot),Command_::nresps,vector<string>(),0);
		connectionNames[slot] = "";	
	}
	void setSlotState(uint8_t slot, bool isOpen){
		usedSlots[slot] = isOpen;
		if(!isOpen) connectionNames[slot] = "";
	}
	void forwardResponse(string resp, uint8_t slot){
		request_queue[slot].front().setResponse(resp);		
	}
}