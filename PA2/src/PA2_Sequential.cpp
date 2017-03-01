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
    std::vector<std::vector<unsigned char>> image; 
    size_t row, col;
    int width = 0, height = 0;
    std::stringstream strStream;
    std::string saveName;
    mb::ComplexNumber cNum, min, max, scale;

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
    image.resize( height );

    for( row = 0; row < image.size( ); row++ )
    {
        image[ row ].resize( width );
    }

    //calculate min and max vals
    max.real = 2;
    max.imaginary = 2;
    min.real = -2;
    min.imaginary = -2;

    scale.real = static_cast<float>( (max.real - min.real) / width );
    scale.imaginary = static_cast<float>( (max.imaginary - min.imaginary) / height );

    //compute the image
    sTime = GetCurrentMicroSecTime();

    for( row = 0; row < image.size( ); row++ )
    {
        for( col = 0; col < image[ row ].size( ); col++ )
        {
            cNum.real = min.real + ( static_cast<float>(col) * scale.real );
            cNum.imaginary = min.imaginary + ( static_cast<float>(row) * scale.imaginary );
            image[row][col] = static_cast<unsigned char>( mb::PixelGenerator( cNum ) );
            std::cout<<mb::PixelGenerator( cNum )<<std::endl;
        }
    }
    
    eTime = GetCurrentMicroSecTime();

    std::cout<<"Image Dimensions\tTime(s)"<<std::endl;
    std::cout<<image[0].size()<<"x"<<image.size()<<"\t"<<ConvertTimeToSeconds( eTime - sTime )<<std::endl;

    pim_write_black_and_white(saveName.c_str(), width, height, &image[0][0]);

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
