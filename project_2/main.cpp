#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// print debugging messages?
#ifndef DEBUG
#define DEBUG	false
#endif

omp_lock_t	Lock;
volatile int	NumInThreadTeam;
volatile int	NumAtBarrier;
volatile int	NumGone;

int	NowYear;		
int	NowMonth;		// 0 - 11


float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// rye grass height in inches
int	NowNumRabbits;		// number of rabbits in the current population
int nextNumRabbits;
int nowNumHunter;
int huntedRabbits;

unsigned int seed = 0;

const float RYEGRASS_GROWS_PER_MONTH =		20.0;
const float ONE_RABBITS_EATS_PER_MONTH =	 1.0;

const float AVG_PRECIP_PER_MONTH =	       12.0;	// average
const float AMP_PRECIP_PER_MONTH =		4.0;	// plus or minus
const float RANDOM_PRECIP =			2.0;	// plus or minus noise

const float AVG_TEMP =				60.0;	// average
const float AMP_TEMP =				20.0;	// plus or minus
const float RANDOM_TEMP =			10.0;	// plus or minus noise

const float MIDTEMP =				60.0;
const float MIDPRECIP =				14.0;

// float x = Ranf( &seed, -1.f, 1.f );

float
SQR( float x )
{
        return x*x;
}

float Ranf( unsigned int *seedp, float low, float high )
{
        float r = (float) rand( );              // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}

void
InitBarrier( int n )
{
        NumInThreadTeam = n;
        NumAtBarrier = 0;
	omp_init_lock( &Lock );
}


// have the calling thread wait here until all the other threads catch up:

void
WaitBarrier( )
{
        omp_set_lock( &Lock );
        {
                NumAtBarrier++;
                if( NumAtBarrier == NumInThreadTeam )
                {
                        NumGone = 0;
                        NumAtBarrier = 0;
                        // let all other threads get back to what they were doing
			// before this one unlocks, knowing that they might immediately
			// call WaitBarrier( ) again:
                        while( NumGone != NumInThreadTeam-1 );
                        omp_unset_lock( &Lock );
                        return;
                }
        }
        omp_unset_lock( &Lock );

        while( NumAtBarrier != 0 );	// this waits for the nth thread to arrive

        #pragma omp atomic
        NumGone++;			// this flags how many threads have returned
}

void Rabbits() {
  while( NowYear < 2029 )
  {
    int nextNumRabbits = NowNumRabbits;
    int carryingCapacity = (int)( NowHeight );
    if( nextNumRabbits < carryingCapacity )
            nextNumRabbits++;
    else
            if( nextNumRabbits > carryingCapacity )
                    nextNumRabbits--;
    
    if( nextNumRabbits < 0 )
            nextNumRabbits = 0;
            
    WaitBarrier( );
    
    NowNumRabbits = nextNumRabbits;
  
    WaitBarrier( );
    
    WaitBarrier( );
  }
}

void 
RyeGrass() {
        while(NowYear < 2029) {
        float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
        float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );
       
        float nextHeight = NowHeight;
        nextHeight += tempFactor * precipFactor * RYEGRASS_GROWS_PER_MONTH;
        nextHeight -= (float)NowNumRabbits * ONE_RABBITS_EATS_PER_MONTH;
        
        if( nextHeight < 0. ) nextHeight = 0.;
        // DoneComputing barrier:
	    WaitBarrier( );

        NowHeight = nextHeight;

	    // DoneAssigning barrier:
	    WaitBarrier( );

	    // DonePrinting barrier:
	    WaitBarrier( );
    }
}


void 
Watcher() {
    while( NowYear < 2029 ) {            
    WaitBarrier( );
    
    WaitBarrier( );
    
    printf("%d, %d, %f, %f, %d, %f, %d\n", NowYear, NowMonth, (5./9.)*(NowTemp-32), NowPrecip, NowNumRabbits, NowHeight, nowNumHunter);
    
    NowMonth++;
    if(NowMonth >= 12){
        NowYear++;
        NowMonth = 0;
    }
    
    float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );
    float temp = AVG_TEMP - AMP_TEMP * cos( ang );
    NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );
    
    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
    NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
    if( NowPrecip < 0. )
    	NowPrecip = 0.;
         
    WaitBarrier( );
  }
}

void 
MyAgent() {
    while (NowYear < 2029) {
        // temp var for number of rabbits
        huntedRabbits = NowNumRabbits;
        WaitBarrier( );
        // if there are more than 5 rabbits, add a hunter 
        if (huntedRabbits > 5) {
            nowNumHunter = 1;
        }
        // if there are 10 or more rabbits, add 2 hunters
        if (huntedRabbits >= 10) {
            nowNumHunter = 2;
        }
        // if there are not more than 5, make sure there are no hunters
        else if (huntedRabbits < 5) {
            nowNumHunter = 0;
        }
    
        // if there is a hunter, removed one rabbit
        if (nowNumHunter > 0 ) {
            huntedRabbits = huntedRabbits - 1;
        }
        // reassign total number of rabbits but add 1 so there is never negative rabbits
        NowNumRabbits = huntedRabbits + 1;

        WaitBarrier( );
        WaitBarrier( );
    }
}
int
main( int argc, char *argv[ ] )
{

    #ifdef _OPENMP
    	//fprintf( stderr, "OpenMP is supported -- version = %d\n", _OPENMP );
    #else
            fprintf( stderr, "No OpenMP support!\n" );
            return 1;
    #endif

    NowMonth = 0;
    NowYear = 2023;

    NowNumRabbits = 1;
    NowHeight = 1;
    nowNumHunter = 0;

    float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );
   
    float temp = AVG_TEMP - AMP_TEMP * cos( ang );
    NowTemp = temp + Ranf(&seed, -RANDOM_TEMP, RANDOM_TEMP );
    
    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
    NowPrecip = precip + Ranf(&seed, -RANDOM_PRECIP, RANDOM_PRECIP );

    if( NowPrecip < 0. )
        NowPrecip = 0.;


    InitBarrier( 4 );		// or 4
    omp_set_num_threads( 4 );	// same as # of sections
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            Rabbits( );
        }
        #pragma omp section
        {
            RyeGrass( );
        }
        #pragma omp section
        {
            Watcher( );
        }
        #pragma omp section
        {
            MyAgent( );	// your own
        }
        }       // implied barrier -- all functions must return in order
    // to allow any of them to get past here
    return 0;
}