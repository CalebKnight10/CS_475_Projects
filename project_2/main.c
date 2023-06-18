#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

omp_lock_t	Lock;
volatile int	NumInThreadTeam;
volatile int	NumAtBarrier;
volatile int	NumGone;

void	InitBarrier( int );
void	WaitBarrier( );



    unsigned int seed = 0;

    int	NowYear;		// 2023 - 2028
    int	NowMonth;		// 0 - 11

    float	NowPrecip;		// inches of rain per month
    float	NowTemp;		// temperature this month
    float	NowHeight;		// rye grass height in inches
    int	NowNumRabbits;		// number of rabbits in the current population

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


    float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

    float temp = AVG_TEMP - AMP_TEMP * cos( ang );
    float NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
    float NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
    if( NowPrecip < 0. )
    	NowPrecip = 0.;


    // starting date and time:
    int NowMonth =    0;
    int NowYear  = 2023;

    // starting state (feel free to change this if you want):
    int NowNumRabbits = 1;
    float NowHeight =  5.;



// specify how many threads will be in the barrier:
//	(also init's the Lock)

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
int main (int argc, char *argv[ ]) {
    omp_set_num_threads( 3 );	// or 4
    InitBarrier( 3 );		// or 4
}