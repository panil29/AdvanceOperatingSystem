#include <stdio.h>
#include <proc.h>

// test send and recv

int xidfoo, xidbar;
xthread_mbox_t m;

int foo()
{
    int i;

    printf("Check return value of xthread_send: %d\n", xthread_send(&m,777));
    printf("Check return value of xthread_send: %d\n", xthread_send(&m,777));
}


xmain(int argc, char* argv[])
{
   xthread_init_mbox(&m);
   xidfoo = xthread_create(foo, 0);
   printf("CISXT: Finish main\n");
}

