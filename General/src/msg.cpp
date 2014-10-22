#include "msg.h"
	
MSG::MSG(char *msgport){
	cout << "initial MSG " << endl;

	struct sockaddr_in saddr;
  	uint32_t my_ip;
  	char name[256];
  	gethostname(name, 255);
  	struct hostent *host = gethostbyname(name);
  	cout << "my host name is "<< name << endl;
  	memcpy(&my_ip, host->h_addr_list[0], host->h_length);
  	cout <<"c " << inet_ntoa(*(struct in_addr*)&my_ip) << endl;
  	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  	memset(&saddr, 0, sizeof(saddr));
  	saddr.sin_family = AF_INET;
  	saddr.sin_addr.s_addr = my_ip;
  	myport = atoi(msgport);
  	cout << "my port is " << myport << endl;
  	saddr.sin_port = htons(myport);
  	bind(sockfd, (struct sockaddr*)&saddr, sizeof(struct sockaddr));

}

void MSG::sendMessage(int type, void* p, int dest_id){

	uint32_t dest_ip;
	struct sockaddr_in saddr;
	IPList::iterator pos = iplist.find(dest_id);
	struct hostent *host = gethostbyname(pos->second.c_str());
	memcpy(&dest_ip, host->h_addr_list[0], host->h_length);
	//cout << "the ip of the dest is "<< pos->second << endl;
    char buf[MAXBUFLEN];
	memset(&saddr, 0, sizeof(saddr));
  	saddr.sin_family = AF_INET;
  	saddr.sin_addr.s_addr = dest_ip;
  	saddr.sin_port = htons(myport);
  	cout << "sending port is " << myport << endl;

	if(type == BYZANTINE){
		ByzantineMessage* byzmsg = (ByzantineMessage*)p;
		char *mp = (char*) p;
		//cout << "2 " << mp << endl;


		for(int i = 0; i < (byzmsg->size); ++i){
			buf[i] = mp[i];
		}
		cout << "size :"<< (byzmsg->size ) << endl;
		
		
		if ((sendto(sockfd, buf, (byzmsg->size), 0, (struct sockaddr*)&saddr, sizeof(struct sockaddr_in))) == -1) {
			perror("talker: sendto");
			exit(1);
		}
		
		freebyz(byzmsg);
	}
	else if(type == ACK){
		Ack* ack = (Ack*)p;
		char *mp = (char*)p;
		cout << "ack size "<< (ack->size) << endl;
		for(int i = 0; i < (ack->size); ++i){
			buf[i] = mp[i];
		}
		if ((sendto(sockfd, buf, (ack->size), 0, (struct sockaddr*)&saddr, sizeof(struct sockaddr_in))) == -1) {
			perror("talker: sendto");
			exit(1);
		}
		freeack(ack);
	}
	else{
		cout << "message content error\n";
	}
}


int  MSG::recvMessage(void *&pmsg){
	//struct sockaddr_storage their_addr;
	static char buf[MAXBUFLEN];
	//socklen_t addr_len;
	//int numbytes;
	printf("listener: waiting to recvfrom...\n");
	struct sockaddr_in addr;
  	socklen_t addrlen = sizeof(struct sockaddr_in);
  	int bytes = recvfrom(sockfd, buf, MAXBUFLEN, 0, (struct sockaddr*)&addr, &addrlen);
  	if(bytes == -1){
  		perror("recvfrom");
		return -1;
  	}

	strncpy(s, inet_ntoa(addr.sin_addr), INET_ADDRSTRLEN);

	uint32_t *ptype = (uint32_t*)buf;
	if((*ptype) == BYZANTINE){
		ByzantineMessage* byzmsg = (ByzantineMessage*)buf;
		byzmsg->type = (byzmsg->type);
		byzmsg->size = (byzmsg->size);
		byzmsg->round = (byzmsg->round);
		//cout << "round is " << byzmsg->round << endl;
		byzmsg->order = (byzmsg->order);
		//cout << "order is " << byzmsg->order << endl;
		int idcounts = (byzmsg->size - sizeof(ByzantineMessage)) / sizeof(uint32_t);
		cout << "idcounts is " << idcounts << endl;
		for(int i = 0; i < idcounts; ++i){
			byzmsg->ids[i] = (byzmsg->ids[i]);
		}
  		cout << "received byzantine msg from " << byzmsg->ids[idcounts-1] <<endl;
		//printByzantineMessageids(nids);
		pmsg = (void*)buf; 
		return BYZANTINE;
		
	}
	else if((*ptype) == ACK){
		Ack* ack = (Ack*)buf;
		ack->type = (ack->type);
		ack->size = (ack->size);
		ack->round = (ack->round);
		pmsg = (void*)buf; //(Ack*)pmsg = ack;
		return ACK;
	}
	else{
		cout << "received message error" << endl;
		return -1;
	}
	return -1;
}
