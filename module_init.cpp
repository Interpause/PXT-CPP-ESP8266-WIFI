#include "module_config.h"
#include <map>

//Extern initializers
MicroBitPin* WIFI_TX = &uBit.io.P0;
MicroBitPin* WIFI_RX = &uBit.io.P1;
uint32_t WIFI_BAUD = 9600;
uint8_t WIFI_MODE = 1;
uint64_t COMMAND_TIMEOUT = 4000;
uint64_t HTTP_REQUEST_TIMEOUT = 60000;
uint64_t HTTP_RESPONSE_TIMEOUT = 90000;
bool isInit = false;

//Initialize helpers
template<typename T> 
vector<T>::iterator find(vector<T>::iterator start,vector<T>::iterator end,T tgt){
	for(vector<T>::iterator it = start; it != end; it++) if(*it == tgt) return it;
	return end;
}

vector<string> regex_search_g(DataString* tgt,regex patt){
	smatch store;
	vector<string> res();
	string raw(ManagedString(tgt).toCharArray());
	while(regex_search(raw,store,patt)){
		for(smatch::iterator it = store.begin()+1; it != store.end(); it++) res.push_back(move(*it));
		raw = store.suffix().str();
	}
	return vector<string>(res);
}

void Wifi_::initializeESP8266(int tx, int rx, BaudRate rate){
    isInit = true;
	fiber_sleep(LOOP_PAUSE*10);
	isInit = false;
	//The rest of init procedures.
	WIFI_TX = getPin(tx);
	WIFI_RX = getPin(rx);
	WIFI_BAUD = rate;
	initCommandLayer();
	initNetLayer();
}

//%
void Wifi_::initWifi(int rx,int tx, int mode, BaudRate baud=BaudRate.BaudRate9600){
	initializeESP8266(tx,rx,baud);
	if (Command_::waitforCmd(string('AT'), &Command_::nresps, NULL, 10000) == CMD_TIMED_OUT) {
        uBit.display.scroll("WIFI MISSING");
        return;
    }
	WIFI_MODE = mode;
	Command_::sendCmd(string("AT+RST"),new vector<string>(1,"ready"),NULL,120000);
	Command_::sendCmd(string("ATE0"),&Command_::nresps);
	Command_::sendCmd(string("AT+CIPMUX=1"),&Command_::nresps);
	Command_::waitforCmd(string("AT+CWMODE=")+to_string(WIFI_MODE),&Command_::nresps);
}
//%
bool Wifi_::connectWifi(StringData* ssid, StringData* key){
	string res = Command_::waitforCmd(string("AT+CWJAP=\"")+ManagedString(ssid).toCharArray()+"\",\""+ManagedString(key).toCharArray()+"\"",&Command_::nresps);
	return res != "ERROR" && res != "timed out" && res != "FAIL";
}