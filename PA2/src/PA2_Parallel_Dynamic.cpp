/**************************************************************

@file PA2_Parallel_Dynamic.cpp

@brief the code for the PA2 Dynamic Parallel program in cs415

@description Calculates a Mandelbrot in parallel

@author Andrew Frost

@version 1.00 ( 03/02/2017 )

@note None

***************************************************************/


// header files //////////////////////////////////////////////
#include <sys/time.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <string>
#include "PIMFuncs.h"
#include "Mandelbrot.h"
#include "mpi.h"

// pre-compiler directives ///////////////////////////////////

#define KILL_SWITCH -1

//free function prototypes ///////////////////////////////////

unsigned long long GetCurrentMicroSecTime( );

double ConvertTimeToSeconds( unsigned long long usTime );

// main /////////////////////////////////////////////////////
int main( int argc, char *argv[ ] )
{
    unsigned long long sTime, eTime;
    std::vector<unsigned char> image;
    std::vector<unsigned char> tmp;
    int row, col, index, tRow, taskCount;
    int width = 0, height = 0, currentRowToSend;
    std::stringstream strStream;
    std::string saveName;
    mb::ComplexNumber min, max, scale;

    int numberOfTasks, taskID;
    MPI_Status status;

    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &numberOfTasks );
    MPI_Comm_rank( MPI_COMM_WORLD, &taskID );

    //cmd line params
    if( argc < 4 )
    {
        std::cout << "The program must be ran with the following:" <<std::endl;
        std::cout << "srun [MPI PARAMS] PA2_Parallel_Dynamic [number of columns] [number of rows] [file path name]" <<std::endl;
        return -1;
    }

    //process the command line parameters
    strStream.str( argv[ 1 ] );
    strStream >> width;

    strStream.str("");
    strStream.clear();

    strStream.str( argv[ 2 ] );
    strStream >> height;

    if( width <= 0 || height <= 0 )
    {
        std::cout<< "Invalid image height or width specified."<<std::endl;
        std::cout<<"Width is "<<width<<std::endl;
        std::cout<<"Height is "<<height<<std::endl;
        return -1;
    }

    saveName = argv[ 3 ];

    //calculate min and max vals
    max.real = 2;
    max.imaginary = 2;
    min.real = -2;
    min.imaginary = -2;

    scale.real = (max.real - min.real) / static_cast<float>( width );
    scale.imaginary = (max.imaginary - min.imaginary) / static_cast<float>( height );

    //compute the image
    if( taskID == 0 )
    {
        //alloc image
        image.resize( width * height );
        taskCount = currentRowToSend = 0;

        sTime = GetCurrentMicroSecTime( );

        //initial sending of rows
        for( index = 0; index < std::min( numberOfTasks - 1, height ); index++ )
        {
            MPI_Send( &index, 1, MPI_INT, index + 1, 0, MPI_COMM_WORLD );
            taskCount++;
        }

        currentRowToSend = std::min( height, index );

        //poll for completed rows
        while( true )
        {
            //get the row and copy it
            MPI_Recv( &tRow, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status );
            MPI_Recv( &image[ tRow * width ], width, MPI_UNSIGNED_CHAR, status.MPI_SOURCE, 1, MPI_COMM_WORLD, &status );

            //decrement the number of tasks running
            taskCount--;

            //send next job if one is available
            if( currentRowToSend < height )
            {
                MPI_Send( &currentRowToSend, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD );

                currentRowToSend++;
                taskCount++;
            }
            else //kill unneeded task
            {
                index = KILL_SWITCH;
                MPI_Send( &index, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD );
            }

            //stop looping if there are no more jobs to be done
            if( taskCount <= 0 )
            {
                break;
            }
        }

        eTime = GetCurrentMicroSecTime( );

        //print information about the test
        std::cout<<"Parallel Dynamic\t"<<numberOfTasks<<"\t";
        std::cout<<width<<"x"<<height<<"\t"<<ConvertTimeToSeconds( eTime - sTime )<<std::endl;

        //save the image
        if(!pim_write_black_and_white(saveName.c_str(), width, height, &image[0]))
        {
            std::cout<<"Failure saving image."<<std::endl;
        }
    }
    else
    {
        //alloc image
        tmp.resize( width );

        while( true )
        {
            MPI_Recv( &row, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status );

            if( row == KILL_SWITCH )
            {
                //terminate loop and task
                break;
            }

            //calculate the row's pixel values
            for( col = 0; col < width; col++ )
            {
                tmp[ col ] = CalculatePixelAt( col, row, min, scale );
            }

            //send the completed row
            MPI_Send( &row, 1, MPI_INT, 0, 0, MPI_COMM_WORLD );
            MPI_Send( &tmp[ 0 ], width, MPI_UNSIGNED_CHAR, 0, 1, MPI_COMM_WORLD );
        }
    }

    MPI_Finalize( );


    return 0;
}

// free function implementation //////////////////////////////////

//get the time in microseconds. stores the result in an unsigned long long
unsigned long long GetCurrentMicroSecTime( )
{
    unsigned long long retTime;

    struct timeval tVal;

    gettimeofday( &tVal, NULL );

    retTime = ( unsigned long long ) (tVal.tv_sec * 1000000L) +( unsigned long long ) tVal.tv_usec;

    return retTime;
}

//gets the time in seconds. stores the result in a double
double ConvertTimeToSeconds( unsigned long long usTime )
{
    return ( double ) usTime / 1000000.0;
}

