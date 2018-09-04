/** 
 * Check out ESP8266's documentation here: 
 * https://www.espressif.com/en/support/download/documents?keys=&field_type_tid%5B%5D=14
 * If module doesn't work, try flashing ESP8266 with newer firmware: 
 * https://bbs.espressif.com/viewtopic.php?f=57&t=433
 * http://wiki.aprbrother.com/en/Firmware_For_ESP8266.html
 * @author John-Henry Lim (Interpause@Github)
 */
#ifndef WIFI_MODULE_INCLUDED
#define WIFI_MODULE_INCLUDED

#include "pxt.h"

//Global Config
PinName WIFI_TX = 0;
PinName WIFI_RX = 1;
uint32_t WIFI_BAUD = 9600;
uint64_t COMMAND_TIMEOUT = 5000; /**< Default timeout for AT commands. */

uint64_t HTTP_REQUEST_TIMEOUT = 30000; /**< Default timeout for HTTP request commands. */
uint64_t HTTP_RESPONSE_TIMEOUT = 90000; /**< Default timeout for HTTP responses. */
#define LOOP_PAUSE 20
#define ESP8266_DELIMITER '\u000D\u000A'


//Helpers
bool strcmp(char* a, char* b); /**< Compares two char[]. */
uint16_t strind(char* find, char** list, uint16_t len); /**< Gets index of char[] in char[][]. */
char* substr(char* str,uint64_t start,uint64_t len); /**< Heavy duty substring. Doesn't support negative index. */

//Handlers
bool isInit = false; /**< Flag for everything to cancel and release. */
/** Initializes the serial and listeners used in the command layer. */
void initCommandLayer();

/** Partial one-way linkedList template for specialized use. Stuff like insert() not implemented as not needed. Heavy reliance on GC.*/
template <class T>
class linkedList{
	private:
		struct node{
			T* val;
			node* next;
		}
		node* head;
		node* tail;
		uint64_t length;
	public:
		linkedList(void);
		~linkedList(void);
		void push(T*);
		T* pop(void);
		T* splice(node*);
		void empty(void);
		node* first(void);
		uint64_t length(void);
}
#endif