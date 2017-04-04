/********************************

@author: Andrew Frost

@brief: AverageData.cpp

@details: averages the timing data collected

@version: 1.00 (04/04/2017)

@Note: None

*******************************/

// header files /////////////////////////////
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// helper structs //////////////////////////
struct TimingData
{
    std::string type;
    int buckets;
    int numberOfIntegers;
    double sTime;
    int denominator;

};

// main ////////////////////////////////////
int main( )
{

    std::string file;
    std::stringstream strStream;
    std::fstream fileStream;
    std::vector<TimingData> averagedData;
    TimingData tmpData;


    std::cout << "Please enter the file path: ";

    std::cin >> file;

    std::cout << std::endl << "Opening " << file << "." <<std::endl;

    // read in the data
    fileStream.open( file.c_str( ) );

    if( !fileStream.is_open( ) )
    {
        std::cout << "Unable to open " << file <<std::endl;
        return -1;

    }

    // place file contents in buffer
    strStream.str( std::string( std::istreambuf_iterator<char>( fileStream ), 
                                std::istreambuf_iterator<char>( ) ) );

    fileStream.close( );


    // process the buffer
    while( ( strStream >> tmpData.type ) )
    {
        strStream >> tmpData.buckets;
        strStream >> tmpData.numberOfIntegers;
        strStream >> tmpData.sTime;
        tmpData.denominator = 1;

        
    }


    return 0;

}




