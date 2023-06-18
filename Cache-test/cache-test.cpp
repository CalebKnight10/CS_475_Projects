#include <omp.h>
#include <stdio.h>
#include <math.h>

#ifndef SIZE
#define SIZE           20000    // array size -- you get to decide
//#define SIZE          180    // array size -- you get to decide
#endif

#define NUMTRIES        10    // how many times to run the timing to get reliable timing data

int A[SIZE][SIZE];

int
main( )
{
    double ijPerformance = 0.;
    double jiPerformance = 0.;
    int sum=0;


    // bust up cache by reinitializing array with new values
    for (int i=0;i<SIZE;i++)
    {
        for(int j=0;j<SIZE;j++)
        {
            A[i][j]=(int)(rand()*10.);
        }
    }

    for( int t = 0; t < NUMTRIES; t++ )
    {
            double time0 = omp_get_wtime( );
            for( int i = 0; i < SIZE; i++ ) {
                for( int j = 0; j < SIZE; j++ ) {
                    sum += A[i][j];
                }
            }

            double time1 = omp_get_wtime( );
            double performance = (double)(SIZE*SIZE)/(time1-time0)/1000./1000.;
            if( performance > ijPerformance )
                    ijPerformance = performance;
    }

    // bust up cache by reinitializing array with new values
    for (int i=0;i<SIZE;i++)
    {
        for(int j=0;j<SIZE;j++)
        {
            A[i][j]=(int)(rand()*10.);
        }
    }
    sum=0;

    for( int t = 0; t < NUMTRIES; t++ )
    {
            double time0 = omp_get_wtime( );
            for( int i = 0; i < SIZE; i++ ) {
                for( int j = 0; j < SIZE; j++ ) {
                    sum += A[j][i];
                }
            }

            double time1 = omp_get_wtime( );
            double performance = (double)(SIZE*SIZE)/(time1-time0)/1000./1000.;
            if( performance > jiPerformance )
                    jiPerformance = performance;
    }

    printf( "%ld, %8.2lf, %8.2lf\n", sizeof(A)/1024, ijPerformance, jiPerformance);

    return 0;
}