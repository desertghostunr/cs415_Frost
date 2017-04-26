/**************************************************************

@file Parallel.cpp

@brief Parallel Matrix Multiplication

@description multiplies matrices using Cannon's Algorithm

@author

@version 1.00 ( 04/11/2017 )

@version 1.01 ( 04/25/2017 ) revised code based on critique comments

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

// free function prototypes /////////////////////////////////
void ShiftLeft( int taskID, int shiftAmnt, size_t matrixDivider, tMath::tMatrix< int > & mat );

void ShiftUp( int taskID, int shiftAmnt, size_t matrixDivider, tMath::tMatrix< int > & mat );

bool InitializeWithFiles( int taskID, int numberOfTasks, std::string & fileA, std::string &fileB, 
                          size_t & numberOfValues, size_t & matrixDim, size_t & matrixDivider, 
                          std::vector< int > & rData, tMath::tMatrix< int > & matA, tMath::tMatrix< int > & matB );

bool MasterOpenFile( std::fstream & fileStream, int numberOfTasks, std::string & fileName );

void MasterPopulateMats( std::vector< int > & rData, size_t numberOfValues, 
                         size_t matrixDim, size_t matrixDivider, int numberOfTasks,
                         std::fstream & fileStream, tMath::tMatrix< int > & mat, int syncOffset );

void SlaveRecvMat( std::vector< int > & rData, tMath::tMatrix< int > & mat, int syncOffset );

void PrintFromMaster( std::vector< int > & rData, size_t numberOfValues, size_t matrixDim,
                      size_t matrixDivider, int numberOfTasks, tMath::tMatrix< int > & mat, int syncOffset );

void SendSlaveToMaster( std::vector< int > & rData, tMath::tMatrix< int > & mat, int syncOffset );

// main /////////////////////////////////////////////////////
int main( int argc, char *argv[ ] )
{
    //vars
    unsigned long long sTime = 0ll, eTime = 0ll;
    double finalTime;
    std::vector< int > rData;
    int saveFlag = 0, shiftAmnt;
    size_t index;
    std::stringstream strStream;
    size_t numberOfValues, matrixDivider, matrixDim;

    int numberOfTasks, taskID;

    tMath::tMatrix< int > matA, matB, matC;

    std::string fileA, fileB;

    // INITIALIZATION //////////////////////////////////////////

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

    //process the command line arguments to the program
    if( !( strStream >> numberOfValues ) ) //if not number, then file name
    {
        strStream.str( std::string( "" ) ); //clear string
        strStream.clear( ); //clear error flags

        MPI_Barrier( MPI_COMM_WORLD );
        
        //test for two file names
        if( argc < 2 )
        {
            std::cout << "Error: Not enough parameters. Only one filename given." << std::endl;
            return -1;
        }

        //get file names
        fileA = argv[ 1 ];
        fileB = argv[ 2 ];

        //initialize using files
        if( !InitializeWithFiles( taskID, numberOfTasks, fileA, fileB, numberOfValues,
                                  matrixDim, matrixDivider, rData, matA, matB ) )
        {
            return -1;
        }

        //get whether or not to print data
        saveFlag = -1;

        if( argc > 3 )
        {
            strStream.str( std::string( "" ) );
            strStream.clear( );

            strStream.str( argv[ 3 ] );
            strStream >> saveFlag;
        }

    }
    else //generate data in place based on number
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

        //check for uneven division of matrix dimensions
        if( numberOfValues % matrixDivider != 0 ) 
        {
            std::cout << "Error: uneven dimension division from node " << taskID << std::endl;
            MPI_Finalize( );
            return -1;
        }

        matrixDim = numberOfValues / matrixDivider;

        //generate matrices
        matA.resize( matrixDim, matrixDim );

        tMath::MakeMatrix( matA, 
                           matrixDim * ( static_cast< size_t > ( taskID ) / matrixDivider ), 
                           matrixDim * ( static_cast< size_t > ( taskID ) % matrixDivider ) );

        matB = matA;
    }

    //zero out c
    matC.resize( matrixDim, matrixDim );
    tMath::ZeroMatrix( matC );

    //write out data ///////////////////////////////////////////////////////
    if( saveFlag == SAVE_FLAG )
    {
        MPI_Barrier( MPI_COMM_WORLD );

        if( taskID == 0 )
        {
            std::cout << "Mat A:" << std::endl;
            //write out matrix
            PrintFromMaster( rData, numberOfValues, matrixDim, matrixDivider, numberOfTasks, matA, 1 );

            std::cout << "Mat B:" << std::endl;
            //write out matrix
            PrintFromMaster( rData, numberOfValues, matrixDim, matrixDivider, numberOfTasks, matB, 2 );            
        }
        else
        {
            SendSlaveToMaster( rData, matA, 1 );
            SendSlaveToMaster( rData, matB, 2 );
        }

    }

    rData.clear( );

    // MATRIX MULTIPLICATION ///////////////////////////////////////////////////////////////////////

    //wait for all processes to be ready
    MPI_Barrier( MPI_COMM_WORLD );

    //get the time at start
    if( taskID == 0 )
    {
        sTime = GetCurrentMicroSecTime( );
    }

    //multiply the matrices using cannon's algorithm

    //initialization //////////////

    //rows
    //calculate shift amount
    shiftAmnt = static_cast< int >( static_cast< size_t > ( taskID ) / matrixDivider );
    ShiftLeft( taskID, shiftAmnt, matrixDivider, matA );
    
    //columns
    //calculate shift amount
    shiftAmnt = static_cast< int >( static_cast< size_t > ( taskID ) % matrixDivider );
    ShiftUp( taskID, shiftAmnt, matrixDivider, matB );
    

    MPI_Barrier( MPI_COMM_WORLD ); //wait for all exchanges to finish

    //repeated multiplication //////////////////
    for( index = 0; index < matrixDivider; index++ )
    {
        //multiply matrices
        tMath::MultiplyMatrices( matA, matB, matC );

        //check if shift is necessary
        if( index < matrixDivider - 1 )
        {
            ShiftLeft( taskID, 1, matrixDivider, matA );
            ShiftUp( taskID, 1, matrixDivider, matB );
        }

    }
    
    //make sure everyone is done
    MPI_Barrier( MPI_COMM_WORLD );

    //get time
    if( taskID == 0 ) 
    {
        eTime = GetCurrentMicroSecTime( );//get the time at completion
        finalTime = ConvertTimeToSeconds( eTime - sTime ); //convert to run time in seconds
    }    

    // FINISHING UP ///////////////////////////////////////////////////////////

    //write out data ///////////////////////////////////////////////////////
    if( saveFlag == SAVE_FLAG )
    {
        rData.resize( matrixDim );

        MPI_Barrier( MPI_COMM_WORLD );

        if( taskID == 0 )
        {
            std::cout << "Mat C:" << std::endl;
            //write out matrix
            PrintFromMaster( rData, numberOfValues, matrixDim, matrixDivider, numberOfTasks, matC, 3 );
        } 
        else
        {
            SendSlaveToMaster( rData, matC, 3 );
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

// free function implementation ////////////////////////////////////////////////////////

/*******************************

@brief: ShiftLeft

@details: shifts a matrix left

@param: taskID: the task ID

@param: shiftAmnt: the amount to shift the row over

@param: matrixDivider: the number that the input matrix is divided by (equal to the sqrt(number of tasks)

@param: mat: the matrix to shift

@note: none

*******************************/
void ShiftLeft( int taskID, int shiftAmnt, size_t matrixDivider, tMath::tMatrix< int > & mat )
{
    int tmpID = taskID - shiftAmnt;
    MPI_Status status;

    if( tmpID < static_cast< int > (( taskID / matrixDivider ) * matrixDivider ) )
    {
        tmpID += matrixDivider;
    }

    if( tmpID != taskID )
    {
        MPI_Sendrecv_replace( &mat( 0, 0 ), mat.size( ), MPI_INT, tmpID, SEND_DATA, MPI_ANY_SOURCE, SEND_DATA, MPI_COMM_WORLD, &status );
    }
}

