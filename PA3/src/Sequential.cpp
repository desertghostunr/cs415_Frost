/**************************************************************

@file Sequential.cpp

@brief the code for the sequential bucket sorting program for PA3

@description Sequentially runs bucket sort on a set of numbers read from file

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
#include "tSort.h"

// Pre-compiler directives ///////////////////////////////////
#define SAVE_CODE 1
#define SEED 100102330
#define UPPER_BOUND 100000

// main /////////////////////////////////////////////////////
int main( int argc, char *argv[ ] )
{
    //vars
    unsigned long long sTime, eTime;
    std::vector< int > data;
    int min, max, tmpInt, numberOfBuckets;
    int index;
    std::stringstream strStream;
    size_t numberOfValues;

    //cmd line params
    if( argc < 2 )
    {
        std::cout << "The program must be ran with the following:" <<std::endl;
        std::cout << "./Sequential [number of ints]" <<std::endl;
        return -1;
    }

    strStream.str(argv[ 1 ]);    

    //process the data
    if( !( strStream >> numberOfValues ) )
    {
        std::cout << "Error: Invalid number of integers!" << std::endl;
        return -1;
    }

    //generate data
    tSort::generateData( SEED, numberOfValues, UPPER_BOUND, data );
    
    max = -1 * std::numeric_limits<int>::infinity( );
    min = std::numeric_limits<int>::infinity( );

    for( index = 0; index < static_cast<int>( data.size( ) ); index++ )
    {
        tmpInt = data[index];

        if( tmpInt < min )
        {
            min = tmpInt;
        }

        if( tmpInt > max )
        {
            max = tmpInt;
        }
    }

    //get the number of buckets to use
    if( argc > 2 )
    {
        strStream.str( std::string( "" ) );
        strStream.clear( );

        strStream.str( argv[ 2 ] );

        strStream >> numberOfBuckets;

        if( numberOfBuckets < 1 )
        {
            numberOfBuckets = std::min( static_cast<int>( data.size( ) ), max - min );
        }
    }
    else
    {
        numberOfBuckets = std::min( static_cast<int>( data.size( ) ), max - min );
    }

    numberOfBuckets = std::max( 1, numberOfBuckets );

    //print current data if requested
    tmpInt = -1;
    
    if( argc > 3 )
    {
        strStream.str( std::string( "" ) );
        strStream.clear( );

        strStream.str( argv[ 3 ] );

        strStream >> tmpInt;
    }

    

    if( tmpInt == SAVE_CODE )
    {
        //write out data
        std::cout << "Unsorted data: "<<std::endl;
        std::cout<< static_cast<int>( data.size( ) ) << std::endl;

        for( index = 0; index < static_cast<int>( data.size( ) ); index++ )
        {
            std::cout << data[ index ] << std::endl;
        }
        std::cout << std::endl;
    }


    sTime = GetCurrentMicroSecTime( );

    //sort
    tSort::sBucket( data, numberOfBuckets, min, max );

    eTime = GetCurrentMicroSecTime();   
    
    
    if( tmpInt == SAVE_CODE )
    {
        //write out data
        std::cout << "Sorted data: "<<std::endl;
        std::cout<< static_cast<int>( data.size( ) ) << std::endl;

        for( index = 0; index < static_cast<int>( data.size( ) ); index++ )
        {
            std::cout << data[ index ] << std::endl;
        }
        std::cout << std::endl;
    }    

    //print the time

    std::cout<<"Sequential\t"<<numberOfBuckets<<"\t";
    std::cout<<data.size()<<"\t"<<ConvertTimeToSeconds( eTime - sTime )<<std::endl;

    return 0;
}

