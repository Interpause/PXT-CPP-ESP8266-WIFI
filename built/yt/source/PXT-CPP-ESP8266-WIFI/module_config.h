//
//
//
//
//
//
//
//
//
#ifndef WIFI_MODULE_INCLUDED
#define WIFI_MODULE_INCLUDED

#include "pxt.h" //Note: has <stdio>, <string>, <vector> and <stdint>
#include <utility>
#include <regex>
#include <map>

using namespace std;

//Global Config
extern MicroBitPin WIFI_TX;
extern MicroBitPin WIFI_RX;
extern uint32_t WIFI_BAUD;
extern uint8_t WIFI_MODE;
extern uint64_t COMMAND_TIMEOUT; /**< Default timeout for AT commands. */

extern uint64_t HTTP_REQUEST_TIMEOUT; /**< Default timeout for HTTP request commands. */
extern uint64_t HTTP_RESPONSE_TIMEOUT; /**< Default timeout for HTTP responses. */

#define LOOP_PAUSE 20
#define CMD_TIMED_OUT "CMD TIMED OUT"
#define NET_REQUEST_TIMED_OUT "REQUEST TIMED OUT"
#define NET_RESPONSE_TIMED_OUT "RESPONSE TIMED OUT"
#define NET_URL_INVALID "INVALID URL"

//Helpers
template<typename T> 
typename vector<T>::iterator vfind(typename vector<T>::iterator start,typename vector<T>::iterator end,T tgt);
vector<string> regex_search_g(StringData* tgt,regex patt);
string itoa_w(int64_t val);

//Handlers
extern bool isInit; /**< Flag for everything to cancel and release. */
//
void initCommandLayer();
void initNetLayer();

//Functions
namespace Wifi_{
	void initializeESP8266(int tx, int rx, int rate);
//
	void initWifi(int rx,int tx, int mode, int baud);
//
	bool connectWifi(StringData* ssid, StringData* key);
}
namespace Command_{
	vector<string> nresps = {"OK","no change","SEND OK","ERROR","FAIL","SEND FAIL"};
//
	void sendCmd(string msg, vector<string> whitelist, vector<string> blacklist, uint64_t timeout);
//
	string waitforCmd(string msg, vector<string> whitelist, vector<string> blacklist, uint64_t timeout);
//
	uint64_t sendKeptCmd(string msg, vector<string> whitelist, vector<string> blacklist, uint64_t timeout);
	string retrieveCmdRep(uint64_t id);
	string checkCmdRep(uint64_t id);
}

#define CONNECTIONSLOTS 4
namespace Net_{
	bool connectSite(string url, uint16_t port, uint8_t slot, string protocol);
	void disconnectSite(uint8_t slot);
	void setSlotState(uint8_t slot, bool isOpen);
	void forwardResponse(string resp, uint8_t slot);
}
#endif
