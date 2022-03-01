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
#include "monitor.h"

//----------------- Fonctions supplémentaires ----------------


int fib(int n)
{
  int  i, Fnew, Fold, temp,ans;

    Fnew = 1;  Fold = 0;
    for ( i = 2;
	  i <= 30 && i <= n;          /* apsim_loop 1 0 */
	  i++ )
    {
      temp = Fnew;
      Fnew = Fnew + Fold;
      Fold = temp;
    }
    ans = Fnew;
  return ans;
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
    
    //---------------------- Espace de test ---------------------



    int a, i;

    a = 32;
    for(i = 0; i < 100000; i++)    
        In = fib(a);
    
    printf("Résultat fonction récursive: %ld\n", In);



    //-----------------------------------------------------------
    
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

