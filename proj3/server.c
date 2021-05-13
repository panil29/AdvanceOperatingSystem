#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <ctype.h>


#define PORT    22551               
#define MAX     1024       
#define BUFMAX 2048




struct record 
 {
     int   acctnum; 
     char  name[20];
     float value;
     char  phone[16];
     int   age;
 };

 void signal_catcher(int the_sig){
  wait(0);                             
 }

struct record rec;
void readFile(int accno, struct record *rec_da)
{
    
	int file;
	int flag = 0;
	int readSize;
	file = open("db21",O_RDWR);
	int size = sizeof(struct record);
	lseek(file,size,1);
	do{
		readSize = read(file,&rec,size);
		if(accno == rec.acctnum)
		{
			flag = 1;
			stpcpy(rec_da->name,rec.name);
			rec_da->acctnum=rec.acctnum;
			rec_da->value=rec.value;
		}
	}while(readSize > 0 && flag == 0);
	close(file);
}



void updateFile(int accno, float ele, struct record *rec_data)
{
	int file;
	int flag = 0;
	int readSize;
	file = open("db21",O_RDWR);
	int size = sizeof(struct record);
	lseek(file,size,1);
	do{
		readSize = read(file,&rec,size);
		if(accno == rec.acctnum)
		{
			lseek(file,-size,1);
			if(lockf(file,F_LOCK,size) == -1){
				printf("Cannot lock record");
				flag = 1;
				break;
			}
			read(file,&rec,size);		
			rec.value = rec.value + ele;
			lseek(file,-size,1);
			write(file,&rec,size);
			lockf(file,F_ULOCK,-size); 
			
			strcpy(rec_data->name,rec.name);
			rec_data->acctnum=rec.acctnum;
			rec_data->value=rec.value;
			flag = 1;
		}
	}while(readSize > 0 && flag == 0); 	
   	close(file);
}

void replacewithcomma(char *str,char old,char new){
	int len = strlen(str);
	int i;
	for(i=0;i<len;i++)
	{
		if(str[i]==old){
			str[i] = new;
		}
	}
}


int main(int argc,char* argv[]) {

struct sockaddr_in cli_tcp,ser_tcp,loc_udp,rmt_udp;


char buf[BUFMAX];

char msg[99]="PUT BANK620 ";

socklen_t clnt_len;            

int sk,rlen=sizeof(rmt_udp),len=sizeof(loc_udp);

int orig_sock,new_sock; 

sk = socket(AF_INET,SOCK_DGRAM,0);


 loc_udp.sin_family = AF_INET; 
 loc_udp.sin_addr.s_addr = INADDR_ANY; 
 loc_udp.sin_port = 0; 
  
  if(bind(sk,(struct sockaddr *)&loc_udp,sizeof(loc_udp)) <0) {
       printf("bind fails!\n");
       exit(0); 
  }

char *ip_buf;

char hst_buf[20];

int h_name;

struct hostent *h_entry;

h_name=gethostname(hst_buf,sizeof(hst_buf));

h_entry=gethostbyname(hst_buf);



ip_buf=inet_ntoa(*((struct in_addr*) h_entry->h_addr_list[0]));

char ser_String[40]="";

strcpy(ser_String,ip_buf);


getsockname(sk,(struct sockaddr *)&loc_udp,&len); 
printf("socket has port %d\n",htons(loc_udp.sin_port)); 
int serv_port=htons(loc_udp.sin_port);


char sock_Quot[5];

char sock_Rem[5];

sprintf(sock_Quot,"%d",serv_port/256);

sprintf(sock_Rem,"%d",serv_port%256);


strcat(ser_String,".");

strcat(ser_String,sock_Quot);

strcat(ser_String,".");

strcat(ser_String,sock_Rem);

replacewithcomma(ser_String,'.',',');

strcat(msg, ser_String);


rmt_udp.sin_family = AF_INET;

rmt_udp.sin_addr.s_addr = inet_addr("137.148.204.16"); 

rmt_udp.sin_port = ntohs(PORT);

setsockopt(sk,SOL_SOCKET,SO_BROADCAST,(struct sockaddr *)&rmt_udp,sizeof(rmt_udp));
	
sendto(sk,msg,strlen(msg)+1,0,(struct sockaddr *)&rmt_udp,sizeof(rmt_udp));
	
recvfrom(sk,buf,BUFMAX,0,(struct sockaddr *)&rmt_udp,&rlen); 
	
char ser_Ipaddress[30]="";
	
strcpy(ser_Ipaddress,inet_ntoa(rmt_udp.sin_addr));
	
printf("Registration %s from %s\n", buf,  ser_Ipaddress);
	
close(sk); 
	
if(signal(SIGCHLD,signal_catcher)==SIG_ERR){
		perror("Signal problem");
		exit(0);
	}
	
	if((orig_sock = socket(AF_INET,SOCK_STREAM,0)) < 0){
		perror("error generated");
		exit(0);
	}



	

ser_tcp.sin_family = AF_INET;		

ser_tcp.sin_addr.s_addr = INADDR_ANY;	

ser_tcp.sin_port = ntohs(serv_port);

printf("%d\n",serv_port);

	if(bind(orig_sock,(struct sockaddr *)&ser_tcp,sizeof(ser_tcp)) < 0)
	{
		close(orig_sock);
		perror("bind error");
		exit(0);
	}
	if(listen(orig_sock,5)<0)
	{
		close(orig_sock);
		perror("tcp listener error");
		exit(0);
	}
	
do {
    clnt_len = sizeof(cli_tcp);                 
    if ((new_sock = accept( orig_sock, (struct sockaddr *) &cli_tcp,&clnt_len)) < 0) {
      close(orig_sock);
      perror("accept error"); 
      return 5;
    }

    char cli_IPaddress[30]="";

    strcpy(cli_IPaddress,inet_ntoa(cli_tcp.sin_addr));

    printf("Service Requested from %s\n",inet_ntoa(cli_tcp.sin_addr));
		
	int length;

    if ( fork() == 0 ) {                        
         
         int database_Operations;

         recv(new_sock,&database_Operations,sizeof(int),0);

         database_Operations=ntohl(database_Operations);

         printf("database_Operations %d\n",database_Operations);

         if(database_Operations == 2000)
         {
             int acctnum;

             read(new_sock,&acctnum,sizeof(acctnum));

			 acctnum = ntohl(acctnum);

			 struct record myRecord;

			 readFile(acctnum,&myRecord);

			 char acct_Details[BUFMAX];

			 sprintf(acct_Details,"%s %d %0.1f %s",myRecord.name,myRecord.acctnum,myRecord.value,myRecord.phone);
			 
             send(new_sock,acct_Details,BUFMAX,0);
         }

      else if(database_Operations == 2001){ 
          				
                int acctnum;

				read(new_sock, &acctnum, sizeof(acctnum));

				acctnum = ntohl(acctnum);

				uint32_t element;

				read(new_sock, &element, sizeof(element));

				element = ntohl(element);

				float ele = *((float*)&element);

				struct record myRecord;

				updateFile(acctnum,ele,&myRecord);

				char act_Details[BUFMAX];

				sprintf(act_Details,"%s %d %0.1f %s",myRecord.name,myRecord.acctnum,myRecord.value,myRecord.phone);

				send(new_sock,act_Details,BUFMAX,0);
			} 
			close(new_sock);
			return 0;
		}
		else{
			close(new_sock);
		}
	}while(1); 

    return 0;
}
 


