#define CONFIGURE_INIT
#include "system.h"
#include "returncode.h"
#include "random.h"
#include <signal.h>
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>



//----------------- Fonctions supplémentaires ----------------

int fib(int i)
{
    if(i==0)
        return 1;

    if(i==1)
        return 1;

    return fib(i-1) + fib(i-2);
}

int anka(int);

int kalle(int i)
{
    if(i<=0)
        return 0;
    else
        return anka(i-1);
}

int anka(int i)
{
    if(i<=0)
        return 1;
    else
        return kalle(i-1);
}

//-----------------------------------------------------------


struct timespec ts_period(struct timespec start, struct timespec end);

void* POSIX_Init(void *argument)
{
    
    int status;
    long In;
    struct timespec start, end, time;

    status=clock_gettime(CLOCK_REALTIME, &start);
    if(status<0)
        returncode("gettime start",status);
    
    //rtems_cpu_usage_reset();

    //---------------------- Espace de test ---------------------
   
    In = fib(32);
    
    printf("Résultat fonction récursive: %ld\n", In);


    //-----------------------------------------------------------

    rtems_cpu_usage_report();
    
    status=clock_gettime(CLOCK_REALTIME, &end);
    if(status<0)
        returncode("gettime end",status);
    
    time = ts_period(start, end);
    
    printf("\nTemps d'execution: %jd.%09ld \n", (intmax_t)time.tv_sec, time.tv_nsec);
    exit(0);
    return 0;
}

struct timespec ts_period(struct timespec start, struct timespec end)
{
    struct timespec res;
    
    res.tv_sec = end.tv_sec - start.tv_sec;
    res.tv_nsec = end.tv_nsec - start.tv_nsec;

    return(res);
}

