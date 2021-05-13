#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<errno.h>

int main(int argc,char *argv[])
{
    void ep(int);
    void et(char *);
    
   // printf("Hello %s %d" ,argv[0],atoi(argv[1]));

    int size=atoi(argv[1]);
    if(strcmp("./pcrt",argv[0])==0)
    {

           ep(size);
    }
    if(strcmp("./tcrt",argv[0])==0)
    {
           et(argv[1]);
    }
}






