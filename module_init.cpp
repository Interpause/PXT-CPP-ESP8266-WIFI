#include "module_config.h"
#include <map>

//Extern initializers
MicroBitPin WIFI_TX = uBit.io.P0;
MicroBitPin WIFI_RX = uBit.io.P1;
uint32_t WIFI_BAUD = 9600;
uint8_t WIFI_MODE = 1;
uint64_t COMMAND_TIMEOUT = 4000;
uint64_t HTTP_REQUEST_TIMEOUT = 60000;
uint64_t HTTP_RESPONSE_TIMEOUT = 90000;
bool isInit = false;

//Initialize helpers
template<class T> 
typename vector<T>::iterator vfind(typename vector<T>::iterator start,typename vector<T>::iterator end,T tgt){
	for(typename vector<T>::iterator it = start; it != end; it++) if(*it == tgt) return it;
	return end;
}
template vector<string>::iterator vfind(typename vector<string>::iterator start,typename vector<string>::iterator end,string tgt);

vector<string> regex_search_g(StringData* tgt,regex patt){
	smatch store;
	vector<string> res;
	string raw(ManagedString(tgt).toCharArray());
	while(regex_search(raw,store,patt)){
		for(smatch::iterator it = store.begin()+1; it != store.end(); it++) res.push_back(move(*it));
		raw = store.suffix().str();
	}
	return res;//HFTB
}
string itoa_w(int64_t val){
	char buf[20];
	itoa(val,buf);
	return string(buf);
}

namespace Wifi_{
	void initializeESP8266(int tx, int rx, int rate){
		isInit = true;
		fiber_sleep(LOOP_PAUSE*10);
		isInit = false;
		//The rest of init procedures.
		WIFI_TX = *getPin(tx);
		WIFI_RX = *getPin(rx);
		WIFI_BAUD = rate;
		initCommandLayer();
		initNetLayer();
	}

	//%
	void initWifi(int rx,int tx, int mode, int baud = 9600){
		initializeESP8266(tx,rx,baud);
		if (Command_::waitforCmd(string("AT"), Command_::nresps, vector<string>(), 10000) == CMD_TIMED_OUT) {
			uBit.display.scroll("WIFI MISSING");
			return;
		}
		WIFI_MODE = mode;
		Command_::sendCmd(string("AT+RST"),vector<string>(1,"ready"),vector<string>(),120000);
		Command_::sendCmd(string("ATE0"),Command_::nresps,vector<string>(),0);
		Command_::sendCmd(string("AT+CIPMUX=1"),Command_::nresps,vector<string>(),0);
		Command_::waitforCmd(string("AT+CWMODE=")+itoa_w(WIFI_MODE),Command_::nresps,vector<string>(),0);
	}
	//%
	bool connectWifi(StringData* ssid, StringData* key){
		string res = Command_::waitforCmd(string("AT+CWJAP=\"")+ManagedString(ssid).toCharArray()+"\",\""+ManagedString(key).toCharArray()+"\"",Command_::nresps,vector<string>(),0);
		return res != "ERROR" && res != "timed out" && res != "FAIL";
	}
}