#ifndef WIFI_COMMAND_INCLUDED
#define WIFI_COMMAND_INCLUDED

#include "module_config.h"

/** Wrapper for AT commands sent to ESP8266. Used in command_layer.cpp. */
class Command{
    public:
        string cmd, reply, reject;
        vector<string> whitelist;
		vector<string> blacklist;
        uint64_t time_sent,lifespan,id;
        bool seen;
		/**
		 * Creates an AT command wrapper for the ESP8266.
		 * @param msg The command/message to send to the ESP8266.
		 * @param wl Pointer to list of acceptable replies. Set as NULL to accept all replies. Pointer used for memory efficiency in case of defaults.
		 * @param bl Pointer to list of ignored replies. Set as NULL to not ignore replies. Pointer used for memory efficiency in case of defaults.
		 * @param timeout Time before command is regarded as unsuccessful.
		 * @param discard If true, replies aren't cached for viewing.
		 */
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

/** Handles ESP8266 responses. */
void handleResponse();
/** Processes serial output of ESP8266. */
void handleSerial();
/** Handles command queue. */
void handleCommandQueue();
/** Handles command cache. */
void handleCommandCache();

namespace Command_{
	//%
	void sendCommand(StringData *msg, StringData *whitelist, StringData* blacklist, int timeout);
	//%
	StringData* waitforCommand(StringData *msg, StringData *whitelist, StringData* blacklist, int timeout);
	//%
	int sendKeptCommand(StringData *msg, StringData *whitelist, StringData* blacklist, int timeout);
	//%
	StringData* retrieveCommandReply(int id);
	//%
	StringData* checkCommandReply(int id);
}
#endif