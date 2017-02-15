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
long long GetCurrentMilliSecTime( );

// main /////////////////////////////////////////////////////
int main( int argc, char *argv[ ] )
{
    double fTime, sTime, eTime;
    int taskID;
    int numberOfInts = 1;
    int* dataPtr = NULL;    

    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &taskID );
    
    if( argc > 1 )
    {
        numberOfInts = atoi( argv[ 1 ] );
    }   

    if( numberOfInts <= 0 )
    {
        printf( "%d is not a valid number of ints to pass.\n", numberOfInts );
        printf( "Error from task %d.\n", taskID );
    }
    else
    {
        
        dataPtr = ( int * ) malloc( numberOfInts * sizeof( int ) );

        if( taskID == 0 )
        {            
            sTime = GetCurrentMilliSecTime( ); //start timing
            
            MPI_Send( &dataPtr[ 0 ], numberOfInts, MPI_INT, 1, 1, MPI_COMM_WORLD );

            MPI_Recv( &dataPtr[ 0 ], numberOfInts, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );

            eTime = GetCurrentMilliSecTime( ); //stop timing

            fTime = ( eTime - sTime ); //get time

            printf( "Transfered %d integers in %.9f seconds.\n", numberOfInts, fTime );
        }
        else if( taskID == 1 )
        {
            MPI_Recv( &dataPtr[ 0 ], numberOfInts, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE );

            MPI_Send( &dataPtr[ 0 ], numberOfInts, MPI_INT, 0, 0, MPI_COMM_WORLD );
        }

        free( dataPtr );

        dataPtr = NULL;
    }
    

    MPI_Finalize( );

    return 0;
}

// free function implementation //////////////////////////////////

long long GetCurrentMilliSecTime( )
{
    long long retTime;

    timeval tVal;

    gettimeofday( &tVal, NULL );

    retTime = tVal.tv_sec * 1000 + tVal.tv_usec / 1000;

    return ret;
}