/*******************************

@brief: ShiftUp

@details: shifts a matrix Up

@param: taskID: the task ID

@param: shiftAmnt: the amount to shift the row over

@param: matrixDivider: the number that the input matrix is divided by (equal to the sqrt(number of tasks)

@param: mat: the matrix to shift

@note: none

*******************************/
void ShiftUp( int taskID, int shiftAmnt, size_t matrixDivider, tMath::tMatrix< int > & mat )
{
    int tmpID = taskID - static_cast< int >( shiftAmnt * matrixDivider );
    MPI_Status status;

    if( tmpID < 0 ) //if incorrect
    {
        //then correct
        tmpID = taskID + static_cast< int >( matrixDivider * ( matrixDivider - shiftAmnt ) );
    }

    if( tmpID != taskID )
    {
        MPI_Sendrecv_replace( &mat( 0, 0 ), mat.size( ), MPI_INT, tmpID, SEND_DATA + 1, MPI_ANY_SOURCE, SEND_DATA + 1, MPI_COMM_WORLD, &status );
    }
}

/*******************************

@brief: InitializeWithFiles

@details: initializes the program variables and main based on the values in two files

@param: taskID: the task ID

@param: numberOfTasks: the number of tasks

@param: fileA: the first file's name

@param: fileB: the second file's name

@param: numberOfValues: the dimension of a matrix

@param: matrixDim: the dimension of a submatrix

@param: matrixDivider: the dimension of the processor mesh

@param: rData: the temporary buffer

@param: matA: the first matrix

@param: matB: the second matrix

@note: none

*******************************/
bool InitializeWithFiles
(
    int taskID,
    int numberOfTasks,
    std::string & fileA, 
    std::string &fileB,
    size_t & numberOfValues,
    size_t & matrixDim,
    size_t & matrixDivider,
    std::vector< int > & rData,
    tMath::tMatrix< int > & matA,
    tMath::tMatrix< int > & matB
)
{
    std::fstream fileStream;
    int tmpInt;
    MPI_Status status;

    //open file and send out data
    if( taskID == 0 /*MASTER*/ )
    {
        if( !MasterOpenFile( fileStream, numberOfTasks, fileA ) )
        {
            return false;
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
            return false;
        }

        //fill matrices
        matrixDim = numberOfValues / matrixDivider;
        matA.resize( matrixDim, matrixDim );
        rData.resize( matrixDim );

        //send out data
        MasterPopulateMats( rData, numberOfValues, matrixDim,
                            matrixDivider, numberOfTasks, fileStream, matA, 1 );

        fileStream.close( );
        fileStream.clear( );

        //repeat above with matrix b
        if( !MasterOpenFile( fileStream, numberOfTasks, fileB ) )
        {
            return false;
        }

        tmpInt = static_cast< int >( numberOfValues );
        fileStream >> numberOfValues;

        if( static_cast< size_t > ( tmpInt ) != numberOfValues )
        {
            numberOfValues = KILL_SWITCH;
        }

        for( tmpInt = 1; tmpInt < numberOfTasks; tmpInt++ )
        {
            MPI_Send( &numberOfValues, 1, MPI_INT, tmpInt, SEND_DATA + 2, MPI_COMM_WORLD );
        }

        if( numberOfValues == KILL_SWITCH )
        {
            std::cout << "Error: different matrix sizes between A and B from " << taskID << std::endl;
            MPI_Finalize( );
            return false;
        }

        matB.resize( matrixDim, matrixDim );

        //send out data
        MasterPopulateMats( rData, numberOfValues, matrixDim,
                            matrixDivider, numberOfTasks, fileStream, matB, 2 );

        fileStream.close( );
        fileStream.clear( );
    }
    else
    {
        //A
        MPI_Recv( &tmpInt, 1, MPI_INT, 0, SEND_DATA + 1, MPI_COMM_WORLD, &status );
        numberOfValues = static_cast< size_t > ( tmpInt );

        if( numberOfValues >= KILL_SWITCH )
        {
            MPI_Finalize( );
            return false;
        }

        if( numberOfValues % matrixDivider != 0 ) //check for uneven division of matrix dimensions
        {
            std::cout << "Error: uneven dimension division from node " << taskID << std::endl;
            MPI_Finalize( );
            return false;
        }

        //fill matrices
        matrixDim = numberOfValues / matrixDivider;
        matA.resize( matrixDim, matrixDim );
        rData.resize( matrixDim );

        SlaveRecvMat( rData, matA, 1 );

        //B
        MPI_Recv( &tmpInt, 1, MPI_INT, 0, SEND_DATA + 2, MPI_COMM_WORLD, &status );
        numberOfValues = static_cast< size_t > ( tmpInt );

        if( numberOfValues >= KILL_SWITCH )
        {
            MPI_Finalize( );
            return false;
        }

        matB.resize( matrixDim, matrixDim );
        SlaveRecvMat( rData, matB, 2 );

    }

    return true;
}

