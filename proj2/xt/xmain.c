#include <stdio.h>
#include <proc.h>

// test send and recv

int xidfoo, xidbar;
xthread_mbox_t m;

int foo()
{
    int i;

    printf("CISXT: entering foo ... \n");
    xthread_recv(&m,&i);
    printf("CISXT: foo got the lucky number: %d\n", i);
}


int bar()
{
    printf("CISXT: entering bar ... \n");
    xthread_send(&m,777);
    printf("CISXT: bar done \n");
}

xmain(int argc, char* argv[])
{
   xthread_init_mbox(&m);
   xidfoo = xthread_create(foo, 0);
   xidbar = xthread_create(bar, 0);
   printf("CISXT: Finish main\n");
}

