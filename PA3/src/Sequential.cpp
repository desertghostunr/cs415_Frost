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
#include <fstream>
#include <cmath>
#include <algorithm>
#include <limits>
#include "Timer.h"
#include "tSort.h"

// main /////////////////////////////////////////////////////
int main( int argc, char *argv[ ] )
{
    //vars
    unsigned long long sTime, eTime;
    std::vector< int > data;
    int min, max, tmpInt, numberOfBuckets;
    int index;
    std::string fileName;
    std::stringstream strStream;
    std::fstream file;
    size_t extensionPos;

    //cmd line params
    if( argc < 2 )
    {
        std::cout << "The program must be ran with the following:" <<std::endl;
        std::cout << "./Sequential [file path name]" <<std::endl;
        return -1;
    }

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
            return -1;
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

    //get the number of buckets to use
    if( argc > 2 )
    {
        strStream.str( std::string( "" ) );
        strStream.clear( );

        strStream.str( argv[ 2 ] );

        strStream >> numberOfBuckets;

        if( numberOfBuckets < 1 )
        {
            numberOfBuckets = data.size( );
        }
    }
    else
    {
        numberOfBuckets = data.size( );
    }

    sTime = GetCurrentMicroSecTime( );

    //sort
    tSort::sBucket( data, numberOfBuckets, min, max );

    eTime = GetCurrentMicroSecTime();

    //write out data
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
        return -1;
    }

    file << static_cast<int>( data.size( ) ) << std::endl;

    for( index = 0; index < static_cast<int>( data.size( ) ); index++ )
    {
        file << data[ index ] << std::endl;
    }

    file.close( );

    //print the time

    std::cout<<"Sequential\t1\t";
    std::cout<<data.size()<<"\t"<<ConvertTimeToSeconds( eTime - sTime )<<std::endl;

    return 0;
}

