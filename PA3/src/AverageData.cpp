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
#include <vector>
#include <string>

// helper structs //////////////////////////
struct TimingData
{
    std::string type;
    int buckets;
    int numberOfIntegers;
    double sTime;
    double denominator;

};

// main ////////////////////////////////////
int main( )
{

    std::string file;
    std::stringstream strStream;
    std::fstream fileStream;
    std::vector<TimingData> averagedData;
    TimingData tmpData;
    size_t index;
    bool exists;

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

        exists = false;

        for( index = 0; index < averagedData.size( ); index++ )
        {
            if( averagedData[ index ].type == tmpData.type
                && averagedData[ index ].buckets == tmpData.buckets
                && averagedData[ index ].numberOfIntegers == tmpData.numberOfIntegers )
            {
                exists = true;

                averagedData[ index ].sTime += tmpData.sTime;
                averagedData[ index ].denominator += tmpData.denominator;

            }
        }

        if( !exists )
        {
            averagedData.resize( averagedData.size( ) + 1 );
            averagedData.back( ).type = tmpData.type;
            averagedData.back( ).buckets = tmpData.buckets;
            averagedData.back( ).numberOfIntegers = tmpData.numberOfIntegers;
            averagedData.back( ).sTime = tmpData.sTime;
            averagedData.back( ).denominator = tmpData.denominator;
            exists = true;
        }
    }


    std::cout << "Please specify the file path to save the data: ";

    std::cin >> file;

    std::cout << std::endl;


    fileStream.clear( );

    fileStream.open( file.c_str( ) );

    if( !fileStream.is_open( ) )
    {
        std::cout << "Unable to open the file! Sorry." << std::endl;
        return -1;
    }

    for( index = 0; index < averagedData.size( ); index++ )
    {
        fileStream << averagedData[ index ].type << "\t" << averagedData[ index ].buckets << "\t" << averagedData[ index ].numberOfIntegers;
        fileStream << "\t" << ( averagedData[ index ].sTime / averagedData[ index ].denominator ) << std::endl;
    }

    fileStream.close( );

    std::cout << "Save operation complete." << std::endl;

    return 0;

}




