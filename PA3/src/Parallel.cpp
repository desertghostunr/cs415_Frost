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
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include "mpi.h"
#include "Timer.h"
#include "tSort.h"

// pre-compiler directives /////////////////////////////////////
#define SAVE_FLAG 1
#define SAVE_CODE 101
#define SEND_DATA 201
#define KILL_SWITCH -100

// main /////////////////////////////////////////////////////
int main( int argc, char *argv[ ] )
{
    //vars
    unsigned long long sTime = 0ll, eTime = 0ll;
    double finalTime;
    std::vector< int > data;
    std::vector< std::vector< int > > buckets;
    int tmpInt, amntOfData, saveFlag = 0, maxBucketSize = 100000000;
    int minMax[2];
    int index, dIndex, rmndr;
    std::string fileName;
    std::fstream file;
    std::stringstream strStream;
    size_t extensionPos;

    int numberOfTasks, taskID;    

    MPI_Status status;    
    
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &numberOfTasks );
    MPI_Comm_rank( MPI_COMM_WORLD, &taskID );

    //cmd line params error checking
    if( argc < 2 )
    {
        std::cout << "The program must be ran with the following:" <<std::endl;
        std::cout << "srun n16 Parallel [file path name]" <<std::endl;
    }

    if( numberOfTasks < 2 )
    {
        std::cout<<"Not enough tasks to run."<<std::endl;
        MPI_Finalize( );
        return -1;
    }

    //master
    if( taskID == 0 )
    {
        fileName = argv[ 1 ];    

        //open the file
        file.open( fileName.c_str( ) );

        if( !file.is_open( ) ) //if error then end program
        {
            std::cout << "Error: unable to open " << fileName << "." << std::endl;

            for( index = 1; index < numberOfTasks; index++ )
            {
                amntOfData = KILL_SWITCH;

                MPI_Send( &amntOfData, 1, MPI_INT, index, SEND_DATA + 1, MPI_COMM_WORLD ); //send KILL_SWITCH due to error     
            }

            MPI_Barrier( MPI_COMM_WORLD );
            MPI_Finalize();
            return -1;
        }
    
        //read in data
        if( !(file >> tmpInt ) )
        {
            std::cout << "Warning: Corrupted file contents detected." << std::endl;
        }

        data.resize( tmpInt / numberOfTasks, 0 );

        //check if the data is divided even by the number of processes
        
        rmndr = amntOfData = tmpInt;
    
        minMax[ 1 ] = -1 * std::numeric_limits<int>::infinity( );
        minMax[ 0 ] = std::numeric_limits<int>::infinity( );

        //read in data and send it to the slaves
        for( index = 1; index < numberOfTasks; index++ )
        {
            for( dIndex = 0; dIndex < static_cast<int>( data.size( ) ); dIndex++ )
            {
                if( !(file >> tmpInt ) )
                {
                    std::cout << "Warning: Corrupted file contents detected." << std::endl;
                }

                if( tmpInt < minMax[ 0 ] )
                {
                    minMax[ 0 ] = tmpInt;
                }

                if( tmpInt > minMax[ 1 ] )
                {
                    minMax[ 1 ] = tmpInt;
                }

                data[ dIndex ] = tmpInt;

                rmndr--;
            }

            MPI_Send( &amntOfData, 1, MPI_INT, index, SEND_DATA + 1, MPI_COMM_WORLD ); //send amntOfData
            MPI_Send( &data[ 0 ], static_cast<int>( data.size( ) ), MPI_INT, index, SEND_DATA, MPI_COMM_WORLD ); //send data          
        }

        maxBucketSize = std::max( rmndr, amntOfData / numberOfTasks );        

        data.resize( rmndr ); //resize for any remainder

        //read in data for master
        for( index = 0; index < static_cast<int>( data.size( ) ); index++ )
        {
            if( !(file >> tmpInt ) )
            {
                std::cout << "Warning: Corrupted file contents detected." << std::endl;
            }

            if( tmpInt < minMax[ 0 ] )
            {
                minMax[ 0 ] = tmpInt;
            }

            if( tmpInt > minMax[ 1 ] )
            {
                minMax[ 1 ] = tmpInt;
            }

            data[ index ] = tmpInt;

            rmndr--;
        }
        
        file.close( );

        //send the min and the max to each slave
        for( index = 1; index < numberOfTasks; index++ )
        {
            MPI_Send( &minMax[ 0 ], 2, MPI_INT, index, SEND_DATA + 2, MPI_COMM_WORLD );
            MPI_Send( &maxBucketSize, 1, MPI_INT, index, SEND_DATA + 3, MPI_COMM_WORLD );
        }
    }
    else //slaves, get data for each slave
    {
        MPI_Recv( &amntOfData, 1, MPI_INT, 0, SEND_DATA + 1, MPI_COMM_WORLD, &status ); //get total amntOfData

        if( amntOfData == KILL_SWITCH )
        {
            MPI_Barrier( MPI_COMM_WORLD );
            MPI_Finalize();
            return -1;
        }

        data.resize( amntOfData / numberOfTasks, 0 ); //alloc for data
        MPI_Recv( &data[ 0 ], static_cast<int>( data.size( ) ), MPI_INT, 0, SEND_DATA, MPI_COMM_WORLD, &status ); //get data

        MPI_Recv( &minMax[ 0 ], 2, MPI_INT, 0, SEND_DATA + 2, MPI_COMM_WORLD, &status ); //get min and max
        MPI_Recv( &maxBucketSize, 1, MPI_INT, 0, SEND_DATA + 3, MPI_COMM_WORLD, &status ); //get max bucket size
    }

    //allocate the buckets for each processs
    //resize for the number of buckets
    buckets.resize( numberOfTasks );

    for( index = 0; index < numberOfTasks; index++ )
    {
        buckets[ index ].reserve( (amntOfData / numberOfTasks ) + 1 );
    }
    
    //wait for all processes to be ready
    MPI_Barrier( MPI_COMM_WORLD );    

    //sort the data
    if( taskID == 0 )
    {
        sTime = GetCurrentMicroSecTime( );
    }    

    tSort::pBucket( data, buckets, numberOfTasks, taskID, minMax[ 0 ], minMax[ 1 ], maxBucketSize );

    MPI_Barrier( MPI_COMM_WORLD );

    if( taskID == 0 )
    {
        eTime = GetCurrentMicroSecTime( );
        finalTime = ConvertTimeToSeconds( eTime - sTime );
    }


    //get whether or not to save ////////////////////////////////////////////
    if( argc > 2 )
    {
        strStream.str( std::string( "" ) );
        strStream.clear( );

        strStream.str( argv[ 2 ] );

        strStream >> saveFlag;
    }
    
    //write out data ///////////////////////////////////////////////////////
    if( saveFlag )
    {
        MPI_Barrier( MPI_COMM_WORLD );

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

            file << amntOfData << std::endl;

            for( index = 0; index < static_cast<int>( data.size( ) ); index++ )
            {
                file << data[ index ] << std::endl;
            }

            for( tmpInt = 1; tmpInt < numberOfTasks; tmpInt++ )
            {
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
            index = static_cast<int>( data.size( ) ); 
            MPI_Send( &index, 1, MPI_INT, 0, SAVE_CODE - 1, MPI_COMM_WORLD ); //send the size
            MPI_Send( &data[0], index, MPI_INT, 0, SAVE_CODE - 2, MPI_COMM_WORLD ); //send the data
        }
        
    }    

    //print the time ///////////////////////////////////////////////////////////////////

    if( taskID == 0 )
    {
        std::cout<<"Parallel\t"<<numberOfTasks<<"\t";
        std::cout<<amntOfData<<"\t"<<finalTime<<std::endl;
    }

   // finalize ///////////////////////////////////////////////////////////////////////// 
    
    MPI_Finalize( );

    return 0;
}

