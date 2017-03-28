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
#include <iostream>

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
    template< typename Type >
    void sBucket( std::vector< Type > & data, int numberOfBuckets, const Type & min, const Type & max )
    {
        int index;
        int targetIndex;
        Type pRegion;
        std::vector< std::vector< Type > > buckets;

        //resize for the number of buckets
        buckets.resize( numberOfBuckets );

        //calculate the partition regions range
        pRegion = ( ( max - min ) + static_cast< Type >( 1 ) ) / static_cast< Type >( numberOfBuckets );
        
        //sort the data into buckets
        for( index = 0; index < static_cast<int>( data.size( ) ); index++ )
        {
            //calculate the bucket of the number
            targetIndex = static_cast< int >( data[ index ] / pRegion ) - 1;

            //clip for bounds
            targetIndex = std::max( 0, targetIndex );
            targetIndex = std::min( targetIndex, numberOfBuckets - 1 );

            //add data to bucket
            buckets[ targetIndex ].push_back( data[ index ] );

        }

        //sort the buckets
        for( index = 0; index < numberOfBuckets; index++ )
        {
            if( buckets[ index ].size( ) > 1 )
            {
                std::sort( buckets[ index ].begin( ), buckets[ index ].end( ) );                
            }
        }        

        //merge the buckets
        data.resize( 0 );

        for( index = 0; index < numberOfBuckets; index++ )
        {
            for( targetIndex = 0; targetIndex < static_cast<int>( buckets[ index ].size( ) ); targetIndex++ )
            {
                data.push_back( buckets[ index ][ targetIndex ] );
            }
        }

    }

}


// terminating pre-compiler directives ///////////////////////
#endif //!TSORT_H