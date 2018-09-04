#include "module_config.h"
//Define helper functions
bool strcmp(char* a, char* b){
	uint64_t i;
	for(i = 0; a[i] != '\0'; i++) if(a[i] != b[i]) return false;
	if(b[i] != '\0') return false;
	return true;
}
int strind(char* find, char** list, uint64_t len){
	for(uint64_t i = 0; i < len; i++) if(strcmp(list[i],find)) return i;
	return -1;
}
int arrind(int find, int* list, uint64_t len){
	for(uint64_t i = 0; i < len; i++) if(find == list[i]) return i;
	return -1;
}
char* substr(char* str, uint64_t start, uint64_t len){
	char sub[len+1];
	uint64_t cur = 0;
	for(uint64_t i = 0; str[i] != '\0' && cur < len; i++) if(i >= start){
		sub[cur] = str[i];
		cur++;
	}
	sub[len] = '\0';
	return sub;
}
int parseInt(char* str){
	int len;
	for(len = 0; arrind(str[len],NUMBERS,10) > -1; len++);
	int res = 0;
	for(int i = 1; i <= len; i++){
		int mul = 1;
		for(int n = 0; n < len - i; n++) mul = mul*10;
		res += arrind(str[len],NUMBERS,10) * mul;
	}
	return res;
}

//Define linkedList template
template <class T>
linkedList<T>::linkedList(){
	head = NULL;
	tail = NULL;
	length = 0;
}
template <class T>
linkedList<T>::~linkedList(){
	empty();
}
template <class T>
void linkedList<T>::push(T* in){
	node* entry = new node;
	entry->val = in;
	entry->next = NULL;
	if(length == 0) head = entry;
	else tail->next = entry;
	tail = entry;
	length++;
}
template <class T>
T* linkedList<T>::pop(){
	if(length == 0) return NULL;
	node* old = head;
	head = old->next;
	length--;
	return old->val;
}
template <class T>
T* linkedList<T>::splice(node* bef){
	if(length <= 1) return NULL;
	node* old = bef->next;
	if(old->next == NULL) tail = bef;
	else bef->next = old->next;
	length--;
	return old->val;
}
template <class T>
void linkedList<T>::empty(){
	while(length > 0) pop();
}
template <class T>
node* linkedList<T>::first(){
	return head;
}
template <class T>
uint64_t linkedList<T>::length(){
	return length;
}

void initializeESP8266(SerialPin rx, SerialPin tx, BaudRate rate){
    isInit = true;
	//pause a while
	isInit = false;
	//The rest of init proccedures.
}