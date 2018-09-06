#include "module_config.h"
#ifndef WIFI_NET_INCLUDED
#define WIFI_NET_INCLUDED
class Request{
	public:
		string method, URI, host, body, req, response;
		map<string,string> headers;
		uint64_t time_sent,lifespan;
		uint16_t port;
		uint8_t slot,id;
		bool sent,seen;
		
		Request(string m, string uri, string msg, uint64_t timeout, bool discard);
		void send(int connectionSlot);
		void update(void);
		void setResponse(string msg);
		string getResponse(void);
}
void handleRequestQueue();
void handleRequestCache();

//Final values
const regex URLREGEX("^(?:https?\:\/\/)?([A-Za-z0-9\-\.]+)(?:\:([0-9]+))?(.*)$");

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
#endif