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

//free function prototypes ///////////////////////////////////
unsigned long long GetCurrentMicroSecTime( );

double ConvertTimeToSeconds( unsigned long long usTime );

// main /////////////////////////////////////////////////////
int main( int argc, char *argv[ ] )
{
    
    

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
