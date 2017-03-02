#include "Mandelbrot.h"

int mb::PixelGenerator( const ComplexNumber & complex )
{
    int itCount, maxIts;
    ComplexNumber z;
    float temp, sqLength;

    maxIts = 255;

    z.real  = 0;
    z.imaginary = 0;

    itCount = 0;

    do
    {
        temp = z.real * z.real - z.imaginary * z.imaginary + complex.real;
        z.imaginary = 2 * z.real * z.imaginary + complex.imaginary;
        z.real = temp;
        sqLength =  z.real * z.real + z.imaginary * z.imaginary;
        itCount++;

    } while( (sqLength < 4.0f ) && ( itCount < maxIts ) );

    return itCount;
}

int mb::CalculatePixelAt
(
    int col,
    int row,
    const ComplexNumber & min,
    const ComplexNumber & scale
)
{
    ComplexNumber cNum;
    cNum.real = min.real + ( static_cast<float>(col) * scale.real );
    cNum.imaginary = min.imaginary + ( static_cast<float>(row) * scale.imaginary );
    return ( mb::PixelGenerator( cNum ) );
}
