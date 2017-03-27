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
#include "Timer.h"

// main /////////////////////////////////////////////////////
int main( int argc, char *argv[ ] )
{
    unsigned long long sTime, eTime;
    std::vector<unsigned char> image;
    int row, col;
    int width = 0, height = 0;
    std::string fileName;

    //cmd line params
    if( argc < 2 )
    {
        std::cout << "The program must be ran with the following:" <<std::endl;
        std::cout << "./Sequential [file path name]" <<std::endl;
        return -1;
    }

    fileName = argv[ 1 ];

    //alloc 

    sTime = GetCurrentMicroSecTime();

    //read in data

    //sort

    eTime = GetCurrentMicroSecTime();

    //write out data

    //print the time  

    std::cout<<"Sequential\t1\t";
    std::cout<<<<"\t"<<ConvertTimeToSeconds( eTime - sTime )<<std::endl;

    return 0;
}

