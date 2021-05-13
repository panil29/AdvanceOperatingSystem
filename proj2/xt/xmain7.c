#include <stdio.h>
#include <proc.h>

// test broadcast and recv

int xidfoo,xidbar,xidoof,xidrab;
xthread_mbox_t p;

int foo()
{
    int m;
    
    xthread_recv(&p,&m);
    printf("foo got the lucky number: %d\n", m);
}

int bar()
{
    int m;

    xthread_recv(&p,&m);
    printf("bar got the lucky number: %d\n", m);
}

int oof()
{
    int m;

    xthread_recv(&p,&m);
    printf("oof got the lucky number: %d\n", m);
}

int rab()
{
    int m=777;

    xthread_send(&p,m);
    m = 888;
    xthread_broadcast(&p,m);
}
    

xmain(int argc, char* argv[])
{
   int msg =100;

   xthread_init_mbox(&p);
   xidfoo = xthread_create(foo, 0);
   xidbar = xthread_create(bar, 0);
   xidoof = xthread_create(oof, 0);
   xidrab = xthread_create(rab, 0);
   printf("CISXT: Finish main\n");
}

