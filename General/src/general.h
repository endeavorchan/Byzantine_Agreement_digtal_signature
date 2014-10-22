/* Two kind of generals which are Commander and Lieutenant */
/* Class Commander */
/* Func */

/* Init -----  get the list of all ips and corresponding process ids */
/* ipToid ----  look up the list and gets the corresponding process id */
/* sendVtoAll ---- send v to the proper processes */

/* Class Lieutenant */

/* Init -----  get the list of all ips and corresponding process ids */
/* ipToid ----  look up the list and gets the corresponding process id */
/* sendVtoAll ---- send v to the proper processes */
/* getip ---- basic network operation */
/* send ---- basic network operation */ 
/* receive ---- basic network operation */
#ifndef GENERAL_H
#define GENERAL_H
#include <map>
#include <iostream>
#include <fstream>
#include "setv.h"
#include "msg.h"
#include <stdio.h>      
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
using namespace std;
class Commander{
public:
	SetV set;
	MSG msg;
	ByzantineMessage* byzmsg;
	Ack* ack;
	uint32_t round;
	uint32_t f;
	int myid;
	uint32_t myorder;
	char hname[128];
	char ipstr[INET_ADDRSTRLEN];
	char *hostfile;
	int cmdid;
	char *cmdport;

	ByzantineMessage* allocatebyz(int idcount){
		ByzantineMessage* p = (ByzantineMessage *)malloc(sizeof(ByzantineMessage) + sizeof(uint32_t) * idcount);
		return p;
	}
	Ack* allocateAck(){
		Ack* p = (Ack*)malloc(sizeof(Ack));
		return p;
	}
	Commander(){}
	Commander(char* msgport, char* hostfile, int f, int cmdid, uint32_t order):msg(msgport){
		//need initialze the id of set and msg
		//cm.msg.my_port = msgport;
		//this->msg.my_port = msgport;
		this->hostfile = hostfile;
		this->f = f;
		this->myorder = order;
		string file_name = this->hostfile;
		ifstream infile(file_name.c_str(),ios::in);
		string textline;
		int index = 0;
		while(getline(infile,textline,'\n')){    
			msg.iplist[index++] = textline;
		}  
		//cout << "there are " << index << " nodes" << endl;
		infile.close();
		IPList::iterator pos;
		for(pos = msg.iplist.begin(); pos != msg.iplist.end(); ++pos){
			cout << pos->first << " " << pos->second << endl;
		}
    	setmyIp();
		myid = msg.getidbyIp(ipstr);
		if(myid != cmdid){
			cout << "cmder id mismatch " << endl;
		}
		set.allPcounts = index;
		set.myid = myid;
		msg.myid = myid;
		cout << "my id is " << msg.myid << endl;
		round = 0;
		cout << "order is " << myorder << endl;
		initCommand();
		set.printV();
	}

	void setmyIp(){
   		uint32_t my_ip;
   		char name[256];
  		gethostname(name, 255);
  		struct hostent *host = gethostbyname(name);
  		cout << "my host name is "<< name << endl;
  		memcpy(&my_ip, host->h_addr_list[0], host->h_length);
  		strncpy(ipstr, inet_ntoa(*(struct in_addr*)&my_ip), INET_ADDRSTRLEN);
  		cout << "my ip is " << ipstr <<endl;

	}

	void makeAck(int round){
		ack = (Ack*)malloc(sizeof(Ack));
		ack->type = (ACK);
		ack->size = (sizeof(Ack));
		ack->round = (round);
	}
	void makeByzantineMessage(ByztMsgNode *byznode, int round, int nodeid ){//int order, int round, int idcount, int id){
		byzmsg = allocatebyz(byznode->nu_ids + 1);
		byzmsg->type = (BYZANTINE);
		byzmsg->size = (sizeof(ByzantineMessage) + sizeof(uint32_t) * (byznode->nu_ids + 1));
		byzmsg->round = (round);
		byzmsg->order = (byznode->order);
		//	byzmsg->order = 2;
		int i = 0;
		for(i = 0; i < byznode->nu_ids; ++i){
			byzmsg->ids[i] = (byznode->ids[i]);  // 
		}
		byzmsg->ids[byznode->nu_ids] = (nodeid);
	}
	
	
	void printByzantineMessageids(int idcount, ByzantineMessage *msg){
		//	cout << byzmsg->order  << " this is order "<< endl;
		cout << "bznt message in round: "<< msg->round << endl;
		cout << "ids originally send the msg is: "; 
		for(int i = 0; i < idcount; ++i){
			cout << msg->ids[i] << " ";
		}
		cout << endl;
	}
	void printAck(Ack* ack){
		cout << "ack the round: " << ack->round << endl;
	}
	void initCommand(){
		set.fortest(myid, myorder);
	}
	void choise(){
	}
	void mainLoop();

};



