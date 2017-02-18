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
unsigned long long GetCurrentMicroSecTime( );

double ConvertTimeToSeconds( unsigned long long usTime );

// main /////////////////////////////////////////////////////
int main( int argc, char *argv[ ] )
{
    unsigned long long eTime, sTime;
    double fTime, avgTime;
    int taskID;
    int counter, tCounter;
    int numberOfInts = 1, numberOfTests = 1, timingTest = 1;
    int* dataPtr = NULL;
    int* timingTestCountPtr = NULL;

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

    if( argc > 3 )
    {
        timingTest = atoi( argv[ 3 ] );
    }

    if( numberOfInts <= 0 || numberOfTests <= 0 || timingTest <= 0 )
    {
        printf( "You specified %d ints, %d tests, and %d timing test mode.\n", numberOfInts, numberOfTests, timingTest );
        printf( "All of these values must be at least equal to 1.\n");
        printf( "Error from task %d.\n", taskID );
    }
    else
    {

        dataPtr = ( int * ) malloc( numberOfInts * sizeof( int ) );

        for( counter = 0; counter < numberOfInts; counter++ )
        {
            dataPtr[ counter ] = counter + 1;
        }

        timingTestCountPtr = ( int * ) malloc( timingTest * sizeof( int ) );

        for( tCounter = 0; tCounter < timingTest; tCounter++ )
        {
            timingTestCountPtr[ tCounter ] = numberOfInts / ( timingTest / ( tCounter + 1 ) );

            if( timingTestCountPtr[ tCounter ] < 1 )
            {
                timingTestCountPtr[ tCounter ] = 1;
            }
        }

        for( tCounter = 0; tCounter < timingTest; tCounter++ )
        {
            if( taskID == 0 )
            {
                printf( "Transfering %d integers. The time in seconds will be displayed below:\n", timingTestCountPtr[ tCounter ] );

                avgTime = 0.0;

                for( counter = 0; counter < numberOfTests; counter++ )
                {

                    sTime = GetCurrentMicroSecTime( ); //start timing

                    MPI_Send( &dataPtr[ 0 ], timingTestCountPtr[ tCounter ], MPI_INT, 1, 1, MPI_COMM_WORLD );

                    MPI_Recv( &dataPtr[ 0 ], timingTestCountPtr[ tCounter ], MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );               


                    eTime = GetCurrentMicroSecTime( ); //stop timing

                    fTime = ConvertTimeToSeconds( eTime - sTime ); //get time

                    avgTime += fTime;

                }

                printf( "%.9f\n", (avgTime  / ( double ) numberOfTests ) );           
            
            }
            else if( taskID == 1 )
            {
                for( counter = 0; counter < numberOfTests; counter++ )
                {
                    MPI_Send( &dataPtr[ 0 ], timingTestCountPtr[ tCounter ], MPI_INT, 1, 1, MPI_COMM_WORLD );

                    MPI_Recv( &dataPtr[ 0 ], timingTestCountPtr[ tCounter ], MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );           
                }
            
            }
        }        

        free( dataPtr );

        free( timingTestCountPtr );

        timingTestCountPtr = NULL;

        dataPtr = NULL;
    }

    MPI_Finalize( );

    return 0;
}

// free function implementation //////////////////////////////////

unsigned long long GetCurrentMicroSecTime( )
{
    unsigned long long retTime;

    struct timeval tVal;

    gettimeofday( &tVal, NULL );

    retTime = ( unsigned long long ) (tVal.tv_sec * 1000000L) +( unsigned long long ) tVal.tv_usec;

    return retTime;
}

double ConvertTimeToSeconds( unsigned long long usTime )
{
    return ( double ) usTime / 1000000.0;
}



