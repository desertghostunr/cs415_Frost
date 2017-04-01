/**************************************************************

@file Parallel.cpp

@brief the code for the parallel bucket sorting program for PA3

@description Runs bucket sort in parallel on a set of numbers read from file

@author Andrew Frost

@version 1.00 ( 04/01/2017 )

@note None

***************************************************************/


// header files //////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <cmath>
#include "mpi.h"
#include "Timer.h"
#include "tSort.h"

// pre-compiler directives /////////////////////////////////////
#define SAVE_FLAG 1
#define SAVE_CODE -1
#define SEND_TIME_CODE -101

// main /////////////////////////////////////////////////////
int main( int argc, char *argv[ ] )
{
    //vars
    unsigned long long sTime, eTime;
    double tTime, finalTime;
    std::vector< int > data;
    int min, max, tmpInt, saveFlag = 0;
    int index;
    std::string fileName;
    std::stringstream strStream;
    std::fstream file;
    size_t extensionPos;

    int numberOfTasks, taskID;

    MPI_Status status;

    //cmd line params error checking
    if( argc < 2 )
    {
        std::cout << "The program must be ran with the following:" <<std::endl;
        std::cout << "srun n16 Parallel [file path name]" <<std::endl;
        return -1;
    }
    
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &numberOfTasks );
    MPI_Comm_rank( MPI_COMM_WORLD, &taskID );

    if( numberOfTasks < 2 )
    {
        std::cout<<"Not enough tasks to run."<<std::endl;
        return -1;
    }

    //master
    if( taskID == 0 )
    {
        fileName = argv[ 1 ];    

        //open the file
        file.open( fileName.c_str( ) );

        if( !file.is_open( ) )
        {
            std::cout << "Error: unable to open " << fileName << "." << std::endl;
            return -1;
        }
    
        //read in data
        strStream.str( std::string( std::istreambuf_iterator<char>( file ),
                                std::istreambuf_iterator<char>( ) ) );
        file.close( );

        //process the data
        if( !( strStream >> tmpInt ) )
        {
            std::cout << "Error: Invalid data in file!" << std::endl;
            return -1;
        }

        data.resize( tmpInt );
    
        max = -1 * std::numeric_limits<int>::infinity( );
        min = std::numeric_limits<int>::infinity( );

        for( index = 0; index < static_cast<int>( data.size( ) ); index++ )
        {
            if( !( strStream >> tmpInt ) )
            {
                std::cout << "Error: Invalid data in file!" << std::endl;
            }

            if( tmpInt < min )
            {
                min = tmpInt;
            }

            if( tmpInt > max )
            {
                max = tmpInt;
            }

            data[ index ] = tmpInt;
        }
    }
    else //slaves
    {

    }

    

    

    

    sTime = GetCurrentMicroSecTime( );

    //sort
    tSort::sBucket( data, numberOfTasks, min, max );

    eTime = GetCurrentMicroSecTime();

    //get the sorting time from each task
    if( taskID == 0 )
    {
        finalTime = ConvertTimeToSeconds( eTime - sTime );

        for( index = 1; index < numberOfTasks; index++ )
        {
            tmpInt = SEND_TIME_CODE;
            MPI_Send( &tmpInt, 1, MPI_INT, index, SEND_TIME_CODE, MPI_COMM_WORLD ); //ask for timing data
            MPI_Recv( &tTime, 1, MPI_DOUBLE, index, SEND_TIME_CODE - 1, MPI_COMM_WORLD, &status ); //get the timing data

            finalTime += tTime;
        }

    }
    else
    {
        tTime = ConvertTimeToSeconds( eTime - sTime );
        MPI_Recv( &tmpInt, 1, MPI_INT, 0, SEND_TIME_CODE, MPI_COMM_WORLD, &status ); //request for timing data
        MPI_Send( &tTime, 1, MPI_DOUBLE, 0, SEND_TIME_CODE - 1, MPI_COMM_WORLD ); //send the timing data
    }


    //get whether or not to save
    if( argc > 2 )
    {
        strStream.str( std::string( "" ) );
        strStream.clear( );

        strStream.str( argv[ 2 ] );

        strStream >> saveFlag;
    }
    
    //write out data
    if( saveFlag )
    {
        if( taskID == 0 )
        {
            file.clear( );

            extensionPos = fileName.find_last_of( "." );

            if( extensionPos == std::string::npos )
            {
                fileName += ".sorted";
            }
            else
            {
                fileName.insert( extensionPos, ".sorted" );
            }

            file.open( fileName.c_str( ), std::fstream::out );

            if( !file.is_open( ) )
            {
                std::cout << "Error: unable to save the sorted data." << std::endl;
            }

            file << static_cast<int>( data.size( ) ) << std::endl;

            for( index = 0; index < static_cast<int>( data.size( ) ); index++ )
            {
                file << data[ index ] << std::endl;
            }

            for( tmpInt = 1; tmpInt < numberOfTasks; tmpInt++ )
            {
                index = SAVE_CODE;
                MPI_Send( &index, 1, MPI_INT, tmpInt, SAVE_CODE, MPI_COMM_WORLD ); //ask for data
                MPI_Recv( &index, 1, MPI_INT, tmpInt, SAVE_CODE - 1, MPI_COMM_WORLD, &status ); //get number of pieces of data
                data.resize( index ); //resize for data
                MPI_Recv( &data[ 0 ], index, MPI_INT, tmpInt, SAVE_CODE - 2, MPI_COMM_WORLD, &status ); //receive data

                //write out data
                for( index = 0; index < static_cast<int>( data.size( ) ); index++ )
                {
                    file << data[ index ] << std::endl;
                }

            }

            file.close( );
        } 
        else
        {
            MPI_Recv( &tmpInt, 1, MPI_INT, 0, SAVE_CODE, MPI_COMM_WORLD, &status ); //wait for order to save
            index = static_cast<int>( data.size( ) ); 
            MPI_Send( &index, 1, MPI_INT, 0, SAVE_CODE - 1, MPI_COMM_WORLD ); //send the size
            MPI_Send( &data[0], index, MPI_INT, 0, SAVE_CODE - 2, MPI_COMM_WORLD ); //send the data
        }
        
    }    

    //print the time

    if( taskID == 0 )
    {
        std::cout<<"Parallel\t"<<numberOfTasks<<"\t";
        std::cout<<data.size()<<"\t"<<finalTime<<std::endl;
    }
    
    MPI_Finalize();

    return 0;
}

