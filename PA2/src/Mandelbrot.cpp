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

void mb::CalculatePixelAt
(
    int col,
    int row,
    int width,
    const ComplexNumber & min,
    const ComplexNumber & scale,
    std::vector<unsigned char> & image
)
{
    ComplexNumber cNum;
    cNum.real = min.real + ( static_cast<float>(col) * scale.real );
    cNum.imaginary = min.imaginary + ( static_cast<float>(row) * scale.imaginary );
    image[ width * row + col ] = static_cast<unsigned char>( mb::PixelGenerator( cNum ) );
}
