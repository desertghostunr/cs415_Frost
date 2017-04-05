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
#include <cstdlib>
#include "mpi.h"

//namespace declaration //////////////////////////////////////
namespace tSort
{
    // templated function declarations ///////////////////////
    /* 
        @brief: insertionSort

        @details: an insertion sort algorithm

        @param: in / out: data: the data to sort

    */   
    template< typename Type >
    void insertionSort( std::vector< Type > & data )
    {
        size_t in, out;
        Type tmp;

        for( out = 1; out < data.size(); out++ )
        {
            in = out;
            while( in > 0 && data[ in - 1 ] > data[ in ] )
            {
                tmp = data[ in ];
                data[ in ] = data[ in - 1 ];
                data[ in - 1 ] = tmp;

                in = in - 1;
            }
        }

    }

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
        double pRegion;
        std::vector< std::vector< Type > > buckets;

        //resize for the number of buckets
        buckets.resize( numberOfBuckets );

        //calculate the partition regions range
        pRegion = ( static_cast< double >( max - min ) + 1.0  ) / static_cast< double >( numberOfBuckets );

        if( pRegion <= 0.0 )
        {
            pRegion = 1.0;

        }

        //sort the data into buckets
        for( index = 0; index < static_cast<int>( data.size( ) ); index++ )
        {
            //calculate the bucket of the number
            targetIndex = static_cast< int >( static_cast< double >( data[ index ] ) / pRegion ) - 1;

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
                insertionSort( buckets[ index ] );    
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

    /*
    @brief: pBucket

    @details: a parallel bucket sort

    @param: in: data: the data to sort

    @param: out: data: the sorted data

    @param: in: numberOfBuckets: the number of buckets to use

    @param: in: bucketID: the ID of this bucket

    @param: in: min: the min value in the data

    @param: in: max: the max value in the data

    */
    void pBucket( std::vector< int > & data, std::vector< std::vector< int > > & buckets, int numberOfBuckets, int bucketID, int min, int max, int maxBucketSize )
    {
        int index;
        int targetIndex;
        double pRegion;
        std::vector< int > tmpBucket;

        tmpBucket.resize( maxBucketSize );

        MPI_Status status;
        MPI_Request request;
        

        //calculate the partition regions range
        pRegion = ( static_cast< double >( max - min ) + 1.0 ) / static_cast< double >( numberOfBuckets );

        if( pRegion <= 0.0 )
        {
            pRegion = 1.0;
        }

        //sort the data into buckets
        for( index = 0; index < static_cast<int>( data.size( ) ); index++ )
        {
            //calculate the bucket of the number
            targetIndex = static_cast< int >( static_cast< double >( data[ index ] ) / pRegion ) - 1;

            //clip for bounds
            targetIndex = std::max( 0, targetIndex );
            targetIndex = std::min( targetIndex, numberOfBuckets - 1 );

            //add data to bucket
            buckets[ targetIndex ].push_back( data[ index ] );

        }

        //send the buckets to the other buckets
        for( index = 0; index < numberOfBuckets; index++ )
        {
            if( index != bucketID )
            {
                MPI_Isend( &buckets[ index ][ 0 ], static_cast< int >( buckets[ index ].size( ) ), MPI_INT, index, static_cast< int >( buckets[ index ].size( ) ), MPI_COMM_WORLD, &request );
            }
        }        

        //receive the data from other buckets
        for( index = 1; index < numberOfBuckets; index++ )
        {
            MPI_Recv( &tmpBucket[ 0 ], maxBucketSize, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status );

            //copy data
            if( status.MPI_TAG > 0 )
            {
                buckets[ bucketID ].insert( buckets[ bucketID ].end( ), tmpBucket.begin( ), tmpBucket.begin( ) + status.MPI_TAG );    

            }
        }        

        //sort my bucket
        if( buckets[ bucketID ].size( ) > 1 )
        {
            insertionSort( buckets[ bucketID ] );
        }
        
        data.resize( 0 );

        data = buckets[ bucketID ];

    }


    /*
    @brief: generateData

    @details: generates data to sort

    @param: in: seed: the seed to the algorithm with

    @param: in: numberOfValues: the number of pieces of data to generate

    @param in: upperBound: highest value

    @param: out: data: a vector of the data generated
    */

    void generateData( unsigned int seed, size_t numberOfValues, int upperBound, std::vector<int> & data )
    {
        size_t index;
        data.resize( 0 );
        data.reserve( numberOfValues );

        std::srand( seed ); //seed

        for( index = 0; index < numberOfValues; index++ ) //generate values
        {
            data.push_back( std::rand( ) % ( upperBound + 1 ) );
        }
    }

}


// terminating pre-compiler directives ///////////////////////
#endif //!TSORT_H
