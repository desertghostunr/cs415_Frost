
/**************************************************************

@file tMatrix.h

@brief templated matrix

@description templated matrix class

@author Andrew Frost

@version 1.00 ( 04/07/2017 )

@note None

***************************************************************/

// pre-compiler directives ///////////////////////////////////
#ifndef TMATRIX_H
#define TMATRIX_H

// header files /////////////////////////////////////////////
#include <vector>

namespace tMath
{
    //class declaration /////////////////////////////////////////
    template< typename Type >
    class tMatrix
    {
        public:
            //constructors
            /*
            @brief: tMatrix
            
            @details: constructs a matrix
            
            @params: none
            */
            tMatrix( ) : m_data( ), m_rows( 0 ), m_cols( 0 ) { }

            /*
            @brief: tMatrix
            
            @details: constructs a matrix with a certain size
            
            @param: rows: the number of rows, integer >= 0

            @param: cols: the number of cols, integer >= 0

            */
            tMatrix( size_t rows, size_t cols ) : m_data( rows ), m_rows( rows ), m_cols( cols )
            {
                size_t index;

                if( rows <= 0 || cols <= 0 )
                {
                    rows = 0;
                    cols = 0;
                }

                m_data.resize( rows );

                for( index = 0; index < rows; index++ )
                {
                    m_data[ index ].resize( cols );
                }

            }

            /*
            @brief: tMatrix
            
            @details: constructs a matrix as a copy of another
            
            @param: src: the source matrix

            */
            tMatrix( const tMatrix<Type> & src ) : m_data( src.m_data ), m_rows( src.m_rows ), m_cols( src.m_cols ){ }

            //destructor
            ~tMatrix( )
            {
                clear( );
            }

            //operators
            /*
            @brief: = 

            @details: the assignment operator, performs a hard copy

            @param: src: the matrix on the rhside
            */
            const tMatrix<Type> & operator = ( const tMatrix<Type> & src )
            {
                if( this != &src )
                {
                    m_data = src.m_data;
                    m_rows = src.m_rows;
                    m_cols = src.m_cols;
                }

                return *this;
            }

            /*
            @brief: ==

            @details: the equality operator

            @param: lh: the lh matrix

            @param: rh: the rh matrix
            */
            friend bool operator == ( const tMatrix & lh, const tMatrix & rh)
            {
                return ( lh.m_rows == rh.m_rows ) 
                       && ( lh.m_cols == rh.m_cols ) 
                       && ( lh.m_data == rh.m_data );
            }

            /*
            @brief: !=

            @details: the inequality operator

            @param: lh: the lh matrix

            @param: rh: the rh matrix
            */
            friend bool operator != ( const tMatrix & lh, const tMatrix & rh)
            {
                return ( lh.m_rows != rh.m_rows ) 
                       || ( lh.m_cols != rh.m_cols ) 
                       || ( lh.m_data != rh.m_data );
            }

            /*
            @brief: +

            @details: the addition operator

            @param: other: the other matrix
            */
            tMatrix<Type> operator + ( const tMatrix<Type> &other)
            {
                size_t row, col;
                tMatrix<Type> retMat;

                if( !isSameSize( other ) )
                {
                    return retMat;
                }

                retMat.resize( m_rows, m_cols );

                for( row = 0; row < m_rows; row++ )
                {
                    for( col = 0; col < m_cols; col++ )
                    {
                        retMat( row,col ) = m_data[row ][col] + other(row, col);
                    }
                }

                return retMat;
            }

            /*
            @brief: -

            @details: the subtraction operator

            @param: other: the other matrix
            */
            tMatrix<Type> operator - ( const tMatrix<Type> &other)
            {
                size_t row, col;
                tMatrix<Type> retMat;

                if( !isSameSize( other ) )
                {
                    return retMat;
                }

                retMat.resize( m_rows, m_cols );

                for( row = 0; row < m_rows; row++ )
                {
                    for( col = 0; col < m_cols; col++ )
                    {
                        retMat( row,col ) = m_data[row ][ col] - other(row, col);
                    }
                }

                return retMat;
            }


            /*
            @brief: *

            @details: the multiplication operator

            @param: other: the other matrix
            */
            tMatrix<Type> operator * ( const tMatrix<Type> &other)
            {
                size_t row, col, index;
                tMatrix<Type> retMat;

                if( !canBeMultiplied( other ) )
                {
                    return retMat;
                }

                retMat.resize( m_rows, other.m_cols );

                for( row = 0; row < retMat.rows( ); row++ )
                {
                    for( col = 0; col < retMat.cols( ); col++ )
                    {
                        retMat( row, col ) = 0;
                        for( index = 0; index < m_cols; index++ )
                        {
                            retMat( row,col ) += m_data[row ][ index] * other(index, col);
                        }
                    }
                }

                return retMat;
            }

