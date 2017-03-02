/**************************************************************

@file Mandelbrot.h

@brief the code to create a Mandelbrot

@description code to calculate a mandelbrot based on the
             implementation in Parallel Programming 2nd Edition
             by Barry Wilkinson and Michael Allen

@author Andrew Frost

@version 1.00 ( 02/26/2017 )

@note None

***************************************************************/

//pre-compiler directives //////////////////////////////////////
#ifndef MANDELBROT_H
#define MANDELBROT_H

// header files //////////////////////////////////////////////

#include <iostream>
#include <vector>
// namespace definition ///////////////////////////////////////////

namespace mb
{
    // struct definitions /////////////////////////////////////////
    struct ComplexNumber
    {
        float real;
        float imaginary;
    };

    // free function prototypes //////////////////////////////////

    int PixelGenerator( const ComplexNumber & complex );

    int CalculatePixelAt( int col, int row,
                          const ComplexNumber & min, const ComplexNumber & scale );

}


#endif //!MANDELBROT_H
