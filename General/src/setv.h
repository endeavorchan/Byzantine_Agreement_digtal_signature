/* Define the v set */
/* At most two elements in the set */

/* Func */
/* Addv */
/* Getv */
/* Contain v */
/* GetSenders */
/* Choise */
/* isEmpty */
#ifndef SETV_H
#define SETV_H
#include <netinet/in.h>
#include <iostream>
#include "msg.h"
using namespace std;

//typedef map <int, bool>  SendingList;  // int is id 
struct SendingList
{
	int id;
	bool issent;
};
struct ByztMsgNode{
	ByztMsgNode *next;
	bool tag;  // on add set to true, after sending set to false
	uint32_t type; // Must be equal to 1
	uint32_t nu_ids; // number of ids 
	uint32_t round; // round number
	uint32_t order; // the order (retreat = 0 and attack = 1)
	uint32_t nu_send; // sent id
	int needsendcount;
	SendingList* sendlist;
	uint32_t ids[]; // id's of the senders of the message
};

class SetV{
   public:
	int allPcounts;    // read the ip file to get it. Usage: get the receivers's id.
	ByztMsgNode *head;
	int myid;
	SetV(){ allPcounts = 0; head = NULL; myid = 0;}
	SetV(int count, int id){
		head = NULL;
		allPcounts = count;
		myid = id;
	}


	void SetSenders(ByztMsgNode *bmn){
	    int msgnu = bmn->nu_ids;
	    bool* allids = new bool[allPcounts];
	    for(int i = 0; i < allPcounts; ++i){
	    	allids[i] = false;
	    }
	    for(int i = 0; i < msgnu; ++i){
	    	allids[bmn->ids[i]] = true;
	    }
	    allids[myid] = true;
	    int needsend = 0;
	    for(int i = 0; i < allPcounts; ++i){
	    	if(!allids[i]){
	    		needsend ++;
	    	}
	    }
	    bmn->needsendcount = needsend;
	    bmn->sendlist = new SendingList[needsend];
	    int j = 0;
	    cout << "sending queue is : ";
	    for(int i = 0; i < allPcounts; ++i){
	    	if(!allids[i]){
	    		bmn->sendlist[j].id = i;
	    		bmn->sendlist[j].issent = false;
	    		j++;
	    		cout << i << " ";
	    	}
	    }
	    cout << endl;
	    for(int i = 0; i < bmn->needsendcount; ++i){
	    	cout << bmn->sendlist[i].id << " " << endl;
	    }
	}

	ByztMsgNode* Addv(ByzantineMessage * bzmsg);

	ByztMsgNode* Getv();

	bool ContainV(ByzantineMessage * bzmsg);

	bool isEmpty();

	void printV();
		
	void fortest(uint32_t comdid, uint32_t orders){
		//int msgnu = (msg->size - sizeof(ByzantineMessage)) / sizeof(uint32_t);
		ByztMsgNode * bmn = (ByztMsgNode*) malloc(sizeof(ByztMsgNode));
		bmn->tag = true;
		bmn->type = BYZANTINE;
		bmn->nu_ids = 0;
		bmn->nu_send = 0;
		bmn->round = 0;
		bmn->order = orders;
		bmn->next = NULL;
		head = bmn;
		SetSenders(head);
	}
};



#endif
