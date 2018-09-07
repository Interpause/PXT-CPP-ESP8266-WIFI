#include "module_config.h"
#ifndef WIFI_NET_INCLUDED
#define WIFI_NET_INCLUDED
class Request{
	public:
		string method, URI, host, body, req, resp;
		map<string,string> headers;
		uint64_t time_started,time_sent,lifespan,resp_time;
		uint16_t port;
		uint8_t slot,id;
		bool sent,seen;
		
		Request(string m, string URL, string msg, uint64_t timeout, uint64_t resp_timeout, bool discard);
		void send(int connectionSlot);
		void update(void);
		void setResponse(string msg);
		string getResponse(void);
		void setHeaders(map<string,string> head);
};
void handleRequestQueue();
void handleRequestCache();

//Final values
#define ENDLINE "\u000D\u000A"
#define RESPCACHEMAX 8
const regex URLREGEX("^(?:https?\\:\\/\\/)?([A-Za-z0-9\\-\\.]+)(?:\\:([0-9]+))?(.*)$");//host,port,uri

namespace Net_{
	//%
	void sendRequest(StringData* method, StringData* uri, StringData* data);
	//%
	StringData* waitforRequest(StringData* method, StringData* uri, StringData* data);
	//%
	int sendKeptRequest(StringData* method, StringData* uri, StringData* data);
	//%
	StringData* retrieveResponse(int id);
	//%
	StringData* checkResponse(int id);
}
#endif