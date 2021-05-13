#include <stdio.h>


int xidfoo, xidbar;
int x=0;

int foo(int f)
{
   int i,z;
   for(i=0;i<20;i++){
      printf("This is foo %d, %d\n", f, i);
      for(z=0;z<2000000;z++);
   }
}

int bar(int p, int q)
{
   int j,z;
   for(j=0;j<20;j++){
      printf("This is bar %d, %d\n", p-q, j);
      for(z=0;z<2000000;z++);
   }
}

xmain(int argc, char* argv[])
{
   xidfoo = xthread_create(foo, 1, 7);
   xidbar = xthread_create(bar, 2, 32, 12);
   printf("Finish main\n");

}


