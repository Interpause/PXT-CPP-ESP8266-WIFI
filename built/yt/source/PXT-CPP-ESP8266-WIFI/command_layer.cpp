#include "command_layer.h"
//Objects
vector<Command> cmd_queue; /**< Queue for AT commands to be sent to ESP8266. */
vector<Command> cmd_cache; /**< Cache for commands that have been replied to. */
string msg = ""; //Holds raw message during processing of uBit.serial input.
string spaces = ""; //Used in trimming message of trailing whitespace.
string httpResponse = ""; //Holds http response sent to response layer.
bool isStart = false; //Used in trimming message of trailing whitespace.
uint64_t responseCharsLeft = 0; //Used for forwarding characters to request layer.
int8_t responseSlot = -1; //which slot is being responded to.

Command::Command(string msg, vector<string> wl, vector<string> bl, uint64_t timeout, bool discard){
    cmd = msg;
    reply = "";
    reject = "";
    whitelist = wl;
    blacklist = bl;
	time_sent = 0;
    lifespan = (timeout==0)?COMMAND_TIMEOUT:timeout;
	id = uBit.systemTime();
	seen = discard;
}
void Command::update(){
	if (time_sent != 0 && uBit.systemTime() - time_sent > lifespan) reply = CMD_TIMED_OUT;
}
void Command::send(){
	for(string::iterator it = cmd.begin(); it != cmd.end(); it++) uBit.serial.sendChar(*it); //slower but prevents overflow.
	uBit.serial.send(ManagedString(ESP8266_DELIMITER));
	time_sent = uBit.systemTime();
}
void Command::setReply(string response){
	if(response == cmd) return;
	if(!whitelist.empty() && vfind(whitelist.begin(),whitelist.end(),response)==whitelist.end()) reject = response;
	else if(!blacklist.empty()){
		if(vfind(blacklist.begin(),blacklist.end(),response)==blacklist.end()) reply = response;
	}else reply = response;
}
string Command::getReply(){
	if(!reply.empty()) seen = true;
	return string(reply);
}

void handleResponse(){
	smatch closeMatch;
	if(regex_search(msg,closeMatch,ATCLOSE)){
		Net_::setSlotState(atoi(closeMatch.str(1).c_str()),false);
		return;
	}
	smatch connectMatch;
	if(regex_search(msg,connectMatch,ATCONNECT)){
		Net_::setSlotState(atoi(connectMatch.str(1).c_str()),true);
		return;
	}
	smatch httpMatch;
	if(regex_search(msg,httpMatch,ATIPD)){
		responseSlot = atoi(httpMatch.str(1).c_str());
		responseCharsLeft = atoi(httpMatch.str(2).c_str());
		httpResponse = httpMatch.str(3);
		responseCharsLeft -= (httpResponse.length() + 2); //Accounts for cut-off CRLF
		return;
	}
	cmd_queue.front().setReply(string(msg));
}

void handleSerial(){
	msg.clear();
	spaces.clear();
	httpResponse.clear();
	responseCharsLeft = 0;
	responseSlot = -1;
	isStart = false;
	while(!isInit){
		auto sin = uBit.serial.read(ASYNC); //Must be redeclared to prevent pointer shenanigans
		if(sin != MICROBIT_NO_DATA){
			char in = (char)sin;
			if(responseSlot != -1){
				httpResponse += in;
				responseCharsLeft--;
			}else if(isStart){
				if(NEXTLINES.find(in) != string::npos){
					if(!msg.empty()) handleResponse();
					msg.clear();
					spaces.clear();
					isStart = false;
				}else{
					spaces += in;
					if(WHITESPACES.find(in) != string::npos){
						msg += spaces;
						spaces.clear();
					}
				}				
			}else if(WHITESPACES.find(in) == string::npos && NEXTLINES.find(in) == string::npos){
				isStart = true;
				msg += in;
			}
		}else if(responseSlot != -1 && responseCharsLeft <= 0){
			Net_::forwardResponse(string(httpResponse),responseSlot);
			httpResponse.clear();
			responseCharsLeft = 0;
			responseSlot = -1;
		}
		fiber_sleep(LOOP_PAUSE);
	}
}

