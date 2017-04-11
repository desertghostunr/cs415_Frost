/**************************************************************

@file tMath.h

@brief templated functions for generating numbers and other mathematical operations

@description templated functions for generating numbers and other mathematical operations

@author Andrew Frost

@version 1.00 ( 04/07/2017 )

@note None

***************************************************************/

// pre-compiler directives ///////////////////////////////////
#ifndef TMATH_H
#define TMATH_H

//header files ///////////////////////////////////////////////
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <iostream>
#include "tMatrix.h"
#include "mpi.h"

//namespace declaration //////////////////////////////////////
namespace tMath
{
    //type defs

    // templated function declarations ///////////////////////

    /*
    @brief: MultiplyMatrices

    @details: multiplies A and B together and stores the result in C

    @param: matA: the first matrix

    @param: matB: the second matrix

    @param: matC: a pre-allocated, pre-zero filled matrix to store the result in
    */
    template<typename Type>
    bool MultiplyMatrices( const tMatrix<Type> & matA, const tMatrix<Type> & matB, tMatrix<Type> & matC )
    {
        size_t row, col, inner;

        //error checking
        if( matC.rows( ) != matA.rows( )
            || matC.cols( ) != matB.cols( ) 
            || matA.cols( ) != matB.rows( ) )
        {
            return false;
        }


        //matrix multiplication

        for( row = 0; row < matC.rows( ); row++ )
        {
            for( col = 0; col < matC.cols( ); col++ )
            {
                for( inner = 0; inner < matA.cols( ); inner++ )
                {
                    matC( row, col ) += matA( row, inner ) * matB( inner, col );
                }
            }
        }

        return true;
    }

    /*
    @brief: MakeMatrix

    @details:fills a pre-allocated matrix with data

    @param: mat: the matrix to fill
    */
    template<typename Type>
    void MakeMatrix( tMatrix<Type> & mat )
    {
        size_t row, col;

        //matrix filling
        for( row = 0; row < mat.rows( ); row++ )
        {
            for( col = 0; col < mat.cols( ); col++ )
            {
                mat( row, col ) = row + col + 1;
            }
        }
    }

    /*
    @brief: MakeMatrix

    @details: fills a pre-allocated matrix with data based off of a logical offset

    @param: mat: the matrix to fill

    @param: rOffset: the logical offset in rows

    @param: cOffset: the logical offset in columns
    */
    template<typename Type>
    void MakeMatrix( tMatrix<Type> & mat, size_t rOffset, size_t cOffset )
    {
        size_t row, col;

        //matrix filling
        for( row = 0; row < mat.rows( ); row++ )
        {
            for( col = 0; col < mat.cols( ); col++ )
            {
                mat( row, col ) = rOffset + cOffset + row + col + 1;
            }
        }
    }

     /*
    @brief: ZeroMatrix

    @details: zeroes out a pre-allocated matrix

    @param: mat: the matrix to fill
    */
    template<typename Type>
    void ZeroMatrix( tMatrix<Type> & mat )
    {
        size_t row, col;

        //matrix filling
        for( row = 0; row < mat.rows( ); row++ )
        {
            for( col = 0; col < mat.cols( ); col++ )
            {
                mat( row, col ) = 0;
            }
        }
    }

    /*
    @brief: PrintMatrix

    @details: prints out a matrix

    @param: mat: the matrix to print
    */
    template<typename Type>
    void PrintMatrix( tMatrix<Type> & mat )
    {
        size_t row, col;

        //matrix filling
        for( row = 0; row < mat.rows( ); row++ )
        {
            for( col = 0; col < mat.cols( ); col++ )
            {
                std::cout << mat( row, col ) << "\t";
            }
            std::cout << std::endl;
        }
    }



    /*
    @brief: generateData

    @details: generates data to sort

    @param: in: seed: the seed to the algorithm with

    @param: in: numberOfValues: the number of pieces of data to generate

    @param in: upperBound: highest value

    @param: out: data: a vector of the data generated
    */
    template<typename Type>
    void generateData( unsigned int seed, size_t numberOfValues, const Type & upperBound, std::vector<Type> & data )
    {
        size_t index;
        data.resize( 0 );
        data.reserve( numberOfValues );

        std::srand( seed ); //seed

        for( index = 0; index < numberOfValues; index++ ) //generate values
        {
            data.push_back( static_cast<Type>( std::rand( ) % ( upperBound + 1 ) ) );
        }
    }

}


// terminating pre-compiler directives ///////////////////////
#endif //!TMATH_H