            /*
            @brief: ( )

            @details: the parenthesis operator for indexing

            @param: row: the row to access

            @param: col: the col to access
            */
            Type & operator( )( size_t row, size_t col )
            {
                return m_data[row][col];
            }

            /*
            @brief: ( )

            @details: the parenthesis operator for indexing

            @param: row: the row to access

            @param: col: the col to access
            */
            const Type & operator( )( size_t row, size_t col ) const
            {
                return m_data[row][col];
            }


            //accessors
            /*
            @brief: isSameSize

            @details: the addition operator

            @param: other: the other matrix
            */
            bool isSameSize( const tMatrix<Type> &other ) const
            {
                return ( m_cols == other.m_cols ) && ( m_rows == other.m_rows );
            }

            /*
            @brief: canBeMultiplied

            @details: checks if this and other can be multiplied

            @param: other: the other matrix
            */
            bool canBeMultiplied( const tMatrix<Type> & other ) const
            {
                return ( m_cols == other.m_rows );
            }

            /*
            @brief: rows

            @details: returns the number of rows

            @param: none
            */
            size_t rows( ) const
            {
                return m_rows;
            }

            /*
            @brief: cols

            @details: returns the number of columns

            @param: none
            */
            size_t cols( ) const
            {
                return m_cols;
            }


            /*
            @brief: empty

            @details: returns true if the matrix is empty

            @param: none
            */
            bool empty( )
            {
                return m_rows == 0;
            }

            /*
            @brief: isSquare

            @details: returns true if the rows == cols

            @param: none
            */
            bool isSquare( )
            {
                return m_rows == m_cols;
            }

            /*
            @brief: transpose

            @details: returns the transpose of the matrix

            @param: none
            */
            tMatrix<Type> transpose( )
            {
                size_t cols;
                size_t rows;

                tMatrix<Type> trans( m_cols, m_rows );

                for( rows = 0; rows < m_cols; rows++ )
                {
                    for( cols = 0; cols < m_rows; cols++ )
                    {
                        trans.m_data[ rows ][ cols ] = m_data[ cols ][ rows ];
                    }
                }

                return trans;
            }

            /*
            @brief: getRow

            @details: copies the row into destRow

            @param: row: the row to copy

            @param: destRow: the row to copy into to
            */
            bool getRow( size_t row, std::vector<Type> & destRow )
            {
                if( row >= m_rows )
                {
                    return false;
                }

                destRow = m_data[ row ];

                return true;
            }

            /*
            @brief: getCol

            @details: copies the col into destCol

            @param: col: the col to copy

            @param: colRow: the col to copy into to
            */
            bool getCol( size_t col, std::vector<Type> & destCol )
            {
                size_t index;

                if( col >= m_cols )
                {
                    return false;
                }

                destCol.resize( m_rows );

                for( index = 0; index < m_cols; index++ )
                {
                    destCol[ index ] = m_data[ index ][ col ];
                }

                return true;
            }

            /*
            @brief: copyToVector

            @details: copies the matrix into a std::vector

            @param: data: the vector to copy the data into
            */
            void copyToVector( std::vector< Type > & data )
            {
                size_t index;
                vector.resize( 0 );
                
                for( index = 0; index < m_rows; index++ )
                {
                    data.insert( data.end( ), m_data[ index ].begin( ), m_data[ index ].end( ) );
                }

            }

            //modifiers
             /*
            @brief: resize

            @details: resizes the matrix

            @param: rows: the new row size

            @param: cols: the new col size

            @note none
            */
            void resize( size_t rows, size_t cols )
            {

                size_t index;

                m_rows = rows;
                m_cols = cols;

                if( rows <= 0 || cols <= 0 )
                {
                    m_rows = 0; 
                    m_cols = 0;
                    m_data.clear( );
                    return;
                }

                m_data.resize( rows );

                for( index = 0; index < rows; index++ )
                {
                    m_data[ index ].resize( cols );
                }

            }


            /*
            @brief: clear

            @details: deallocates all memory in the matrix

            @param: none
            */
            void clear()
            {
                m_data.clear( );
                m_rows = 0;
                m_cols = 0;
            }

        private:
            std::vector< std::vector< Type > > m_data;
            size_t m_rows;
            size_t m_cols;

    };

}




#endif //!TMATRIX_H