void handleCommandQueue(){
	cmd_queue.clear();
	while(!isInit){
		if(!cmd_queue.empty()){
			cmd_queue.front().update();
			if(cmd_queue.front().time_sent == 0) cmd_queue.front().send();
			if(!cmd_queue.front().reply.empty()){
				cmd_cache.push_back(move(cmd_queue.front()));
				cmd_queue.erase(cmd_queue.begin());
			}
		}
		fiber_sleep(LOOP_PAUSE);
	}
}

void handleCommandCache(){ //async right? if not this is problematic. Vector isnt threadsafe.
	cmd_cache.clear();
	while(!isInit){
		for(vector<Command>::iterator it = cmd_cache.begin(); it != cmd_cache.end() && !isInit; it++) if((*it).seen) cmd_cache.erase(it);
		if(cmd_cache.size() > CMDCACHEMAX) cmd_cache.erase(cmd_cache.begin(),cmd_cache.begin()+(cmd_cache.size() - CMDCACHEMAX));
		fiber_sleep(LOOP_PAUSE);
	}
}

void initCommandLayer(){
	uBit.serial.redirect(WIFI_TX.name,WIFI_RX.name);
	uBit.serial.baud(WIFI_BAUD);
	create_fiber(handleCommandQueue);
	create_fiber(handleCommandCache);
	create_fiber(handleSerial);
}

namespace Command_{
//
	void sendCmd(string msg, vector<string> whitelist, vector<string> blacklist, uint64_t timeout){
		cmd_queue.emplace_back(msg,whitelist,blacklist,timeout,true);
	}
//
	string waitforCmd(string msg, vector<string> whitelist, vector<string> blacklist, uint64_t timeout){
		return retrieveCmdRep(sendKeptCmd(msg,whitelist,blacklist,timeout));
	}
//
	uint64_t sendKeptCmd(string msg, vector<string> whitelist, vector<string> blacklist, uint64_t timeout){
		cmd_queue.emplace_back(msg,whitelist,blacklist,timeout,false);
		return cmd_queue.back().id;
	}
	string retrieveCmdRep(uint64_t id){
		bool inQueue = false;
		for(vector<Command>::iterator it = cmd_queue.begin(); it != cmd_queue.end() && !inQueue; it++) if((*it).id == id) inQueue = true;
		if(inQueue) while(!isInit){
			if(cmd_cache.back().id==id) return cmd_cache.back().getReply();
			fiber_sleep(LOOP_PAUSE);
		}
		return checkCmdRep(id);
	}
	string checkCmdRep(uint64_t id){
		for(vector<Command>::iterator it = cmd_cache.begin(); it != cmd_cache.end(); it++) if((*it).id == id) return (*it).getReply();
		return string("");
	}

//
	void sendCommand(StringData *msg, StringData *whitelist, StringData* blacklist, int timeout){
		sendCmd(string(ManagedString(msg).toCharArray()),regex_search_g(whitelist,DESTRINGIFY),regex_search_g(blacklist,DESTRINGIFY),timeout);
	}
//
	StringData* waitforCommand(StringData *msg, StringData *whitelist, StringData* blacklist, int timeout){
		return retrieveCommandReply(sendKeptCommand(msg,whitelist,blacklist,timeout));
	}
//
	int sendKeptCommand(StringData *msg, StringData *whitelist, StringData* blacklist, int timeout){
		return sendKeptCmd(string(ManagedString(msg).toCharArray()),regex_search_g(whitelist,DESTRINGIFY),regex_search_g(blacklist,DESTRINGIFY),timeout);
	}
//
	StringData* retrieveCommandReply(int id){
		return ManagedString(retrieveCmdRep(id).c_str()).leakData();
	}
//
	StringData* checkCommandReply(int id){
		return ManagedString(checkCmdRep(id).c_str()).leakData();
	}
}
