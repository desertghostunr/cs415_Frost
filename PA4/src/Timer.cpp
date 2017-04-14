/**************************************************************

@file Timer.cpp

@brief the implementation for timing operations

@description functions that can be used for timing and time conversion

@author 

@version 1.00 ( 03/26/2017 )

@note None

***************************************************************/

// header files //////////////////////////////////////////////
#include "Timer.h"


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
