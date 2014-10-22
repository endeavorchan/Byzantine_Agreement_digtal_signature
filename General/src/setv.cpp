/* Addv */
/* Getv */
/* Contain v */
/* SetSenders */
/* Choise */
/* isEmpty */
#include "setv.h"
ByztMsgNode* SetV::Addv(ByzantineMessage * msg){
	int msgnu = (msg->size - sizeof(ByzantineMessage)) / sizeof(uint32_t);
	ByztMsgNode * bmn = (ByztMsgNode*) malloc(sizeof(ByztMsgNode) + sizeof(uint32_t) * msgnu);
	bmn->tag = true;
	bmn->type = msg->type;
	bmn->nu_ids = msgnu;
	bmn->nu_send = 0;
	bmn->round = msg->round;
	bmn->order = msg->order;
	bmn->next = NULL;
	cout << "previously sent id are : ";
	for(int i = 0; i < msgnu; ++i){  
		bmn->ids[i] = msg->ids[i];
		cout << bmn->ids[i] << " ";
	}
	cout << endl;

	SetSenders(bmn);

	if(isEmpty()){
		head = bmn;
		return bmn;
	}
	else{
		ByztMsgNode *ptnode = head;
		while(ptnode->next != NULL){
			ptnode = ptnode->next;
		}
		ptnode->next = bmn;
		return bmn;
	}
	return NULL;
}

ByztMsgNode* SetV::Getv(){
	ByztMsgNode *ptnode = head;
	if(isEmpty()){
		cout << "head is null" << endl;
		return NULL;
	}
	else{
		while(ptnode){
			if(ptnode->tag == true)
				return ptnode;
			else
				ptnode = ptnode->next;
		}
	}
	return ptnode;
}

bool SetV::ContainV(ByzantineMessage * bzmsg){
	ByztMsgNode *ptnode = head;
	while(ptnode){
		if(ptnode->order == bzmsg->order){
			return true;
		}
		ptnode = ptnode->next;
	}
	return false;
}

bool SetV::isEmpty(){
  	if(head)
  		return false;
  	else
  		return true;
}

void SetV::printV(){
	ByztMsgNode * temp = head;
	if(isEmpty()) cout << "the table is null" << endl;
	while(temp){
		if(temp->order == ATTACK){
			cout << "this message is from ";
			for(int i = 0; i < temp->nu_ids; ++i){
				cout << temp->ids[i] << "**";
			}

			cout << "    the order is attack" << temp->order << endl;
		}
		else if(temp->order == RETREAT){
			cout << "this message is from ";
			for(int i = 0; i < temp->nu_ids; ++i){
				cout << temp->ids[i] << "**";
			}
		 	cout << "    the order is retreat " << temp->order << endl;
		}
		else {
			cout << "this message is from ";
			for(int i = 0; i < temp->nu_ids; ++i){
				cout << temp->ids[i] << "**";
			}
			cout << "    the order is " << temp->order << endl;
		}
		temp = temp->next;
	}

}

