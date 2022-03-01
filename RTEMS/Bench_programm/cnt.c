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

#define MAXSIZE 1000

// Typedefs

typedef int matrix [MAXSIZE][MAXSIZE];

// Forwards declarations

int main(void);
int Test(matrix);
int Initialize(matrix);
int InitSeed(void);
void Sum(matrix);
int RandomInteger(void);

// Globals

int Seed;
matrix Array;
int Postotal, Negtotal, Poscnt, Negcnt;

//-----------------------------------------------------------


struct timespec ts_period(struct timespec start, struct timespec end);

void* POSIX_Init(void *argument)
{
    
    int status;
    struct timespec start, end, time;

    InitSeed();
    Initialize(Array);

    status=clock_gettime(CLOCK_REALTIME, &start);
    if(status<0)
        returncode("gettime start",status);
    

    //---------------------- Espace de test ---------------------
   

    Sum(Array);


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

// Intializes the given array with random integers.
int Initialize(matrix Array)
{
   register int OuterIndex, InnerIndex;

   for (OuterIndex = 0; OuterIndex < MAXSIZE; OuterIndex++) //100 + 1
      for (InnerIndex = 0; InnerIndex < MAXSIZE; InnerIndex++) //100 + 1
         Array[OuterIndex][InnerIndex] = RandomInteger();

   return 0;
}


// Initializes the seed used in the random number generator.


int InitSeed (void)
{
   Seed = 0;
   return 0;
}


void Sum(matrix Array)
{
  register int Outer, Inner;
  int Ptotal = 0; /* changed these to locals in order to drive worst case */
  int Ntotal = 0;
  int Pcnt = 0;
  int Ncnt = 0;


  for (Outer = 0; Outer < MAXSIZE; Outer++) //Maxsize = 100
    for (Inner = 0; Inner < MAXSIZE; Inner++)
#ifdef WORSTCASE
      if (Array[Outer][Inner] >= 0) {
#else
	if (Array[Outer][Inner] < 0) {
#endif
	  Ptotal += Array[Outer][Inner];
	  Pcnt++;
	}
	else {
	  Ntotal += Array[Outer][Inner];
	  Ncnt++;
	}

  Postotal = Ptotal;
  Poscnt = Pcnt;
  Negtotal = Ntotal;
  Negcnt = Ncnt;
}


// Generates random integers between 0 and 8095
int RandomInteger(void)
{
   Seed = ((Seed * 133) + 81) % 8095;
   return Seed;
}
