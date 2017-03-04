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
    int row, col, index, messageAvailable = 0, tRow;
    int width = 0, height = 0, rowReceivedCount = 0, currentRowToSend;
    std::stringstream strStream;
    std::string saveName;
    mb::ComplexNumber min, max, scale;

    std::vector<bool> rowReceived;

    int numberOfTasks, taskID;
    int activeTasks;
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
        tmp.resize( width );
        rowReceived.resize( height, false );
        rowReceivedCount = currentRowToSend = 0;

        activeTasks = 0;

        sTime = GetCurrentMicroSecTime( );

        //initial sending of rows
        for( index = 1; index < std::min( numberOfTasks, height ); index++ )
        {
            MPI_Send( &index, 1, MPI_INT, index, 0, MPI_COMM_WORLD );
            activeTasks++;
        }

        currentRowToSend = std::min( height, index );

        row = 0;
        col = 0;

        //poll for completed rows
        while( true )
        {
            //check for row
            MPI_Iprobe( MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &messageAvailable, &status );

            if( messageAvailable )
            {
                //get the row and copy it
                MPI_Recv( &tRow, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status );
                MPI_Recv( &image[ tRow * width ], width, MPI_UNSIGNED_CHAR, status.MPI_SOURCE, 1, MPI_COMM_WORLD, &status );
                activeTasks--;

                if( !rowReceived[ tRow ] )
                {
                    rowReceived[ tRow ] = true;
                    rowReceivedCount++;
                }

                if( rowReceivedCount >= height  )
                {
                    break;
                }

                index = 0;

                //find the next row to send
                while( index < height )
                {
                    index++;

                    if( !rowReceived[ currentRowToSend ] )
                    {
                        break;
                    }

                    currentRowToSend = ( currentRowToSend + 1 ) % height;
                }

                //make sure task 0 isn't working on the row to send
                if( !rowReceived[ currentRowToSend ] )
                {
                    MPI_Send( &currentRowToSend, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD );
                    activeTasks++;
                }

            } //work on current row
            else if( !rowReceived[ row ] && col < width )
            {
                image[ row * width + col ] = static_cast<unsigned char> ( CalculatePixelAt( col, row, min, scale ) );
                col++;
            }
            else
            {
                if( !rowReceived[ row ] )
                {
                    rowReceived[ row ] = true;
                    rowReceivedCount++;
                }

                if( rowReceivedCount >= height )
                {
                    break;
                }

            }

        }

        eTime = GetCurrentMicroSecTime( );

        //finishing up
        std::cout<<"Parallel Dynamic"<<std::endl;
        std::cout<<"Image Dimensions\tTime(s)"<<std::endl;
        std::cout<<width<<"x"<<height<<"\t"<<ConvertTimeToSeconds( eTime - sTime )<<std::endl;

        if(!pim_write_black_and_white(saveName.c_str(), width, height, &image[0]))
        {
            std::cout<<"Failure saving image."<<std::endl;
        }

        currentRowToSend = KILL_SWITCH;

        // get slow nodes rows
        for( index = 0; index < activeTasks; index++ )
        {
             MPI_Recv( &tRow, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status );
             MPI_Recv( &tmp[0], width, MPI_UNSIGNED_CHAR, status.MPI_SOURCE, 1, MPI_COMM_WORLD, &status );
        }

        activeTasks = 0;

        // terminate remaining tasks
        for( index = 1; index < numberOfTasks; index++ )
        {
            MPI_Send( &currentRowToSend, 1, MPI_INT, index, 0, MPI_COMM_WORLD );
        }

        for( index = 1; index < numberOfTasks; index++ )
        {
            MPI_Recv( &row, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status );
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
                MPI_Send( &row, 1, MPI_INT, 0, 0, MPI_COMM_WORLD );

                break;
            }

            for( col = 0; col < width; col++ )
            {

                tmp[ col ] = CalculatePixelAt( col, row, min, scale );
            }

            MPI_Send( &row, 1, MPI_INT, 0, 0, MPI_COMM_WORLD );
            MPI_Send( &tmp[ 0 ], width, MPI_UNSIGNED_CHAR, 0, 1, MPI_COMM_WORLD );
        }
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

