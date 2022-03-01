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

/* configuration information */
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_TIMER_DRIVER
#define CONFIGURE_TICKS_PER_TIMESLICE        0
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_MAXIMUM_TASKS 3
#define CONFIGURE_INIT
#include <rtems/confdefs.h>

struct timespec start;
struct timespec ts_period(struct timespec start, struct timespec end);
void chronoStart();
struct timespec chronoGetTime();
void printTime(struct timespec time);

#define BENCHMARKS 50000

//----------------- Fonctions supplémentaires ----------------

rtems_task Task02(rtems_task_argument ignored);
rtems_task Init(rtems_task_argument ignored);

rtems_id           Task_id[2];
rtems_name         Task_name[2];
struct timespec    loop_overhead;
struct timespec    dir_overhead;
unsigned long      count1, count2;
rtems_status_code  status;


rtems_task Task02( rtems_task_argument ignored )
{
    struct timespec telapsed;

    /* All overhead accounted for now, we can begin benchmark */
    chronoStart();

    for ( count1 = 0; count1 < BENCHMARKS - 1; count1++ ) {
        rtems_task_wake_after( RTEMS_YIELD_PROCESSOR );
    }

    telapsed = chronoGetTime();
    printf("Rhealstone: Task switch\n");
    printf("telapsed: ");
    printTime(telapsed);
    printf("loop_overhead: ");
    printTime(loop_overhead);
    printf("dir_overhead: ");
    printTime(dir_overhead);
    exit(0);
}

rtems_task Task01( rtems_task_argument ignored )
{
    rtems_task_start( Task_id[1], Task02, 0 );

    /* Yield processor so second task can startup and run */
    rtems_task_wake_after( RTEMS_YIELD_PROCESSOR );

    for ( count2 = 0; count2 < BENCHMARKS; count2++ ) {
        rtems_task_wake_after( RTEMS_YIELD_PROCESSOR );
    }

}


//-----------------------------------------------------------



void* POSIX_Init(void *argument)
{
    
    int status;
    struct timespec time;
    
    chronoStart();

    //---------------------- Espace de test ---------------------
    
    Task_name[0] = rtems_build_name( 'T','A','0','1' );
    rtems_task_create(
        Task_name[0],
        30,
        RTEMS_MINIMUM_STACK_SIZE,
        RTEMS_DEFAULT_MODES,
        RTEMS_DEFAULT_ATTRIBUTES,
        &Task_id[0]
    );

    Task_name[1] = rtems_build_name( 'T','A','0','2' );
    status = rtems_task_create(
        Task_name[1],
        30,
        RTEMS_MINIMUM_STACK_SIZE,
        RTEMS_DEFAULT_MODES,
        RTEMS_DEFAULT_ATTRIBUTES,
        &Task_id[1]
    );

    /* find overhead of routine (no task switches) */
    chronoStart();
    for ( count1 = 0; count1 < BENCHMARKS - 1; count1++ ) {
        /* rtems_task_wake_after( RTEMS_YIELD_PROCESSOR ) */
    }
    for ( count2 = 0; count2 < BENCHMARKS; count2++ ) {
        /* rtems_task_wake_after( RTEMS_YIELD_PROCESSOR ) */
    }
    loop_overhead = chronoGetTime();

    /* find overhead of rtems_task_wake_after call (no task switches) */
    chronoStart();
    rtems_task_wake_after( RTEMS_YIELD_PROCESSOR );
    dir_overhead = chronoGetTime();

    status = rtems_task_start( Task_id[0], Task01, 0);

    status = rtems_task_delete( RTEMS_SELF);

    //-----------------------------------------------------------

    time = chronoGetTime();

    printTime(time);
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

void chronoStart()
{
    int status;

    status=clock_gettime(CLOCK_REALTIME, &start);
    if(status<0)
        returncode("gettime start",status);
}

struct timespec chronoGetTime()
{
    int status;
    struct timespec end;

    status=clock_gettime(CLOCK_REALTIME, &end);
    if(status<0)
        returncode("gettime start",status);

    return ts_period(start, end);
}

void printTime(struct timespec time){
    printf("Temps d'execution: %jd.%09ld \n", (intmax_t)time.tv_sec, time.tv_nsec);
}