class Lieutenant{
public:
	SetV set;
	MSG msg;
	ByzantineMessage* byzmsg;
	Ack* ack;
	uint32_t round;
	uint32_t f;
	int myid;
	char hname[128];
	char ipstr[INET_ADDRSTRLEN];
	char* hostfile;
	int cmdid;
	char *cmdport;

	ByzantineMessage* allocatebyz(int idcount){
		ByzantineMessage* p = (ByzantineMessage *)malloc(sizeof(ByzantineMessage) + sizeof(uint32_t) * idcount);
		return p;
	}
	Ack* allocateAck(){
		Ack* p = (Ack*)malloc(sizeof(Ack));
		return p;
	}
	Lieutenant(){}
	Lieutenant(char* msgport, char* hostfile, int f, int cmdid):msg(msgport){
		//need initialze the id of set and msg
		this->hostfile = hostfile;
		this->f = f;
		this->cmdid = cmdid;
		string file_name = this->hostfile;
		ifstream infile(file_name.c_str(),ios::in);
		string textline;
		int index = 0;
		while(getline(infile,textline,'\n')){    
			msg.iplist[index++] = textline;
		}  
		//cout << "there are " << index << " nodes" << endl;
		infile.close();
		IPList::iterator pos;
		for(pos = msg.iplist.begin(); pos != msg.iplist.end(); ++pos){
			cout << pos->first << " " << pos->second << endl;
		}
    	setmyIp();
		myid = msg.getidbyIp(ipstr);
		set.allPcounts = index;
		set.myid = myid;
		msg.myid = myid;
		cout << "my id is " << msg.myid << endl;
		round = 0;
		set.printV();
	}

void setmyIp(){

   		uint32_t my_ip;
   		char name[256];
  		gethostname(name, 255);
  		struct hostent *host = gethostbyname(name);
  		cout << "my host name is "<< name << endl;
  		memcpy(&my_ip, host->h_addr_list[0], host->h_length);
  		strncpy(ipstr, inet_ntoa(*(struct in_addr*)&my_ip), INET_ADDRSTRLEN);
  		cout << "my ip is " << ipstr <<endl;

	}

	void makeAck(int round){
		ack = (Ack*)malloc(sizeof(Ack));
		ack->type = (ACK);
		ack->size = (sizeof(Ack));
		ack->round = (round);
	}
	void makeByzantineMessage(ByztMsgNode *byznode, int round, int nodeid ){//int order, int round, int idcount, int id){
		byzmsg = allocatebyz(byznode->nu_ids + 1);
		byzmsg->type = (BYZANTINE);
		byzmsg->size = (sizeof(ByzantineMessage) + sizeof(uint32_t) * (byznode->nu_ids + 1));
		byzmsg->round = (round);
		byzmsg->order = (byznode->order);
		//	byzmsg->order = 2;
		int i = 0;
		for(i = 0; i < byznode->nu_ids; ++i){
			byzmsg->ids[i] = (byznode->ids[i]);  // 
		}
		byzmsg->ids[byznode->nu_ids] = (nodeid);
	}
	
	
	void printByzantineMessageids(int idcount, ByzantineMessage *msg){
		//	cout << byzmsg->order  << " this is order "<< endl;
		cout << "bznt message in round: "<< msg->round << endl;
		cout << "ids originally send the msg is: "; 
		for(int i = 0; i < idcount; ++i){
			cout << msg->ids[i] << " ";
		}
		cout << endl;
	}
	void printAck(Ack* ack){
		cout << "ack the round: " << ack->round << endl;
	}
	
	void choise(){
		set.printV();
	}
	void mainLoop();

};

#endif
