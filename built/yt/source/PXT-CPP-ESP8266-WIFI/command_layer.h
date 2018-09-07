#ifndef WIFI_COMMAND_INCLUDED
#define WIFI_COMMAND_INCLUDED

#include "module_config.h"

//
class Command{
    public:
        string cmd, reply, reject;
        vector<string> whitelist;
		vector<string> blacklist;
        uint64_t time_sent,lifespan,id;
        bool seen;
//
//
//
//
//
//
//
//
        Command(string msg, vector<string> wl, vector<string> bl, uint64_t timeout, bool discard);
        void update(void);
        void send(void);
        void setReply(string response);
        string getReply(void); 
};

//Final values
#define ESP8266_DELIMITER "\u000D\u000A"
#define CMDCACHEMAX 16
const string WHITESPACES = "\u0009\u000B\u0020\u200E\u200F\u2028\u2029";
const string NEXTLINES = "\u000A\u000C\u000D\u0085";
const string CIPCLOSE_REP = ",CLOSED";
const string IPD_REP = "+IPD";
const regex ATCLOSE("(\\d),CLOSED"); //connectionSlot
const regex ATCONNECT("(\\d),CONNECT"); //connectionSlot
const regex ATIPD("\\+IPD,(\\d),(\\d+):(.+)"); //connectionSlot,responseLen,Header
const regex DESTRINGIFY("\"(.+?)\""); //[],["hi"],["hi","bye"],etc

//
void handleResponse();
//
void handleSerial();
//
void handleCommandQueue();
//
void handleCommandCache();

namespace Command_{
//
	void sendCommand(StringData *msg, StringData *whitelist, StringData* blacklist, int timeout);
//
	StringData* waitforCommand(StringData *msg, StringData *whitelist, StringData* blacklist, int timeout);
//
	int sendKeptCommand(StringData *msg, StringData *whitelist, StringData* blacklist, int timeout);
//
	StringData* retrieveCommandReply(int id);
//
	StringData* checkCommandReply(int id);
}
#endif
