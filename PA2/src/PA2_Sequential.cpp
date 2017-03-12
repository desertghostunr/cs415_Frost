/**************************************************************

@file PA2_Sequential.cpp

@brief the code for the sequential PA2 program in cs415

@description Calculates a Mandelbrot sequentially

@author Andrew Frost

@version 1.00 ( 02/26/2017 )

@note None

***************************************************************/


// header files //////////////////////////////////////////////
#include <sys/time.h>
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
    std::vector<unsigned char> image;
    int row, col;
    int width = 0, height = 0;
    std::stringstream strStream;
    std::string saveName;
    mb::ComplexNumber min, max, scale;

    //cmd line params
    if( argc < 4 )
    {
        std::cout << "The program must be ran with the following:" <<std::endl;
        std::cout << "./PA2_Sequential [number of columns] [number of rows] [file path name]" <<std::endl;
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

    //alloc image
    image.resize( width * height );

    //calculate min and max vals
    max.real = 2;
    max.imaginary = 2;
    min.real = -2;
    min.imaginary = -2;

    scale.real = (max.real - min.real) / static_cast<float>( width );
    scale.imaginary = (max.imaginary - min.imaginary) / static_cast<float>( height );

    //compute the image
    sTime = GetCurrentMicroSecTime();

    for( row = 0; row < height; row++ )
    {
        for( col = 0; col < width; col++ )
        {
            image[ row * width + col ] = static_cast<unsigned char>( mb::CalculatePixelAt( col, row, min, scale ) );
        }
    }

    eTime = GetCurrentMicroSecTime();

    std::cout<<"Sequential\t1\t";
    std::cout<<width<<"x"<<height<<"\t"<<ConvertTimeToSeconds( eTime - sTime )<<std::endl;

    if(!pim_write_black_and_white(saveName.c_str(), width, height, &image[0]))
    {
        std::cout<<"Failure saving image."<<std::endl;
    }

    return 0;
}

// free function implementation //////////////////////////////////

//get the time in microseconds. stores the result in an unsigned long long
unsigned long long GetCurrentMicroSecTime( )
{
    unsigned long long retTime;

    struct timeval tVal;

    gettimeofday( &tVal, NULL );

    retTime = ( unsigned long long ) (tVal.tv_sec * 1000000L) +( unsigned long long ) tVal.tv_usec;

    return retTime;
}

//gets the time in seconds. stores the result in a double
double ConvertTimeToSeconds( unsigned long long usTime )
{
    return ( double ) usTime / 1000000.0;
}
