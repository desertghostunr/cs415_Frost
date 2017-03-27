/**************************************************************

@file tSort.h

@brief templated functions for sorting

@description functions that can be used for sorting

@author Andrew Frost

@version 1.00 ( 03/26/2017 )

@note None

***************************************************************/

// pre-compiler directives ///////////////////////////////////
#ifndef TSORT_H
#define TSORT_H

//header files ///////////////////////////////////////////////
#include <algorithm>
#include <vector>

//namespace declaration //////////////////////////////////////
namespace tSort
{
    // templated function declarations ///////////////////////
    /* 
        @brief: sBucket

        @details: a sequential bucket sort

        @param: in: data: the data to sort

        @param: out: data: the sorted data

        @param: in: the number of buckets to use

        @param: in: the min value in the data

        @param: in: the max value in the data

    */    
    template <typename Type>
    void sBucket( std::vector<Type> & data, int numberOfBuckets, const Type & min, const Type & max )
    {
        std::vector<std::vector<Type>> buckets;

        //size for the number of buckets
        buckets.resize( numberOfBuckets );

        
    }

}


// terminating pre-compiler directives ///////////////////////
#endif //!TSORT_H