#include <stdio.h>
#include <string.h>
#include<stdlib.h>//included stdlib.h(addedline)
const int N = 80000;
void applepie(char *tptr,int seed)
{   
    char title[29];//increased value from 24 to 29
    double x,y; 
    int i; float count=0;//changed int value to float (added line)
    srand48(seed);
    for(i=0; i<N; i++ ) {
         x= drand48();
         y= drand48();
         if(x*x+y*y < 1) count++;
    }
    strcpy(title,tptr);
    printf("%s \n applepie = %f\n", title, count / N * 4.0);
}

