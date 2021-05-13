#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<errno.h>
#include<pthread.h>
#include<math.h>

void *foo(void *vargp)

{
    int size=atoi(vargp);
    float a[size];

    for(int i=0;i<size;i++)
    {
       a[i]=sqrt(i+1);
    }

    
}
void et(char *argv)
{

    float etime();
    pthread_t tid;
    etime();
    pthread_create(&tid,NULL,foo,argv);
    pthread_join(tid,NULL);
    float time=etime();
    printf("\n The toal time elapse is %f\n",time);

}