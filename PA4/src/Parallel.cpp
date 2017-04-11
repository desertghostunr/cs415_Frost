/**************************************************************

@file Parallel.cpp

@brief Parallel Matrix Multiplication

@description multiplies matrices using Canon's Algorithm

@author Andrew Frost

@version 1.00 ( 04/11/2017 )

@note None

***************************************************************/


// header files //////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include "mpi.h"
#include "Timer.h"
#include "tMath.h"
#include "tMatrix.h"

// pre-compiler directives /////////////////////////////////////
#define SAVE_FLAG 1
#define SAVE_CODE 101
#define SEND_DATA 201
#define KILL_SWITCH -100
#define SEED 100102330
#define UPPER_BOUND 10

// main /////////////////////////////////////////////////////
int main( int argc, char *argv[ ] )
{
    //vars
    unsigned long long sTime = 0ll, eTime = 0ll;
    double finalTime;
    std::vector< int > rData, cData;
    int tmpInt, saveFlag = 0;
    size_t index, rIndex, tIndex;
    std::stringstream strStream;
    size_t numberOfValues, matrixDivider, matrixDim;

    int numberOfTasks, taskID;    

    tMath::tMatrix< int > matA, matB, matC;

    MPI_Status status;    
    
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &numberOfTasks );
    MPI_Comm_rank( MPI_COMM_WORLD, &taskID );

    //cmd line params error checking
    if( argc < 2 )
    {
        std::cout << "Error: The program must be ran with the following:" <<std::endl;
        std::cout << "srun n16 Parallel [matrix dimension: integer] [save flag: 1 to save]" <<std::endl;
    }

    if( numberOfTasks < 2 )
    {
        std::cout<<"Error: Not enough tasks to run."<<std::endl;
        MPI_Finalize( );
        return -1;
    }

    matrixDivider = static_cast< size_t >( std::sqrt( numberOfTasks ) );

    if( matrixDivider == 0 )
    {
        std::cout << "Error: Invalid number of tasks: " << numberOfTasks << "." << std::endl;
        std::cout << "The number of tasks must not have a zero (when truncated) square root." << std::endl;
        MPI_Finalize( );
        return -1;
    }

    if( ( matrixDivider * matrixDivider ) != static_cast< size_t >( numberOfTasks ) ) //square cores only
    {
        std::cout << "Error: Invalid number of tasks: " << numberOfTasks << "." << std::endl;
        std::cout << "The number of tasks must have an integer square root." << std::endl;
        MPI_Finalize( );
        return -1;
    }

    //get number of values for each dimension of the matrix
    strStream.str( std::string( "" ) );
    strStream.clear( );
    strStream.str( argv[ 1 ] );

    if( !( strStream >> numberOfValues ) ) //if error then end program
    {
        std::cout << "Error: invalid dimension from node " << taskID << std::endl;

        MPI_Finalize( );
        return -1;
    }

    if( numberOfValues % matrixDivider != 0 ) //check for uneven division of matrix dimensions
    {
        std::cout << "Error: uneven dimension division from node " << taskID << std::endl;

        MPI_Finalize( );
        return -1;
    }

    //get whether or not to save ////////////////////////////////////////////
    if( argc > 2 )
    {
        strStream.str( std::string( "" ) );
        strStream.clear( );

        strStream.str( argv[ 2 ] );

        strStream >> saveFlag;
    }

    //allocate memory
    matrixDim = numberOfValues / matrixDivider;
    
    matA.resize( matrixDim, matrixDim );

    tMath::MakeMatrix( matA, matrixDim * ( taskID / matrixDivider ), matrixDim * ( taskID % matrixDivider ) );

    matB = matA;

    matC.resize( matrixDim, matrixDim );

    tMath::ZeroMatrix( matC );

    
    //wait for all processes to be ready
    MPI_Barrier( MPI_COMM_WORLD );

    //grab the time
    if( taskID == 0 )
    {
        sTime = GetCurrentMicroSecTime( );
    }    

    //multiply the matrices using canon's algorithm
    

    MPI_Barrier( MPI_COMM_WORLD );

    if( taskID == 0 )
    {
        eTime = GetCurrentMicroSecTime( );
        finalTime = ConvertTimeToSeconds( eTime - sTime );
    }    
    
    //write out data ///////////////////////////////////////////////////////
    if( saveFlag == SAVE_FLAG )
    {
        MPI_Barrier( MPI_COMM_WORLD );

        if( taskID == 0 )
        {
            std::cout << "Mat A:" << std::endl;            

            //write out matrix
            for( index = 0; index < matrixDim; index++ )
            {
                matA.getRow( index, rData ); //write out master
                for( rIndex = 0; rIndex < rData.size( ); rIndex++ )
                {
                    std::cout << rData[ rIndex ] << "\t";
                }

                
            }

            for( tmpInt = 1; tmpInt < numberOfTasks; tmpInt++ )
            {
                MPI_Recv( &index, 1, MPI_INT, tmpInt, SAVE_CODE - 1, MPI_COMM_WORLD, &status ); //get number of pieces of data
                data.resize( index ); //resize for data
                MPI_Recv( &data[ 0 ], index, MPI_INT, tmpInt, SAVE_CODE - 2, MPI_COMM_WORLD, &status ); //receive data

                //write out data
                for( index = 0; index < static_cast<int>( data.size( ) ); index++ )
                {
                    std::cout << data[ index ] << std::endl;
                }

            }

            std::cout << std::endl;
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

