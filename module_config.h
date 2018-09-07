/** 
 * Add typename to "unclear" class types if it starts giving problems.
 * Check out ESP8266's documentation here: 
 * https://www.espressif.com/en/support/download/documents?keys=&field_type_tid%5B%5D=14
 * If module doesn't work, try flashing ESP8266 with newer firmware: 
 * https://bbs.espressif.com/viewtopic.php?f=57&t=433
 * http://wiki.aprbrother.com/en/Firmware_For_ESP8266.html
 * @author John-Henry Lim (Interpause@Github)
 */
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
/** Initializes the serial and listeners used in the command layer. */
void initCommandLayer();
void initNetLayer();

//Functions
namespace Wifi_{
	void initializeESP8266(int tx, int rx, int rate);
	//%
	void initWifi(int rx,int tx, int mode, int baud);
	//%
	bool connectWifi(StringData* ssid, StringData* key);
}
namespace Command_{
	vector<string> nresps = {"OK","no change","SEND OK","ERROR","FAIL","SEND FAIL"};
	/** Sends a command and forgets about it. */
	void sendCmd(string msg, vector<string> whitelist, vector<string> blacklist, uint64_t timeout);
	/** Sends a command and waits for its reply. */
	string waitforCmd(string msg, vector<string> whitelist, vector<string> blacklist, uint64_t timeout);
	/** Sends a command and return a integer id that can be used to get the reply via retrieveCmdRep(id) or checkCmdRep(id) */
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