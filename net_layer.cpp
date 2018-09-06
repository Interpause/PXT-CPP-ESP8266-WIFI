#include "net_layer.h"	
class Request{
	public:
		string method, URI, host, body, req, response;
		map<string,string> headers;
		uint64_t time_sent,lifespan;
		uint16_t port;
		uint8_t slot,id;
		bool sent,seen;
		
Request::Request(string m, string URL, string msg, uint64_t timeout, bool discard){
	method = m;
	body = msg;
	lifespan = timeout;
	sent = false;
	seen = discard;
	req = "";
	smatch urlMatch();
	if(regex_match(URL,urlMatch,URLREGEX)){
		host = urlMatch.str(1);
		port = stoul(urlMatch.str(2));
		URI = urlMatch.str(3);
	}else{
		sent = true;
		response = "INVALID URL";
	}
}
		void send(int connectionSlot);
		void update(void);
		void setResponse(string msg);
		string getResponse(void);
}
void handleRequestQueue();
void handleRequestCache();

void initNetLayer();

namespace Net_{
	//%
	void sendRequest(StringData* method, StringData* uri, StringData* data = "", int timeout = 0);
	//%
	StringData* waitforRequest(StringData* method, StringData* uri, StringData* data = "", int timeout = 0);
	//%
	int sendKeptRequest(StringData* method, StringData* uri, StringData* data = "", int timeout = 0);
	//%
	StringData* retrieveResponse(int id);
	//%
	StringData* checkResponse(int id);
}
namespace Net_{
	bool usedSlots[];
	bool connectSite(string url, uint8_t slot, string protocol = "TCP");
	bool disconnectSite(uint8_t slot);
	void setSlotState(uint8_t slot, bool isOpen);
	void forwardResponse(string resp, uint8_t slot);
}