/*******************************

@brief: MasterOpenFile

@details: opens a file; terminates the application if it fails to open

@param: fileStream: the filestream to open

@param: numberOfTasks: the number of tasks

@param: syncOffset: the offset in the send tag

@note: none

*******************************/
bool MasterOpenFile( std::fstream & fileStream, int numberOfTasks, std::string & fileName )
{
    size_t index, rIndex;

    fileStream.open( fileName.c_str( ) );

    if( !fileStream.is_open( ) )
    {
        std::cout << "Error: Could not open " << fileName << std::endl;
        for( index = 1; index < static_cast<size_t> ( numberOfTasks ); index++ )
        {
            rIndex = KILL_SWITCH;
            MPI_Send( &rIndex, 1, MPI_INT, index, SEND_DATA + 1, MPI_COMM_WORLD );
        }
        MPI_Finalize( );
        return false;
    }

    return true;
}


/*******************************

@brief: MasterPopulateMats

@details: populates matrices in the master and the slaves from file

@param: rData: the preallocated temporary buffer

@param: numberOfValues: the dimension of a matrix

@param: matrixDim: the dimension of a submatrix

@param: matrixDivider: the dimension of the processor mesh

@param: numberOfTasks: the number of tasks

@param: fileStream: an openfile stream of matrix data;
                    the dimension of the matrix has already been read in

@param: mat: the mat being populated

@param: syncOffset: the offset in the send tag

@note: none

*******************************/
void MasterPopulateMats
( 
    std::vector< int > & rData, 
    size_t numberOfValues, 
    size_t matrixDim,
    size_t matrixDivider,
    int numberOfTasks,
    std::fstream & fileStream, 
    tMath::tMatrix< int > & mat, 
    int syncOffset 
)
{
    size_t index, row, col;

    int tmpID, tmpInt;

    //send out data
    for( index = 0; index < numberOfValues; index++ )
    {
        row = index % matrixDim;
        for( tmpInt = 0; tmpInt < static_cast<int> ( matrixDivider ); tmpInt++ )
        {
            tmpID = tmpInt + ( matrixDivider * ( index / matrixDim ) );

            if( tmpID != 0 )
            {
                MPI_Send( &row, 1, MPI_INT, tmpID, 
                          SEND_DATA + syncOffset, MPI_COMM_WORLD );

                for( col = 0; col < rData.size( ); col++ )
                {
                    fileStream >> rData[ col ];
                }

                MPI_Send( &rData[ 0 ], rData.size( ), MPI_INT, tmpID, 
                          SEND_DATA + syncOffset, MPI_COMM_WORLD );

            }
            else
            {
                for( col = 0; col < rData.size( ); col++ )
                {
                    fileStream >> mat( row, col );
                }
            }


        }

    }

    for( index = 1; index < static_cast<size_t> ( numberOfTasks ); index++ )
    {
        col = KILL_SWITCH;
        MPI_Send( &col, 1, MPI_INT, index, SEND_DATA + syncOffset, MPI_COMM_WORLD );
    }
}

