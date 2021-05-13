#include <stdio.h>
#include <proc.h>
#include<setjmp.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>


extern void xmain();

struct linklist
 {

    struct linklist *nextlink;

     int tid;

    
     int *ptr;
 

 };
 // here we are defineing linked list node
 typedef struct linklist n;



 struct messagebox
 {

      n *first;

      int message;

      n *last;
         
 };

 

 typedef struct messagebox xthread_mbox_t;

int xthread_init_mbox(xthread_mbox_t *mptr);

 int xthread_send(xthread_mbox_t *mptr, int msg);

int xthread_broadcast(xthread_mbox_t *mptr, int msg);

 void xthread_recv(xthread_mbox_t *mptr, int *msgptr);

struct xentry xtab[10]; 
int currxid = 0; 
// here the code handler in added in main.c file
// i can store the information of my queue.

int  xthread_init_mbox(xthread_mbox_t *mptr)
{

    mptr->first=mptr->last=NULL;

    mptr->message=-1;

    printf("\nInitialized our mail box\n");

   return 0;
}

int xthread_send(xthread_mbox_t *mptr, int msg)
{

   int usec=ualarm(0,0);

   
printf("\n sending  message to the thread \n");
   ualarm(usec,0);
   return 0;
}

int xthread_broadcast(xthread_mbox_t *mptr, int msg)
{

   int usec=ualarm(0,0);


printf("\n receving broadcast message from the thread \n");
   ualarm(usec,0);


   return 0;
}

 void xthread_recv(xthread_mbox_t *mptr, int *msgptr)
 {

  printf("\n receving  message from the thread \n");

resched();
 }



void handler()
{
     sigset_t set;

     //unblock sigalarm

     sigemptyset(&set);

     sigaddset(&set,SIGALRM);

     sigprocmask(SIG_UNBLOCK,&set,NULL);

    
     xtab[currxid].xstate = XREADY; 
      resched();
    // longjmp(env,1);// This will jump the  control to xmain function

}
void main(int argc, char *argv[])
{
   register struct xentry *xptr;
   struct xentry m;
   int i;
   int xidxmain;

   for(i=0 ; i < NPROC; i++){
      xptr = &xtab[i];
      xptr->xid = i;
      xptr->xlimit =  (WORD) malloc(STKSIZE);
      xptr->xbase = xptr->xlimit + STKSIZE - sizeof(WORD);
      xptr->xstate = XFREE;
   }



  

   /* the first thread runs user's xmain with id 0*/
   xidxmain = xthread_create(xmain, 2, argc, argv);
   xtab[xidxmain].xstate = XRUN; 

   signal(SIGALRM,handler);

   ualarm(10000,0);  // 0.01 sec

   ctxsw(m.xregs, xtab[xidxmain].xregs);

   
   /* never be here */


}


