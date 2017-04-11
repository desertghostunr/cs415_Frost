/**************************************************************

@file Sequential.cpp

@brief the code for the sequential matrix multiplication for PA3

@description Sequentially multiplies matrices

@author Andrew Frost

@version 1.00 ( 03/26/2017 )

@note None

***************************************************************/


// header files //////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <limits>
#include "Timer.h"
#include "tMath.h"
#include "tMatrix.h"

// Pre-compiler directives ///////////////////////////////////
#define SAVE_CODE 1
#define SEED 100102330
#define UPPER_BOUND 10

// main /////////////////////////////////////////////////////
int main( int argc, char *argv[ ] )
{
    //vars
    unsigned long long sTime, eTime;    
    int tmpInt;
    std::stringstream strStream;
    size_t numberOfValues;

    tMath::tMatrix< int > matA, matB, matC;

    //cmd line params
    if( argc < 2 )
    {
        std::cout << "The program must be ran with the following:" <<std::endl;
        std::cout << "./Sequential [matrix size]" <<std::endl;
        return -1;
    }

    strStream.str(argv[ 1 ]);    

    //process the data
    if( !( strStream >> numberOfValues ) )
    {
        std::cout << "Error: Invalid matrix size!" << std::endl;
        return -1;
    }

    //generate matrices
    matA.resize( numberOfValues, numberOfValues );
    matC.resize( numberOfValues, numberOfValues );

    tMath::MakeMatrix( matA );

    matB = matA.transpose( );

    tMath::ZeroMatrix( matC );

    //print current data if requested
    tmpInt = -1;
    
    if( argc > 2 )
    {
        strStream.str( std::string( "" ) );
        strStream.clear( );

        strStream.str( argv[ 2 ] );

        strStream >> tmpInt;
    }

    

    if( tmpInt == SAVE_CODE )
    {
        //write out data
        std::cout << "Mat A: "<<std::endl;
        tMath::PrintMatrix( matA );
        std::cout << std::endl;

        std::cout << "Mat B: "<<std::endl;
        tMath::PrintMatrix( matB );
        std::cout << std::endl;
    }


    sTime = GetCurrentMicroSecTime( );

    //multiply matrices
    tMath::MultiplyMatrices( matA, matB, matC );

    eTime = GetCurrentMicroSecTime();   
    
    
    if( tmpInt == SAVE_CODE )
    {
        //write out data
        std::cout << "Mat C: "<<std::endl;
        tMath::PrintMatrix( matC );
        std::cout << std::endl;
    }    

    //print the time

    std::cout<<"Sequential\t"<<1<<"\t";
    std::cout<<numberOfValues<<"\t"<<ConvertTimeToSeconds( eTime - sTime )<<std::endl;

    return 0;
}

