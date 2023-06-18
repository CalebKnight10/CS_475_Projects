#include <OpenCL/opencl.h> 
#include <OpenCL/cl_platform.h>
#include <omp.h>
#include <stdio.h>
#include <math.h>

int
main( )
{
#ifndef _OPENMP
        fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
        return 1;
#endif

        omp_set_num_threads( 2 );
        fprintf( stderr, "OpenMP is supported. Yes! \n" );
        return 0;
}

