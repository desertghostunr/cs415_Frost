/**************************************************************

@file PA1.c

@brief the code for PA1 in cs415

@description Measures the time to send and receive integers

@author Andrew Frost

@version 1.00 ( 02/09/2017 )

@note None

***************************************************************/


// header files //////////////////////////////////////////////

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

//free function prototypes ///////////////////////////////////
long long GetCurrentMicroSecTime( );

double ConvertTimeToSeconds( long long usTime );


// main /////////////////////////////////////////////////////
int main( int argc, char *argv[ ] )
{
    long long eTime, sTime;
    double fTime;
    int taskID;
    int counter;
    int numberOfInts = 1, numberOfTests = 1;
    int* dataPtr = NULL;    

    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &taskID );
    
    if( argc > 1 )
    {
        numberOfInts = atoi( argv[ 1 ] );
    }

    if( argc > 2 )
    {
        numberOfTests = atoi( argv[ 2 ] );
    }
    

    if( numberOfInts <= 0 || numberOfTests <= 0 )
    {
        printf( "You specified %d ints and %d tests.\n", numberOfInts, numberOfTests );
        printf( "Both of these values must at least equal to 1.\n");
        printf( "Error from task %d.\n", taskID );
    }
    else
    {

        dataPtr = ( int * ) malloc( numberOfInts * sizeof( int ) );

	for( counter = 0; counter < numberOfInts; counter++ )
        {
	    dataPtr[ counter ] = counter;
	}

        if( taskID == 0 )
        {
	  
	    printf( "Transfering %d integers. The time in seconds will be displayed below:\n", numberOfInts);

	    for( counter = 0; counter < numberOfTests; counter++ )
	    {
                 sTime = GetCurrentMicroSecTime( ); //start timing

	         MPI_Send( &dataPtr[ 0 ], numberOfInts, MPI_INT, 1, 1, MPI_COMM_WORLD );


	         MPI_Recv( &dataPtr[ 0 ], numberOfInts, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );


	         eTime = GetCurrentMicroSecTime( ); //stop timing

	         fTime = ConvertTimeToSeconds( eTime - sTime ); //get time

                 printf( "%.9f\n", fTime );

	     
	    }
	   
        }
        else if( taskID == 1 )
        {
	   for( counter = 0; counter < numberOfTests; counter++ )
	   {
	  
                MPI_Recv( &dataPtr[ 0 ], numberOfInts, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE );

                MPI_Send( &dataPtr[ 0 ], numberOfInts, MPI_INT, 0, 0, MPI_COMM_WORLD );

           }
        }
        free( dataPtr );

        dataPtr = NULL;
    }

    MPI_Finalize( );

    return 0;
}

// free function implementation //////////////////////////////////

long long GetCurrentMicroSecTime( )
{
    long long retTime;
    
    struct timeval tVal;

    gettimeofday( &tVal, NULL );

    retTime = tVal.tv_sec * 1000 + tVal.tv_usec / 1000;

    return retTime;
}

double ConvertTimeToSeconds( long long usTime )
{
  return ( double ) usTime / 1000000.0;
}



