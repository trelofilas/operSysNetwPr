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
#define BUFFER2_SIZE 512

char keys[1000][50],values[1000][50];

int main(int argc, char *argv[])
{
	//socket()  bind()   listen()  accept()   read()  
	//write()   read()     close()
	char *port;
	int sockfd, newsockfd, n, read_size;
	const int on=1;
	struct addrinfo hints, *res, *ressave;
	socklen_t addrlen;
	struct sockaddr *cliaddr;
	char* host=NULL;
	int i, rc;
	char buffer1[BUFFER1_SIZE],buffer2[BUFFER2_SIZE];
	char key1[50], value1[50];

	if ( argc != 2 ) {
		fprintf(stderr,"tcpserver port \n");
		exit(0);
	}
	port = argv[1];

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags=AI_PASSIVE;
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype=SOCK_STREAM;
	hints.ai_protocol=IPPROTO_TCP;

	if((n= getaddrinfo(host, port, &hints, &res)) !=0)
		printf("tcp listen error for %s, %s", host, port);

	ressave=res;

	do{

		sockfd=socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		 
		if(sockfd<0)
			continue;   /*fail*/
		 
		//setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

		if(bind(sockfd, res->ai_addr, res->ai_addrlen)==0)
			break;     /*sucess*/

		close(sockfd);/*fail, try next one*/
	} while((res=res->ai_next) !=NULL);

	listen(sockfd, 5);

	if(&addrlen)
		addrlen=res->ai_addrlen;

	freeaddrinfo(ressave);
	
	cliaddr=malloc(addrlen);
	while(1){	 
	if((newsockfd=accept(sockfd, cliaddr, &addrlen))<0)
			{ //perror("accept");
				exit(0);
			}

	bzero(buffer1,BUFFER1_SIZE);
	bzero(buffer2,BUFFER2_SIZE);
	bzero(key1,50);
	bzero(value1,50);
	while( (read_size = read(newsockfd , buffer1 , BUFFER1_SIZE )) > 0 )
	{	
		int j,l=0,m=0,k=0;
		for(j=0;j<read_size;j++){
			if(buffer1[j] == 'p'){
				j++;
				while(buffer1[j] != '\0'){
					key1[m] = buffer1[j];
					j++;
					m++;
				}
				j++;
				key1[m] = '\0';
				m=0;
				while(buffer1[j] != '\0'){
					value1[l] = buffer1[j];
					j++;
					l++;
				}
				value1[l] = '\0';
				l=0;
				put(key1, value1);
				bzero(key1,50);
				bzero(value1,50);
			}else if(buffer1[j] == 'g'){
				j++;
				while(buffer1[j] != '\0'){
					key1[k] = buffer1[j];
					j++;
					k++;
				}
				strcpy(buffer2, get(key1));
				bzero(key1,50);
				k=0;
				write(newsockfd, buffer2, BUFFER2_SIZE);
				bzero(buffer2,BUFFER2_SIZE);
			}else{
				//if((buffer1[j] == '\0') && (buffer1[j+1] == '\0')){
					j=0;
					l=0;
					m=0;
					k=0;
					bzero(buffer1,BUFFER1_SIZE); 
					close(newsockfd);  
					close(sockfd);
					break;
				/*}else{
					j=0;
					l=0;
					m=0;
					k=0;
					bzero(buffer1,BUFFER1_SIZE); 
					close(newsockfd);  
					//close(sockfd);
					break;
				}*/
			}				
		}
		bzero(buffer1,BUFFER1_SIZE); 
	}
	//bzero(buffer2,BUFFER2_SIZE); 
	close(newsockfd); 
	close(sockfd);
	}
}

char *get(char *key){
	int i,flag = 0;
	char *value1;
	char *value2;
	int valuesize;
	value1 = (char*)malloc(51*sizeof(char));
	value2 = (char*)malloc(51*sizeof(char));
	value1[0]='n';
	value2[0]='f';
	for(i=0; i<1000; i++){
		if(strcmp(keys[i],key) == 0){
			flag = 1;
			valuesize = strlen(values[i]);
			int j=0;
			while( j < valuesize){
				value2[j+1] = values[i][j];
				j++;
			}
			value2[j+1] = '\0';
			break;
		}
	}
	if(flag == 1){
		return value2;
	}else{
		return value1;
	}
}

void put(char *key, char *value){
	int i,j=0,flag = 0;
	int keysize = strlen(key);
	int valuesize = strlen(value);
	for(i=0; i<1000; i++){
		if(strcmp(keys[i], key) == 0){
			bzero(values[i], 50);
			while( j < valuesize){
				values[i][j] = value[i];
				j++;
			}
			flag == 1;
			break;
		}
	}
	j=0;
	if(flag == 0){
		for(i=0; i<1000; i++){
			if(strcmp(keys[i], "\0") == 0){
				while(j < 50){
					if(j<keysize){
						keys[i][j] = key[j];
					}
					if(j<valuesize){
						values[i][j] = value[j];
					}
					j++;
				}
				break;
			}
		
		}
	}
}
