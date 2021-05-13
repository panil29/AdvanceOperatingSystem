#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<malloc.h>
#include<errno.h>


void ep(int size)
{
    float etime();
    char *ch=(char *) calloc(size*size,sizeof(char));

    etime();
    int id1=fork();

    int id2=fork();

    while(wait(NULL)!=-1 || errno!=ECHILD)
    {
       // printf("Waited for a child to finish");
    }
    float time=etime();

    printf("\n The total elasped time is %f",time);
   
    printf("\n");
}