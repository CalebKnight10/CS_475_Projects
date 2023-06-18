#include <omp.h>
#include <stdio.h>
#include <math.h>

#define NUMT	        4	        // number of threads to use -- do once for 1 and once for 4
#define SIZE       	100000000	// array size -- you get to decide
#define NUMTRIES        30	        // how many times to run the timing to get reliable timing data

float A[SIZE];
float B[SIZE];
float C[SIZE];


double run_loops(int num) 
{
        // inialize the arrays:
	for( int i = 0; i < SIZE; i++ )
	{
		A[i] = 1.;
		B[i] = 2.;
	}

        omp_set_num_threads( num );

        double maxMegaMults = 0.;

        for( int t = 0; t < NUMTRIES; t++ )
        {
                double time0 = omp_get_wtime( );

                #pragma omp parallel for
                for( int i = 0; i < SIZE; i++ )
                {
                        C[i] = A[i] * B[i];
                }

                double time1 = omp_get_wtime( );
                double megaMults = (double)SIZE/(time1-time0)/1000000.;
                if( megaMults > maxMegaMults )
                        maxMegaMults = megaMults;
        }

        fprintf( stderr, "For %d threads, Peak Performance = %8.2lf MegaMults/Sec\n", num, maxMegaMults );

	// note: %lf stands for "long float", which is how printf prints a "double"
	//        %d stands for "decimal integer", not "double"

	// Speedup = (Peak performance for 4 threads) / (Peak performance for 1 thread)
        // float S = 1046.47/293.77;
        // // ^ 818 is the result of 4 threads, and 278 is the result of 1 thread next time clean up and don't hardcode
        // float Fp = (4./3.)*( 1. - (1./S) );
        // fprintf( stderr, "Parallel Fraction: %8.21lf\n", Fp);
        return maxMegaMults;

        return 0;
}

int
main( )
{
#ifdef   _OPENMP
	fprintf( stderr, "OpenMP version %d is supported here\n", _OPENMP );
#else
	fprintf( stderr, "OpenMP is not supported here\n" );
	exit( 0 );
#endif
        double singleThread = run_loops(1);
        double fourThreads = run_loops(4);

        double S = fourThreads / singleThread;
        fprintf( stderr, "Speed-up: %8.21lf\n", S); 
        double Fp = (4./3.)*( 1. - (1./S) );
        fprintf( stderr, "Parallel Fraction: %8.21lf\n", Fp);

	// // inialize the arrays:
	// for( int i = 0; i < SIZE; i++ )
	// {
	// 	A[i] = 1.;
	// 	B[i] = 2.;
	// }

        // omp_set_num_threads( NUMT );

        // double maxMegaMults = 0.;

        // for( int t = 0; t < NUMTRIES; t++ )
        // {
        //         double time0 = omp_get_wtime( );

        //         #pragma omp parallel for
        //         for( int i = 0; i < SIZE; i++ )
        //         {
        //                 C[i] = A[i] * B[i];
        //         }

        //         double time1 = omp_get_wtime( );
        //         double megaMults = (double)SIZE/(time1-time0)/1000000.;
        //         if( megaMults > maxMegaMults )
        //                 maxMegaMults = megaMults;
        // }

        // fprintf( stderr, "For %d threads, Peak Performance = %8.2lf MegaMults/Sec\n", NUMT, maxMegaMults );

	// // note: %lf stands for "long float", which is how printf prints a "double"
	// //        %d stands for "decimal integer", not "double"

	// // Speedup = (Peak performance for 4 threads) / (Peak performance for 1 thread)
        // float S = 1046.47/293.77;
        // // ^ 818 is the result of 4 threads, and 278 is the result of 1 thread next time clean up and don't hardcode
        // float Fp = (4./3.)*( 1. - (1./S) );
        // fprintf( stderr, "Parallel Fraction: %8.21lf\n", Fp);

        // return 0;
}