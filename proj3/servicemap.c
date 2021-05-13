#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<sys/types.h>


#define MSGTOServer "OK"
#define SIZE 50
#define BUFMAX 2048
#define PORT 22551  


struct myData
{
	char name[15];
	char data[30];
};

struct myData* s_table[SIZE];
struct myData* item;


struct myData* search(char ser_Name[15])
{
	int i= 0;
	while(s_table[i]!= NULL)
	{
		if(strcmp(s_table[i]->name,ser_Name)==0)
			return s_table[i];
		i++;
	}
	return NULL;
}


void insert(char ser_Name[15],char data[30])
{
	struct myData *item=(struct myData*) malloc(sizeof(struct myData));
	strcpy(item->data,data);
	strcpy(item->name,ser_Name);
	int i= 0;
	for(;s_table[i]!= NULL && strcmp(s_table[i]->name,"")!=0;)
		i++;
	s_table[i]= item;
}

int main()
{
	 do
	{
		struct sockaddr_in loc, rem;
		int sock, rlen= sizeof(rem),len= sizeof(loc);
		char buf[BUFMAX];
		sock= socket(AF_INET,SOCK_DGRAM,0); 
		loc.sin_family= AF_INET;
		loc.sin_addr.s_addr= INADDR_ANY;
		loc.sin_port = ntohs(PORT);
		if(bind(sock,(struct sockaddr *)&loc,sizeof(loc))< 0)
		{
			printf("Bind Failed!!!\n");
			exit(0);
		}
		recvfrom(sock,buf,BUFMAX,0,(struct sockaddr*)&rem,&rlen);
		
		char *ch;
		int count= 0;
		char q_type[10]= "";
		char ser_Name[15]= "";
		char ser_Data[20]= "";
		char ser_ip[20]= "";
		
		char f_value[30]= "";
		
		strcpy(ser_ip,inet_ntoa(rem.sin_addr));
		strcat(ser_ip,":");
		printf("Received From %s %s \n",ser_ip,buf);
	    ch= strtok(buf," ");
		while(ch!= NULL)
		{
			
			if(count==0)
				strcpy(q_type,ch);
			else if(count==1)
				strcpy(ser_Name,ch);
			else if(count==2)
				strcpy(ser_Data,ch);
			ch= strtok(NULL," ");
			count++;
		}
		if(strcmp(q_type,"PUT")==0)
		{
			strcat(f_value,ser_Data);
			insert(ser_Name,f_value);
			
			sendto(sock,MSGTOServer,strlen(MSGTOServer)+1,0,(struct sockaddr *) &rem, sizeof(rem));
		}
		else if(strcmp(q_type,"GET")==0)
		{
			item= search(ser_Name);
			if(item!=NULL)
			{
				sendto(sock,item->data,strlen(item->data)+1,0,(struct sockaddr *) &rem, sizeof(rem));
			}
			
			else
			{
				sendto(sock,"Service Name not found!",strlen("Service Name not found!")+1,0,(struct sockaddr *) &rem, sizeof(rem));
			}
		}
		close(sock);
	}while(1);
	return 0;
}