#include <stdio.h>


int xidfoo;
int x=0;

int foo(int f)
{
   int i,z;
   for(i=0;i<50;i++){
      printf("This is foo %d, %d\n", f, i);
      for(z=0;z<5000000;z++);
   }
}

xmain(int argc, char* argv[])
{
   xidfoo = xthread_create(foo, 1, 7);
   printf("Finish main\n");

}


