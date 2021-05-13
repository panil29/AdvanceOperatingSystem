#include<stdio.h>
#include<sys/time.h>
#include<time.h>
#include<math.h>

float etime()
{
    // static float variable
    static float elapsed_time=0.0;

    // struct variable;

    struct timeval getTime;

    gettimeofday(&getTime,(struct timezone *)NULL);

    elapsed_time=(elapsed_time+getTime.tv_usec)/1000000;

    return elapsed_time;

} 