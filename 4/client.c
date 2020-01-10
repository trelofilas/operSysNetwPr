#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include "keyvalue.h"

#define BUFFER1_SIZE 1024
#define BUFFER2_SIZE 51

void error(char *msg){
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[])
{
	//socket()  connect()   write()  read()   close()
	int clientsockfd, socknect, clwrite, clread, clclose;
	ssize_t write_to_server,read_from_server;
	char buffer1[BUFFER1_SIZE], buffer2[BUFFER2_SIZE];
	//struct sockaddr_in server_addr;
	//socklen_t addr_size;
	struct addrinfo hostaddr;
	struct addrinfo *servinfo, *p;
	int i,j, counter;
	int getAI;
	char value[50];


	if(argc < 4){
		error("Arguements must be: host port [put,get] key (value)...");
	}

	//bzero(&hostaddr, sizeof(struct addrinfo));
	memset(&hostaddr, 0, sizeof(struct addrinfo));
	hostaddr.ai_family = AF_UNSPEC;
	hostaddr.ai_socktype = SOCK_STREAM;
	//hostaddr.ai_flags = 0;
	hostaddr.ai_protocol = IPPROTO_TCP;

	if( (getAI = getaddrinfo(argv[1], argv[2], &hostaddr, &servinfo) ) != 0 ){
		error("Address error\n");
	}

	for(p = servinfo; p!= NULL; p = p->ai_next){
		if((clientsockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1 ){
			continue;
		}

		if(socknect = connect(clientsockfd, p->ai_addr, p->ai_addrlen) != -1){
			break;
		}

		close(clientsockfd);
	}

	if(p == NULL){
		error("Could not connect");
	}

	freeaddrinfo(servinfo);

	bzero(buffer1,BUFFER1_SIZE);
	for(i=3; i<argc; i++){
		if(strcmp(argv[i], "put") == 0){
			strcat(buffer1, "p");
			strcat(buffer1, argv[i+1]);
			strcat(buffer1, "\0");
			strcat(buffer1, argv[i+2]);
			strcat(buffer1, "\0");
			i+=2;
		}else if(strcmp(argv[i], "get") == 0){
			strcat(buffer1, "g");
			strcat(buffer1, argv[i+1]);
			strcat(buffer1, "\0");
			i+=1;
		}else{
			error("Wrong input arguements...\n");
		}
	}	

	if((write_to_server = write(clientsockfd, buffer1, BUFFER1_SIZE)) == -1 ){
		error("Error on writing\n");
	}

	bzero(buffer2,BUFFER2_SIZE);
	while((read_from_server = read(clientsockfd, buffer2, BUFFER2_SIZE)) > 0 ){
		int j=0,m=0;
		if(buffer2[j] == 'f'){
			j++;
			while(buffer2[j] != '\0'){
				value[m] = buffer2[j];
				j++;
				m++;
			}
			printf("%s\n",value);
			bzero(value,50);
			bzero(buffer2,BUFFER2_SIZE);
		}else if(buffer2[j] == 'n'){
			printf("\n");
			bzero(buffer2,BUFFER2_SIZE);
		}else{
			bzero(buffer2,BUFFER2_SIZE);
			close(socknect);  
		}
	}

	if((clclose = close(socknect)) < 0 ){
		error("Error on closing");
	}


}
