#include "general.h"
void Commander::mainLoop(){
	fd_set fdset;
  	int timetic = -1;
  	int timeticround = 40;
    while (round < 1) {    /*need to continue to next round f*/  
	cout << "Iterate" << endl;
	struct timeval timeout={1,0}; 
	FD_ZERO(&fdset);
	FD_SET(msg.sockfd, &fdset);

	select(msg.sockfd+1, &fdset, NULL, NULL, &timeout);
	if (FD_ISSET(msg.sockfd, &fdset)) {   
	   // cout << "receive message" << endl;
	    void *p = NULL;
	    int type = msg.recvMessage(p);
	    /*insert the messages received in this round(discard messages not for this round), 
				must be a way to sort the messages */
		/*Send ACK*/
	    if(type == BYZANTINE){
  		    cout << "A Commander can't receive a byzantine msg. error" << endl;
			exit(1);
		}
		else if(type == ACK){
			Ack* rc_ack = (Ack*)p;
			//int round = ntohl(rc_ack->round);
			cout << "received a ack in main loop in " << round << " round" << endl;
			if(rc_ack->size == sizeof(Ack) && rc_ack->round == round){
				//set_bymsg(round);
				/*tell send this one has be received*/
				cout << "the received ip is "<< msg.s << endl;
				// to do   int recvid = retid(msg.s);
				// cout << 
				int recvid = msg.getidbyIp(msg.s);
				if(recvid == -1){ cout << "id error \n" << endl; }
				ByztMsgNode * byztnode = set.Getv();
				for(int i = 0; i < byztnode->needsendcount; ++i){
					if(recvid == byztnode->sendlist[i].id){
						byztnode->sendlist[i].issent = true;
						break;
					}
				}
			}
		}
		else{
			cout << "receiving error"<< endl;
		}
	    
	}
	timetic = (timetic + 1) % 3;
	/*forword (to all others who should receive) messages in the last round received. if not contine*/
	if(timetic == 0){
		ByztMsgNode * byztnode = set.Getv();
		if(byztnode){
			int sendtally = 0;
			for(int i = 0; i < byztnode->needsendcount; ++i){
				if(byztnode->sendlist[i].issent == false){
					sendtally++;
					makeByzantineMessage(byztnode,round, myid);
					msg.sendMessage(BYZANTINE, (void*)byzmsg, byztnode->sendlist[i].id); 
					cout << "byzmsg send to " << byztnode->sendlist[i].id << endl;  
				}
			}
			if(sendtally == 0){
				byztnode->tag = false;
				round++;
				timetic = -1;
				timeticround = 10;
				continue;
			}
		}
	}
	timeticround--;
	if(timeticround < 0){
		ByztMsgNode *byztnode = set.Getv();
		if(byztnode)
			byztnode->tag = false;
		round++;
		timetic = -1;
		timeticround = 10;	
	}
   }
	// cout << "final table is: "<< endl;
	//printmsgtable();
	//cout << "done in round " << round << endl;
   cout << "the Commander has sent all it's orders" << endl;
   set.printV();
}
void Lieutenant::mainLoop(){
	fd_set fdset;
  	int timetic = -1;
  	int timeticround = 40;
    while (round <= f+1) {    /*need to continue to next round f*/  
	//cout << "Iterate" << endl;
	struct timeval timeout={1,0}; 
	FD_ZERO(&fdset);
	FD_SET(msg.sockfd, &fdset);

	select(msg.sockfd+1, &fdset, NULL, NULL, &timeout);
	if (FD_ISSET(msg.sockfd, &fdset)) {   
	   // cout << "receive message" << endl;
	    void *p = NULL;
	    int type = msg.recvMessage(p);
	    /*insert the messages received in this round(discard messages not for this round), 
				must be a way to sort the messages */
		/*Send ACK*/
	    if(type == BYZANTINE){
	    	ByzantineMessage* rc_byzmsg = (ByzantineMessage*)p;
	    	
				
			cout << rc_byzmsg->round << "my round is " << round << endl;
			if(rc_byzmsg->round == round){
				int nu_ids = (rc_byzmsg->size - sizeof(ByzantineMessage)) / sizeof(uint32_t);
				cout << "received a byzantine msg in main loop from " << rc_byzmsg->ids[nu_ids - 1] <<endl; 
				//cout << "received a byzantine msg in main loop from " << byznode->ids[byznode->nu_ids-1] <<endl; 
				if(!set.ContainV(rc_byzmsg)){
					ByztMsgNode* byznode = set.Addv(rc_byzmsg);
					cout << "set is: "<< endl;
					set.printV(); 
				}
				makeAck(rc_byzmsg->round);
				cout << "preparing send ack" << endl;
				//cout << "num of ids " << byznode->nu_ids << endl;
				//cout << "receiving from node "<<  << endl;
				msg.sendMessage(ACK, (void*)ack, rc_byzmsg->ids[nu_ids-1]);
				cout << "ack sent" << endl;
				// can compare the ip to see if are equal
				
  		   		
  		   	}
	
		}
		else if(type == ACK){
			Ack* rc_ack = (Ack*)p;
			//int round = ntohl(rc_ack->round);
			cout << "received a ack in main loop in " << round << " round" << endl;
			if(rc_ack->size == sizeof(Ack) && rc_ack->round == round){
				//set_bymsg(round);
				/*tell send this one has be received*/
				cout << "the received ip is "<< msg.s << endl;
				// to do   int recvid = retid(msg.s);
				// cout << 
				int recvid = msg.getidbyIp(msg.s);
				if(recvid == -1){ cout << "id error \n" << endl; }
				ByztMsgNode * byztnode = set.Getv();
				for(int i = 0; i < byztnode->needsendcount; ++i){
					if(recvid == byztnode->sendlist[i].id){
						byztnode->sendlist[i].issent = true;
						break;
					}
				}
			}
		}
		else{
			cout << "receiving error"<< endl;
		}
	    
	}
	timetic = (timetic + 1) % 3;
	/*forword (to all others who should receive) messages in the last round received. if not contine*/
	if(timetic == 0){
		cout << "in round " << round << endl;
		ByztMsgNode * byztnode = set.Getv();
		if(byztnode && byztnode->round == round - 1){
			cout << "in round " << round  <<  " gonna send"<< endl;
			int sendtally = 0;
			for(int i = 0; i < byztnode->needsendcount; ++i){
				if(byztnode->sendlist[i].issent == false){
					sendtally++;
					makeByzantineMessage(byztnode,round, myid);
					msg.sendMessage(BYZANTINE, (void*)byzmsg, byztnode->sendlist[i].id); 
					cout << "byzmsg send to " << byztnode->sendlist[i].id << endl;  
				}
			}
			if(sendtally == 0 && timeticround < 0){
				cout << "set tag to false" << endl;
				byztnode->tag = false;
				round++;
				timetic = -1;
				timeticround = 18; 
				continue;
			}
		}
	}
	timeticround--;
	if(timeticround < 0){
		ByztMsgNode *byztnode = set.Getv();
		if(byztnode && byztnode->round != round){
			cout << "set tag to false" << endl;
			byztnode->tag = false;
		}	
		round++;
		timetic = -1;
		timeticround = 18;	
	}
		
	/*all messages in the last round had be succedly forwarded and
		 all messages should be received in this round has received enter next round*/
   }
	choise();
	// cout << "final table is: "<< endl;
	//printmsgtable();
	//cout << "done in round " << round << endl;
}



int main(int argc, char* argv[]){
/*
	cout << "****************" << endl;
	for(int i = 0; i < argc; ++i){
		cout << argv[i] << endl;
	}
	cout<< "**********" << endl;
*/
	char* port = argv[2];
	char* hostfile = argv[4];
	char* faulty = argv[6];
	int f = atoi(faulty);
	cout << f << endl;
	char* commander_id = argv[8];
	int cmdid = atoi(commander_id);
	
	if(argc > 9){
		char* order = argv[10];
		cout << "I'm the Commander" << endl;
		if(order[0] == 'r'){
			Commander cm(port, hostfile, f, cmdid, RETREAT);
			cm.mainLoop();
		}
		else if (order[0] == 'a'){
			Commander cm(port, hostfile, f, cmdid, ATTACK);
			cm.mainLoop();
		} 
		else{
			cout << "please input retreat or attack for the cmd" << endl;
			exit(1);
		}		
	}
	else{
		cout << "I'm the Lieutenant" << endl;
		Lieutenant lt(port, hostfile, f, cmdid);
		lt.mainLoop();
	}
	
	return 0;
}
