#include "command_layer.h"
Command::Command(char* msg, char** wl, uint16_t wll, char** bl, uint16_t bll, uint64_t timeout, bool discard){
    cmd = msg;
    reply = NULL;
    reject = NULL;
    whitelist = wl;
    blacklist = bl;
    whitelist_len = (wl==NULL)?0:wll;
    blacklist_len = (bl==NULL)?0:bll;
	time_sent = 0;
    lifespan = (timeout==0)?COMMAND_TIMEOUT:timeout;
	id = uBit.systemTime();
	seen = discard;
}
void Command::update(){
	if (time_sent != 0 && uBit.systemTime() - time_sent > lifespan) reply = "TIMED OUT";
}
void Command::send(){
	for(uint64_t i = 0; cmd[i] != '\0'; i++) serial.sendChar(cmd[i]); //slower but prevents overflow.
	serial.sendString(ESP8266_DELIMITER,2);
	time_sent = uBit.systemTime();
}
void Command::setReply(char* response){
	if(strcmp(response,cmd)) return;
	if(whitelist_len != 0 && strind(response,whitelist,whitelist_len) == -1) reject = response;
	else if(blacklist_len != 0){
		if(strind(response,blacklist,blacklist_len) == -1) reply = response;
	}else reply = response;
}
char* Command::getReply(){
	if(reply != NULL) seen = true;
	return reply;
}

void handleResponse(char* msg){
	
}
void handleSerial(){
	char* 
	while(!isInit){
		char in = serial.read(ASYNC);
		if(in != MICROBIT_NO_DATA){
			
		}
		fiber_sleep(LOOP_PAUSE);
	}
}
void handleCommands(){
	
}
void initCommandLayer(){
	serial.redirect(WIFI_TX,WIFI_RX);
	serial.baud(WIFI_BAUD);
	cmd_queue.empty();
	cmd_cache.empty();
	
}

/**
 * Register an event to be fired when one of the delimiter is matched.
 * @param delimiters the characters to match received characters against.
 */
//% help=serial/on-data-received
//% weight=18 blockId=serial_on_data_received block="serial|on data received %delimiters=serial_delimiter_conv"
void onDataReceived(StringData* delimiters, Action body) {
    
    uBit.messageBus.listen(MICROBIT_ID_SERIAL, MICROBIT_SERIAL_EVT_DELIM_MATCH, onButtonA);
    // lazy initialization of serial buffers
    uBit.serial.read(MicroBitSerialMode::ASYNC);
}

return uBit.serial.readUntil(ManagedString(delimiter)).leakData();