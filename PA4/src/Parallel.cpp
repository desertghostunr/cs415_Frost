/**************************************************************

@file Parallel.cpp

@brief Parallel Matrix Multiplication

@description multiplies matrices using Cannon's Algorithm

@author 

@version 1.00 ( 04/11/2017 )

@note None

***************************************************************/


// header files //////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <fstream>
#include "mpi.h"
#include "Timer.h"
#include "tMath.h"
#include "tMatrix.h"

// pre-compiler directives /////////////////////////////////////
#define SAVE_FLAG 1
#define SAVE_CODE 101
#define SEND_DATA 201
#define KILL_SWITCH 999999999u
#define SEED 100102330
#define UPPER_BOUND 10

// main /////////////////////////////////////////////////////
int main( int argc, char *argv[ ] )
{
    //vars
    unsigned long long sTime = 0ll, eTime = 0ll;
    double finalTime;
    std::vector< int > rData, sData;
    int tmpInt, saveFlag = 0;
    size_t index, rIndex, tIndex;
    std::stringstream strStream;
    size_t numberOfValues, matrixDivider, matrixDim, row, col;

    int numberOfTasks, taskID, tmpID;    

    tMath::tMatrix< int > matA, matB, matC;

    std::string fileA, fileB;

    std::fstream fileStream;

    MPI_Status status;    
    
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &numberOfTasks );
    MPI_Comm_rank( MPI_COMM_WORLD, &taskID );

    //cmd line params
    if( argc < 2 )
    {
        std::cout << "The program must be ran with the following:" << std::endl;
        std::cout << "srun n16 Parallel [matrix size]" << std::endl;
        std::cout << "OR as" << std::endl;
        std::cout << "srun n16 Parallel [matrix A file] [matrix B file]" << std::endl;
        return -1;
    }

    if( numberOfTasks < 2 )
    {
        std::cout << "Error: Not enough tasks to run." << std::endl;
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

    strStream.str( argv[ 1 ] );

    //process the data
    if( !( strStream >> numberOfValues ) )
    {
        MPI_Barrier( MPI_COMM_WORLD );

        strStream.str( std::string( "" ) );
        strStream.clear( );

        fileA = argv[ 1 ];

        if( argc < 2 )
        {
            std::cout << "Error: Not enough parameters. Only one filename given." << std::endl;
            return -1;
        }

        fileB = argv[ 2 ];

        //get whether or not to print data
        saveFlag = -1;

        if( argc > 3 )
        {
            strStream.str( std::string( "" ) );
            strStream.clear( );

            strStream.str( argv[ 3 ] );

            strStream >> saveFlag;
        }

        

        if( taskID == 0 )
        {
            fileStream.open( fileA.c_str( ) );

            if( !fileStream.is_open( ) )
            {
                std::cout << "Error: Could not open " << fileA << std::endl;
                for( index = 1; index < static_cast<size_t> ( numberOfTasks ); index++ )
                {
                    rIndex = KILL_SWITCH;
                    MPI_Send( &rIndex, 1, MPI_INT, index, SEND_DATA + 1, MPI_COMM_WORLD );
                }
                MPI_Finalize( );
                return -1;
            }

            fileStream >> numberOfValues;

            for( tmpInt = 1; tmpInt < numberOfTasks; tmpInt++ )
            {
                MPI_Send( &numberOfValues, 1, MPI_INT, tmpInt, SEND_DATA + 1, MPI_COMM_WORLD );
            }
            if( numberOfValues % matrixDivider != 0 ) //check for uneven division of matrix dimensions
            {
                std::cout << "Error: uneven dimension division from node " << taskID << std::endl;
                fileStream.close( );
                MPI_Finalize( );
                return -1;
            }
            //fill matrices
            matrixDim = numberOfValues / matrixDivider;

            matA.resize( matrixDim, matrixDim );

            rData.resize( matrixDim );

            //send out data
            for( index = 0; index < numberOfValues; index++ )
            {
                row = index % matrixDim;
                for( tmpInt = 0; tmpInt < static_cast<int> ( matrixDivider ); tmpInt++ )
                {
                    tmpID = tmpInt + ( matrixDivider * ( index / matrixDim ) );

                    if( tmpID != 0 )
                    {
                        MPI_Send( &row, 1, MPI_INT, tmpID, SEND_DATA + 1, MPI_COMM_WORLD );

                        for( rIndex = 0; rIndex < rData.size( ); rIndex++ )
                        {
                            fileStream >> rData[ rIndex ];
                        }

                        MPI_Send( &rData[ 0 ], rData.size( ), MPI_INT, tmpID, SEND_DATA + 1, MPI_COMM_WORLD );

                    }
                    else
                    {
                        for( rIndex = 0; rIndex < rData.size( ); rIndex++ )
                        {
                            fileStream >> matA( row, rIndex );
                        }
                    }

                    
                }                

            }

            fileStream.close( );

            fileStream.clear( );

            for( index = 1; index < static_cast<size_t> ( numberOfTasks ); index++ )
            {
                rIndex = KILL_SWITCH;
                MPI_Send( &rIndex, 1, MPI_INT, index, SEND_DATA + 1, MPI_COMM_WORLD );
            }

            fileStream.open( fileB.c_str( ) );

            if( !fileStream.is_open( ) )
            {
                std::cout << "Error: Could not open " << fileB << std::endl;
                for( index = 1; index < static_cast<size_t> ( numberOfTasks ); index++ )
                {
                    rIndex = KILL_SWITCH;
                    MPI_Send( &rIndex, 1, MPI_INT, index, SEND_DATA + 2, MPI_COMM_WORLD );
                }
                MPI_Finalize( );
                return -1;
            }

            fileStream >> numberOfValues;

            for( tmpInt = 1; tmpInt < numberOfTasks; tmpInt++ )
            {
                MPI_Send( &numberOfValues, 1, MPI_INT, tmpInt, SEND_DATA + 2, MPI_COMM_WORLD );
            }

            if( numberOfValues % matrixDivider != 0 ) //check for uneven division of matrix dimensions
            {
                std::cout << "Error: uneven dimension division from node " << taskID << std::endl;
                fileStream.close( );
                
                MPI_Finalize( );
                return -1;
            }

            

            //fill matrices
            matrixDim = numberOfValues / matrixDivider;

            matB.resize( matrixDim, matrixDim );

            rData.resize( matrixDim );

            //send out data
            for( index = 0; index < numberOfValues; index++ )
            {
                row = index % matrixDim;
                for( tmpInt = 0; tmpInt < static_cast<int> ( matrixDivider ); tmpInt++ )
                {
                    tmpID = tmpInt + ( matrixDivider * ( index / matrixDim ) );

                    if( tmpID != 0 )
                    {
                        MPI_Send( &row, 1, MPI_INT, tmpID, SEND_DATA + 2, MPI_COMM_WORLD );

                        for( rIndex = 0; rIndex < rData.size( ); rIndex++ )
                        {
                            fileStream >> rData[ rIndex ];
                        }

                        MPI_Send( &rData[ 0 ], rData.size( ), MPI_INT, tmpID, SEND_DATA + 2, MPI_COMM_WORLD );

                    }
                    else
                    {
                        for( rIndex = 0; rIndex < rData.size( ); rIndex++ )
                        {
                            fileStream >> matB( row, rIndex );
                        }
                    }
                }

            }

            fileStream.close( );

            fileStream.clear( );

            for( index = 1; index < static_cast<size_t> ( numberOfTasks ); index++ )
            {
                rIndex = KILL_SWITCH;
                MPI_Send( &rIndex, 1, MPI_INT, index, SEND_DATA + 2, MPI_COMM_WORLD );
            }
        }
        else
        {
            //A
            MPI_Recv( &tmpInt, 1, MPI_INT, 0, SEND_DATA + 1, MPI_COMM_WORLD, &status );

            numberOfValues = static_cast< size_t > ( tmpInt );

            if( numberOfValues >= KILL_SWITCH )
            {
                MPI_Finalize( );
            }

            if( numberOfValues % matrixDivider != 0 ) //check for uneven division of matrix dimensions
            {
                std::cout << "Error: uneven dimension division from node " << taskID << std::endl;
                MPI_Finalize( );
                return -1;
            }

            //fill matrices
            matrixDim = numberOfValues / matrixDivider;

            matA.resize( matrixDim, matrixDim );

            rData.resize( matrixDim );


            while( true )
            {
                MPI_Recv( &tmpInt, 1, MPI_INT, 0, SEND_DATA + 1, MPI_COMM_WORLD, &status );
                rIndex = static_cast< size_t > ( tmpInt );
                if( rIndex >= KILL_SWITCH )
                {
                    break;
                }

                row = rIndex;

                MPI_Recv( &rData[ 0 ], rData.size( ), MPI_INT, 0, SEND_DATA + 1, MPI_COMM_WORLD, &status );
                
                for( rIndex = 0; rIndex < rData.size( ); rIndex++ )
                {
                    matA( row, rIndex ) = rData[ rIndex ];
                }

            }


            //B
            MPI_Recv( &tmpInt, 1, MPI_INT, 0, SEND_DATA + 2, MPI_COMM_WORLD, &status );

            numberOfValues = static_cast< size_t > ( tmpInt );

            if( numberOfValues >= KILL_SWITCH )
            {
                MPI_Finalize( );
            }

            if( numberOfValues % matrixDivider != 0 ) //check for uneven division of matrix dimensions
            {
                std::cout << "Error: uneven dimension division from node " << taskID << std::endl;
                MPI_Finalize( );
                return -1;
            }

            //fill matrices
            matrixDim = numberOfValues / matrixDivider;

            matB.resize( matrixDim, matrixDim );

            rData.resize( matrixDim );


            while( true )
            {
                MPI_Recv( &tmpInt, 1, MPI_INT, 0, SEND_DATA + 2, MPI_COMM_WORLD, &status );
                rIndex = static_cast< size_t > ( tmpInt );
                if( rIndex >= KILL_SWITCH )
                {
                    break;
                }

                row = rIndex;

                MPI_Recv( &rData[ 0 ], rData.size( ), MPI_INT, 0, SEND_DATA + 2, MPI_COMM_WORLD, &status );

                for( rIndex = 0; rIndex < rData.size( ); rIndex++ )
                {
                    matB( row, rIndex ) = rData[ rIndex ];
                }

            }
        }
        
        

    }
    else
    {
        //get whether or not to print data
        saveFlag = -1;

        if( argc > 2 )
        {
            strStream.str( std::string( "" ) );
            strStream.clear( );

            strStream.str( argv[ 2 ] );

            strStream >> saveFlag;
        }

        if( numberOfValues % matrixDivider != 0 ) //check for uneven division of matrix dimensions
        {
            std::cout << "Error: uneven dimension division from node " << taskID << std::endl;

            MPI_Finalize( );
            return -1;
        }

        matrixDim = numberOfValues / matrixDivider;

        //generate matrices
        matA.resize( matrixDim, matrixDim );       

        tMath::MakeMatrix( matA, matrixDim * ( static_cast< size_t > ( taskID ) / matrixDivider ), matrixDim * ( static_cast< size_t > ( taskID ) % matrixDivider ) );

               

        matB = matA;
    }

    //zero out c
    matC.resize( matrixDim, matrixDim );
    tMath::ZeroMatrix( matC );

    
    //wait for all processes to be ready
    MPI_Barrier( MPI_COMM_WORLD );

    //grab the time
    if( taskID == 0 )
    {
        sTime = GetCurrentMicroSecTime( );
    }    

    //multiply the matrices using cannon's algorithm
    //initialization
    /*matA.copyToVector( sData );
    MPI_Send( &sData[ 0 ], sData.size( ), MPI_INT, 

    matB.copyToVector( sData );


    //repeated multiplication*/
    
    
    //make sure everyone is done
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
            for( index = 0; index < numberOfValues; index++ )
            {
                row = index % matrixDim;
                for( tmpInt = 0; tmpInt < static_cast<int> ( matrixDivider ); tmpInt++ )
                {
                    tmpID = tmpInt + ( matrixDivider * ( index / matrixDim ) );

                    if( tmpID != 0 )
                    {
                        MPI_Send( &row, 1, MPI_INT, tmpID, SAVE_CODE + 1, MPI_COMM_WORLD );
                        MPI_Recv( &rData[ 0 ], rData.size( ), MPI_INT, tmpID, SAVE_CODE + 1, MPI_COMM_WORLD, &status );
                        
                    }
                    else
                    {
                        matA.getRow( index, rData );
                    }

                    for( rIndex = 0; rIndex < rData.size( ); rIndex++ )
                    {
                        std::cout << rData[ rIndex ] << "\t";
                    }
                }

                std::cout << std::endl;
                
            }

            std::cout << std::endl;

            for( index = 1; index < static_cast<size_t> ( numberOfTasks ); index++ )
            {
                rIndex = KILL_SWITCH;
                MPI_Send( &rIndex, 1, MPI_INT, index, SAVE_CODE + 1, MPI_COMM_WORLD );
            }

            std::cout << "Mat B:" << std::endl;            

            //write out matrix
            for( index = 0; index < numberOfValues; index++ )
            {
                
                row = index % matrixDim;
                for( tmpInt = 0; tmpInt < static_cast<int> ( matrixDivider ); tmpInt++ )
                {
                    tmpID = tmpInt + ( matrixDivider * (index / matrixDim ) );

                    if( tmpID != 0 )
                    {
                        MPI_Send( &row, 1, MPI_INT, tmpID, SAVE_CODE + 2, MPI_COMM_WORLD );
                        MPI_Recv( &rData[ 0 ], rData.size( ), MPI_INT, tmpID, SAVE_CODE + 2, MPI_COMM_WORLD, &status );

                        
                    }
                    else
                    {
                        matB.getRow( index, rData );
                    }

                    for( rIndex = 0; rIndex < rData.size( ); rIndex++ )
                    {
                        std::cout << rData[ rIndex ] << "\t";
                    }
                }

                std::cout << std::endl;
                
            }

            std::cout << std::endl;

            for( index = 1; index < static_cast<size_t> ( numberOfTasks ); index++ )
            {
                rIndex = KILL_SWITCH;
                MPI_Send( &rIndex, 1, MPI_INT, index, SAVE_CODE + 2, MPI_COMM_WORLD );
            }


            std::cout << "Mat C:" << std::endl;            

            //write out matrix
            for( index = 0; index < numberOfValues; index++ )
            {
                row = index % matrixDim;
                for( tmpInt = 0; tmpInt < static_cast<int> ( matrixDivider ); tmpInt++ )
                {
                    tmpID = tmpInt + ( matrixDivider * ( index / matrixDim ) );

                    if( tmpID != 0 )
                    {
                        MPI_Send( &row, 1, MPI_INT, tmpID, SAVE_CODE + 3, MPI_COMM_WORLD );
                        MPI_Recv( &rData[ 0 ], rData.size( ), MPI_INT, tmpID, SAVE_CODE + 3, MPI_COMM_WORLD, &status );                        
                    }
                    else
                    {
                        matC.getRow( index, rData ); //write out master
                    }
                    for( rIndex = 0; rIndex < rData.size( ); rIndex++ )
                    {
                        std::cout << rData[ rIndex ] << "\t";
                    }
                }

                std::cout << std::endl;
                
            }

            std::cout << std::endl;

            for( index = 1; index < static_cast<size_t> ( numberOfTasks ); index++ )
            {
                rIndex = KILL_SWITCH;
                MPI_Send( &rIndex, 1, MPI_INT, index, SAVE_CODE + 3, MPI_COMM_WORLD );
            }
        } 
        else
        {
            //A
            while( true )
            {
                MPI_Recv( &tmpInt, 1, MPI_INT, 0, SAVE_CODE + 1, MPI_COMM_WORLD, &status );
                rIndex = static_cast< size_t > ( tmpInt );
                if( rIndex >= KILL_SWITCH )
                {
                    break;
                }
                matA.getRow( rIndex, rData );
                MPI_Send( &rData[ 0 ], rData.size( ), MPI_INT, 0, SAVE_CODE + 1, MPI_COMM_WORLD );
            }

            //B
            while( true )
            {
                MPI_Recv( &tmpInt, 1, MPI_INT, 0, SAVE_CODE + 2, MPI_COMM_WORLD, &status );
                rIndex = static_cast< size_t > ( tmpInt );
                if( rIndex >= KILL_SWITCH )
                {
                    break;
                }
                matB.getRow( rIndex, rData );
                MPI_Send( &rData[ 0 ], rData.size( ), MPI_INT, 0, SAVE_CODE + 2, MPI_COMM_WORLD );
            }

            //C
            while( true )
            {
                MPI_Recv( &tmpInt, 1, MPI_INT, 0, SAVE_CODE + 3, MPI_COMM_WORLD, &status );
                rIndex = static_cast< size_t > ( tmpInt );
                
                if( rIndex >= KILL_SWITCH )
                {
                    break;
                }
                matC.getRow( rIndex, rData );
                MPI_Send( &rData[ 0 ], rData.size( ), MPI_INT, 0, SAVE_CODE + 3, MPI_COMM_WORLD );
            }
        }
        
    }    

    //print the time ///////////////////////////////////////////////////////////////////

    if( taskID == 0 )
    {
        std::cout<<"Parallel\t"<<numberOfTasks<<"\t";
        std::cout<<numberOfValues<<"\t"<<finalTime<<std::endl;
    }

   // finalize /////////////////////////////////////////////////////////////////////////
    MPI_Finalize( );

    return 0;
}

