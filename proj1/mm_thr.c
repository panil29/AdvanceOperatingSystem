#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<errno.h>
#include<pthread.h>
#include<math.h>
#include<malloc.h>

float a[240][240],b[240][240],c[240][240];
pthread_barrier_t barrier;
int threadcount;

float timings;
void *computation(void *args)
{
    float etime();
    int k=*((int *) args);
    int m=240/threadcount;
    
    printf("\n%d thread waiting at the barrier\n",k);

    pthread_barrier_wait(&barrier);
   
     etime();
     for(int i=k*m;i<=((k+1)*m)-1;i++)
     {
         for(int j=k*m;j<=((k+1)*m)-1;j++)
         {
             c[i][j]=0;

             for(int l=k*m;l<=((k+1)*m)-1;k++)
             {
                 c[i][j]+=a[i][l]*b[l][j];
             }
         }
     }
     timings=etime();

    //printf("We passed the barrier \n");
}

int main(int argc,char *argv[])
{
   int i,j;
    for(i=0;i<240;i++)
    {
        for(j=0;j<240;j++)
        {
        a[i][j]=rand();
        b[i][j]=rand();
    }
    }
    threadcount=atoi(argv[1]);

    pthread_t threads[threadcount];

    pthread_barrier_init(&barrier,NULL,threadcount);
   
    
    for(i=0;i<threadcount;i++)
    {
       
       int *id = malloc(sizeof(int));

        *id=i;
        pthread_create(&threads[i],NULL,&computation,id);
    }    
    for(i=0;i<threadcount;i++)
    {
        pthread_join(threads[i],NULL);
    }
  pthread_barrier_destroy(&barrier);
 
   // printf("The thread count is %d",threadCount);
   printf("The total time taken is %f\n",timings);

  return 0;
}

