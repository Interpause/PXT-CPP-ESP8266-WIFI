#ifndef WIFI_COMMAND_INCLUDED
#define WIFI_COMMAND_INCLUDED

#include "module_config.h"

/** Wrapper for AT commands sent to ESP8266. Used in command_layer.cpp. */
class Command{
    public:
        char* cmd, reply, reject;
        char** whitelist,blacklist;
        uint16_t whitelist_len,blacklist_len;
        uint64_t time_sent,lifespan,id;
        bool seen;
		/**
		 * Creates an AT command wrapper for the ESP8266.
		 * @param msg The command/message to send to the ESP8266.
		 * @param wl List of acceptable replies. Set as NULL to accept all replies.
		 * @param wll Length of wl.
		 * @param bl List of ignored replies. Set as NULL to not ignore replies.
		 * @param bll Length of bl.
		 * @param timeout Time before command is regarded as unsuccessful.
		 * @param discard If true, replies aren't cached for viewing.
		 */
        Command(char* msg, char** wl, uint16_t wll, char** bl, uint16_t bll, uint64_t timeout, bool discard);
        void update(void);
        void send(void);
        void setReply(char* response);
        char* getReply(void); 
};
template class linkedList<Command>;
template class linkedList<char>;

//Objects
MicroBitSerial serial(WIFI_TX,WIFI_RX); /**< Used to prevent interruption to other Serial applications. */
linkedList<Command> cmd_queue(); /**< Queue for AT commands to be sent to ESP8266. */
linkedList<Command> cmd_cache(); /**< Cache for commands that have been replied to. */

/** Handles ESP8266 responses. */
void handleResponse(char* msg);
/** Processes serial output of ESP8266. */
void handleSerial();
/** Handles command queue and cache. */
void handleCommands();
#endif