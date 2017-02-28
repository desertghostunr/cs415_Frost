/**************************************************************

@file PA2_Sequential.cpp

@brief the code for the sequential PA2 program in cs415

@description Calculates a Mandelbrot sequentially

@author Andrew Frost

@version 1.00 ( 02/26/2017 )

@note None

***************************************************************/


// header files //////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include "PIMFuncs.h"
#include "Mandelbrot.h"

//free function prototypes ///////////////////////////////////
unsigned long long GetCurrentMicroSecTime( );

double ConvertTimeToSeconds( unsigned long long usTime );

// main /////////////////////////////////////////////////////
int main( int argc, char *argv[ ] )
{
    unsigned long long sTime, eTime;
    std::vector<std::vector<char>> image; 
    size_t row, col;
    std::stringstream strStream;
    std::string saveName;
    mb::ComplexNumber cNum;

    if( argc < 4 )
    {
        std::cout << "The program must be ran with the following:" <<std::endl;
        std::cout << "./PA2_Sequential [number of columns] [number of rows] [file path name]" <<std::endl;
        return -1;
    }
    
    strStream.str( ) = argv[ 2 ];
    strStream >> col;

    strStream.str( ) = argv[ 3 ];
    strStream >> row;

    saveName = argv[ 4 ];

    image.resize( row );

    for( row = 0; row < image.size( ); row++ )
    {
        image[ row ].resize( col );
    }

    for( row = 0; row < image.size( ); row++ )
    {
        for( col = 0; col < image[ row ].size( ); col++ )
        {
            
        }
    }
    

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
