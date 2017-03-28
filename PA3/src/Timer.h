/**************************************************************

@file Timer.h

@brief the prototypes for timing operations

@description functions that can be used for timing and time conversion

@author Andrew Frost

@version 1.00 ( 03/26/2017 )

@note None

***************************************************************/

// pre-compiler directives ////////////////////////////////////////////
#ifndef TIMER_H
#define TIMER_H

//header files /////////////////////////////////////////////////////////
#include <sys/time.h>
#include <iostream>

//Gets the current time of the system in MicroSeconds
unsigned long long GetCurrentMicroSecTime( );

//Converts usTime (microsecond Time) into seconds 
double ConvertTimeToSeconds( unsigned long long usTime );

#endif //!TIMER_H