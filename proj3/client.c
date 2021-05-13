#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/un.h>
#include <signal.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>


#define BUFMAX 2048
#define QUERY 2000
#define UPDATE 2001
#define PORT 22551  
#define MSG "GET BANK620"

int main(void)
{
	
	char buf[BUFMAX];
	struct sockaddr_in rmt_udp,loc_udp,ser_tcp;
	int sk,len = sizeof(loc_udp),orig_sock;
	struct hostent *host;
	struct in_addr ipaddress;
	

	sk = socket(AF_INET, SOCK_DGRAM, 0);
	loc_udp.sin_family = AF_INET;  
	loc_udp.sin_addr.s_addr =  INADDR_ANY;  
	loc_udp.sin_port = 0; 
	if( bind(sk,(struct sockaddr *)&loc_udp,sizeof(loc_udp)) < 0){
		printf("Bind Failed \n");
		exit(0);
	}
	
	rmt_udp.sin_family = AF_INET;
	rmt_udp.sin_addr.s_addr = inet_addr("137.148.204.10");
	rmt_udp.sin_port = ntohs(PORT);
	setsockopt(sk,SOL_SOCKET,SO_BROADCAST,(struct sockaddr *)&rmt_udp,sizeof(rmt_udp));
	
	sendto(sk, MSG, strlen(MSG)+1,0,(struct sockaddr *)&rmt_udp, sizeof(rmt_udp)); 
	read(sk,buf,BUFMAX); 
	
	
	if(strcmp(buf,"Service Name not found!")==0){
		printf("%s \n",buf);
		exit(1);
	}
	
	char serverPort[10] = "";
	char qo[4];
	char re[4];
	char serverIp[20] = "";
	char *tableData;
	int count = 0;
	
		tableData = strtok(buf,",");
	while(tableData != NULL){
		if(count == 0 || count == 1 || count == 2){
			strcat(serverIp,tableData); 
			strcat(serverIp,".");
		}
		else if(count == 3){
			strcat(serverIp,tableData);
		}
		else if(count == 4){
			strcpy(qo,tableData); 
		}
		else if(count == 5){
			strcpy(re,tableData); 
		}
		tableData = strtok(NULL,",");
		count++;
	}
	int qut = atoi(qo);
	int rmd = atoi(re);
	int pNumber = (qut*256)+rmd;
	sprintf(serverPort, "%u", pNumber); 
	printf("Service Provided by %s at port %s\n",serverIp,serverPort); 
	close(sk); 
	
	
	inet_pton(AF_INET,serverIp,&ipaddress); 
	host = gethostbyaddr(&ipaddress,sizeof(ipaddress),AF_INET); 
	ser_tcp.sin_family = AF_INET;
	memcpy(&ser_tcp.sin_addr,host->h_addr,host->h_length);
	char *ptr;
	long ret;
	ret = strtoul(serverPort,&ptr,10);
	ser_tcp.sin_port = ntohs(ret);
	
	char user_msg[50];
	do{ 						
		printf("> ");
		fgets(user_msg,50,stdin); 
		user_msg[strcspn(user_msg,"\n")] = '\0';
		if(strcmp(user_msg, "quit")!=0){
			if((orig_sock = socket(AF_INET,SOCK_STREAM,0))<0){
				perror("generate error");
				return 3;
			}
			if(connect(orig_sock,(struct sockaddr *)&ser_tcp,sizeof(ser_tcp))<0){
				perror("Connect error");
				return 4;
			}
			char *queryToken;
			char queryType[10] = "";
			char accountNumber[10] = "";
			char accountBalance[10] = "";
			int count = 0;
			queryToken = strtok(user_msg," ");
			
			while(queryToken != NULL)
			{
				if(count == 0){
					strcpy(queryType,queryToken);
				}
				else if(count == 1){
					strcpy(accountNumber,queryToken);
				}
				else if(count == 2){
					strcpy(accountBalance,queryToken);
				}
				queryToken = strtok(NULL," ");
				count++;
			}
			if(strcmp(queryType,"query") == 0)
			{
				int queryNumber =  QUERY;
				queryNumber = htonl(queryNumber);
				write(orig_sock,&queryNumber,sizeof(queryNumber));
				int accNo = atoi(accountNumber);		
				accNo = htonl(accNo);
				write(orig_sock,&accNo,sizeof(accNo));  
				send(orig_sock,user_msg,len,0);
				char act_details[BUFMAX];
				recv(orig_sock,act_details,BUFMAX,0);
				printf("%s\n",act_details);
			}
			else if(strcmp(queryType,"update") == 0)
			{
				int queryNumber =  UPDATE;
				queryNumber = htonl(queryNumber);
				write(orig_sock,&queryNumber,sizeof(queryNumber));  
				int accNo = atoi(accountNumber);
				accNo = htonl(accNo);
				write(orig_sock,&accNo,sizeof(accNo));	
				float accBal = atof(accountBalance);
				uint32_t updateBalance;
				memcpy(&updateBalance, &accBal, sizeof(updateBalance));
				updateBalance = htonl(updateBalance);
				write(orig_sock,&updateBalance,sizeof(updateBalance));  
				send(orig_sock,user_msg,len,0);
				char act_details[BUFMAX];
				recv(orig_sock,act_details,BUFMAX,0);
				printf("%s\n",act_details);
			}
			close(orig_sock);
		}
	}while(strcmp(user_msg,"quit")!=0);
	return 0;	
}
