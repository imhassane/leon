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

unsigned int a[11];

//-----------------------------------------------------------


struct timespec ts_period(struct timespec start, struct timespec end);

void* POSIX_Init(void *argument)
{
    
    int status, i, j, k, temp;
    struct timespec start, end, time;

    status=clock_gettime(CLOCK_REALTIME, &start);
    if(status<0)
        returncode("gettime start",status);
    

    //---------------------- Espace de test ---------------------
    
    for(k = 0; k < 50000; k++)
    {     
        a[0] = 0;   /* assume all data is positive */
        a[1] = 11; a[2]=10;a[3]=9; a[4]=8; a[5]=7; a[6]=6; a[7]=5;
        a[8] =4; a[9]=3; a[10]=2;
        i = 2;
        while(i <= 10){
            j = i;
            while (a[j] < a[j-1])
            {
                temp = a[j];
                a[j] = a[j-1];
                a[j-1] = temp;
                j--;
            }
            i++;
        }
    }

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
