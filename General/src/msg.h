/* operations about the msg */

/* Func */
/* getip ---- basic network operation */
/* send ---- basic network operation */ 
/* receive ---- basic network operation */

#ifndef MSG_H
#define MSG_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <map>
#define MAXBUFLEN 1024
using namespace std;

enum msg_type {BYZANTINE = 1, ACK = 2};
enum orders {RETREAT = 0, ATTACK = 1};

/* The structure of the Byzantine message */
typedef struct {
	uint32_t type; // Must be equal to 1
	uint32_t size; // size of message in bytes
	uint32_t round; // round number
	uint32_t order; // the order (retreat = 0 and attack = 1)
	uint32_t ids[]; // id's of the senders of the message
} ByzantineMessage;

/* The sturcture of the ACK */
typedef struct {
	uint32_t type; // Must be equal to 2
	uint32_t size; // size of message in bytes
	uint32_t round; // round number
} Ack;

typedef map<int, string>  IPList;

class MSG{
public:

	int sockfd;
	uint16_t myport;
	uint32_t my_ip;
	int myid;
	IPList iplist;
	char s[INET_ADDRSTRLEN];
	MSG(){}
	MSG(char *);
	MSG(int id){myid = id;}
	int getidbyIp(char* s){
		IPList::iterator pos;
		for(pos = iplist.begin(); pos != iplist.end(); ++pos){
			char temp[INET_ADDRSTRLEN];
			uint32_t my_ip;
			struct hostent *host = gethostbyname(pos->second.c_str());
  			memcpy(&my_ip, host->h_addr_list[0], host->h_length);
  			//const char *ipaddr = ;
  			strncpy(temp, inet_ntoa(*(struct in_addr*)&my_ip) ,INET_ADDRSTRLEN);

			if(!strncmp(temp, s, INET_ADDRSTRLEN))
				return pos->first;
		}
		return -1;
	}
	void freeack(Ack* p){
		free(p);
	}

	void freebyz(ByzantineMessage *pbyzmsg){
		free(pbyzmsg);
	}
	void sendMessage(int type, void* p, int dest_id);
	int  recvMessage(void *&pmsg);

};

#endif





