/*******************************

@brief: SlaveRecvMat

@details: receives matrices from the master

@param: rData: the preallocated temporary buffer

@param: mat: the mat being populated

@param: syncOffset: the offset in the send tag

@note: none

*******************************/
void SlaveRecvMat( std::vector< int > & rData, tMath::tMatrix< int > & mat, int syncOffset )
{
    int tmpInt;

    size_t row, col;

    MPI_Status status;

    while( true )
    {
        MPI_Recv( &tmpInt, 1, MPI_INT, 0, SEND_DATA + syncOffset, MPI_COMM_WORLD, &status );
        row = static_cast< size_t > ( tmpInt );

        if( row >= KILL_SWITCH )
        {
            break;
        }

        MPI_Recv( &rData[ 0 ], rData.size( ), MPI_INT, 0, 
                  SEND_DATA + syncOffset, MPI_COMM_WORLD, &status );

        for( col = 0; col < rData.size( ); col++ )
        {
            mat( row, col ) = rData[ col ];
        }

    }


}

/*******************************

@brief: PrintFromMaster

@details: prints matrices in the master as they are retrieved from slaves

@param: rData: the preallocated temporary buffer

@param: numberOfValues: the dimension of a matrix

@param: matrixDim: the dimension of a submatrix

@param: matrixDivider: the dimension of the processor mesh

@param: numberOfTasks: the number of tasks

@param: mat: the mat being populated

@param: syncOffset: the offset in the send tag

@note: none

*******************************/
void PrintFromMaster
( 
    std::vector< int > & rData, 
    size_t numberOfValues, 
    size_t matrixDim,
    size_t matrixDivider, 
    int numberOfTasks,
    tMath::tMatrix< int > & mat, 
    int syncOffset 
)
{
    size_t index, row, rIndex;

    int tmpInt, tmpID;

    MPI_Status status;    

    //write out matrix
    for( index = 0; index < numberOfValues; index++ )
    {
        row = index % matrixDim;
        for( tmpInt = 0; tmpInt < static_cast<int> ( matrixDivider ); tmpInt++ )
        {
            tmpID = tmpInt + ( matrixDivider * ( index / matrixDim ) );

            if( tmpID != 0 )
            {
                MPI_Send( &row, 1, MPI_INT, tmpID, SAVE_CODE + syncOffset, MPI_COMM_WORLD );
                MPI_Recv( &rData[ 0 ], rData.size( ), MPI_INT, tmpID, SAVE_CODE + syncOffset, MPI_COMM_WORLD, &status );
            }
            else
            {
                mat.getRow( index, rData ); //write out master
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
        MPI_Send( &rIndex, 1, MPI_INT, index, SAVE_CODE + syncOffset, MPI_COMM_WORLD );
    }
}

/*******************************

@brief: SendSlaveToMaster

@details: sends a matrix to the master for printing

@param: rData: the preallocated temporary buffer

@param: mat: the mat being populated

@param: syncOffset: the offset in the send tag

@note: none

*******************************/
void SendSlaveToMaster( std::vector< int > & rData, tMath::tMatrix< int > & mat, int syncOffset )
{
    int tmpInt;
    size_t rIndex;

    MPI_Status status;
    while( true )
    {
        MPI_Recv( &tmpInt, 1, MPI_INT, 0, SAVE_CODE + syncOffset, MPI_COMM_WORLD, &status );
        rIndex = static_cast< size_t > ( tmpInt );

        if( rIndex >= KILL_SWITCH )
        {
            break;
        }
        mat.getRow( rIndex, rData );
        MPI_Send( &rData[ 0 ], rData.size( ), MPI_INT, 0, SAVE_CODE + syncOffset, MPI_COMM_WORLD );
    }
}

