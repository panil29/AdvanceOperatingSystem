#include <stdio.h>
#include <proc.h>

// test send and recv

int xidfoo, xidbar,xidoof;
xthread_mbox_t p,q,r;

int foo()
{
    int i,m;
    
    for(i=0;i<3;i++) {
       xthread_recv(&p,&m);
       printf("foo got the lucky number: %d\n", m++);
       xthread_send(&q,m);
    }
}

int bar()
{
    int i,m;

    for(i=0;i<3;i++) {
       xthread_recv(&q,&m);
       printf("bar got the lucky number: %d\n", m++);
       xthread_send(&r,m);
    }
}

int oof()
{
    int i,m;

    for(i=0;i<3;i++) {
       xthread_recv(&r,&m);
       printf("oof got the lucky number: %d\n", m++);
       xthread_send(&p,m);
    }
}


xmain(int argc, char* argv[])
{
   int msg =100;

   xthread_init_mbox(&p);
   xthread_init_mbox(&q);
   xthread_init_mbox(&r);
   xidfoo = xthread_create(foo, 0);
   xidbar = xthread_create(bar, 0);
   xidoof = xthread_create(oof, 0);
   xthread_send(&p,msg);
   printf("CISXT: Finish main\n");
}

