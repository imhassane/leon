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

#define UPPSALAWCET 0


/* ***UPPSALA WCET***:
   disable stupid UNIX includes */
#ifndef UPPSALAWCET
#include <sys/types.h>
#include <sys/times.h>
#endif


//----------------- Fonctions supplémentaires ----------------

#define UPPERLIMIT 150

typedef int matrix [UPPERLIMIT][UPPERLIMIT];

int Seed;
matrix ArrayA, ArrayB, ResultArray;

/* Our picky compiler wants prototypes! */
void Multiply(matrix A, matrix B, matrix Res);
void InitSeed(void);
void Initialize(matrix Array);
int RandomInteger(void);

//-----------------------------------------------------------


struct timespec ts_period(struct timespec start, struct timespec end);

void* POSIX_Init(void *argument)
{
    
    int status;
    struct timespec start, end, time;

    InitSeed();
    Initialize(ArrayA);
    Initialize(ArrayB);

    status=clock_gettime(CLOCK_REALTIME, &start);
    if(status<0)
        returncode("gettime start",status);
    

    //---------------------- Espace de test ---------------------

    Multiply(ArrayA, ArrayB, ResultArray);

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

void InitSeed(void)
/*
 * Initializes the seed used in the random number generator.
 */
{
  /* ***UPPSALA WCET***:
     changed Thomas Ls code to something simpler.
   Seed = KNOWN_VALUE - 1; */
  Seed = 0;
}

void Initialize(matrix Array)
/*
 * Intializes the given array with random integers.
 */
{
   int OuterIndex, InnerIndex;

   for (OuterIndex = 0; OuterIndex < UPPERLIMIT; OuterIndex++)
      for (InnerIndex = 0; InnerIndex < UPPERLIMIT; InnerIndex++)
         Array[OuterIndex][InnerIndex] = RandomInteger();
}


int RandomInteger(void)
/*
 * Generates random integers between 0 and 8095
 */
{
   Seed = ((Seed * 133) + 81) % 8095;
   return (Seed);
}

void Multiply(matrix A, matrix B, matrix Res)
/*
 * Multiplies arrays A and B and stores the result in ResultArray.
 */
{
   register int Outer, Inner, Index;

   for (Outer = 0; Outer < UPPERLIMIT; Outer++)
      for (Inner = 0; Inner < UPPERLIMIT; Inner++)
      {
         Res [Outer][Inner] = 0;
         for (Index = 0; Index < UPPERLIMIT; Index++)
            Res[Outer][Inner]  +=
               A[Outer][Index] * B[Index][Inner];
       }
}
