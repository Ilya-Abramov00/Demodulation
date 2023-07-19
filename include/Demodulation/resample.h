#ifndef RESAMPLE_H
#define RESAMPLE_H
#include <vector>
#include "../../libs/Base/include/Base/complex.h"
#include <cmath>

class Resample {

public:

    template < typename Type >
    std::vector< Complex< Type > > resampled( std::vector< Complex< Type > > const& mData, uint32_t freqOLD, uint32_t freqNEW );
    template < typename Type >
    std::vector< Type > resampled( std::vector< Type > const& mData, uint32_t freqOLD, uint32_t freqNEW );

private:

};

template < typename Type >
std::vector< Type > Resample::resampled( std::vector< Type > const& mData, uint32_t freqOLD, uint32_t freqNEW ) {


    if( ( freqNEW == 0 ) || ( freqOLD == 0 ) ) {

        throw std::runtime_error( "Fd cant be equal 0" );
    }
    if( freqNEW >= freqOLD ) {
        throw std::runtime_error( "wrong new Fd" );
    }

    uint64_t size = mData.size();
    if( size == 0 ) {
        throw std::runtime_error( "size cant be equal 0" );
    }
    uint32_t k = round( freqOLD / freqNEW );
    if( k == 1 ) {
        throw std::runtime_error( "no resample with these frequencies" );
    }
    float maxW = 0;
    uint32_t maxQ = 0;
    float Wy = 0;

    uint64_t sizere = round( size / k );
    std::vector< Type > res_data( sizere );
    if( k > size ) {
        throw std::runtime_error( "data length is too short" );
    }


    for( uint32_t q = 0; q < k; q++ ) {

        Wy = 0;

        for( uint32_t i = 0; i < ( sizere ); i++ ) {

            res_data[ i ] = mData[ i * k + q ];
            Wy = Wy + pow( res_data[ i ], 2 );
        }
        if( Wy > maxW ) {
            maxW = Wy;
            maxQ = q;
        }

    }
    for( uint32_t i = 0; i < ( sizere ); i++ ) {

        res_data[ i ] = mData[ i * k + maxQ ];

    }

    return res_data;
}

template < typename Type >
std::vector< Complex< Type > > Resample::resampled( std::vector< Complex< Type > > const& mData, uint32_t freqOLD, uint32_t freqNEW ) {


    if( ( freqNEW == 0 ) || ( freqOLD == 0 ) ) {

        throw std::runtime_error( "Fd cant be equal 0" );
    }
    if( freqNEW > freqOLD ) {
        throw std::runtime_error( "wrong new Fd" );
    }

    uint64_t size = mData.size();
    if( size == 0 ) {
        throw std::runtime_error( "size cant be equal 0" );
    }
    uint32_t k = round( freqOLD / freqNEW );
    if( k == 1 ) {
        throw std::runtime_error( "no resample with these frequencies" );
    }
    double maxW = 0;
    uint32_t maxQ = 0;
    double Wy = 0;
    uint64_t sizere = round( size / k );
    if( k > size ) {
        throw std::runtime_error( "data length is too short" );
    }

    std::vector< Complex< Type > > res_data( sizere );


    for( uint32_t q = 0; q < k; q++ ) {

        Wy = 0;

        for( uint32_t mi = 0; mi < ( sizere ); mi++ ) {

            res_data[ mi ] = mData[ mi * k + q ];
            Wy += res_data[ mi ].abs() * res_data[ mi ].abs();
        }
        if( Wy > maxW ) {
            maxW = Wy;
            maxQ = q;
        }

    }
    for( uint32_t i = 0; i < ( sizere ); i++ ) {

        res_data[ i ] = mData[ i * k + maxQ ];

    }

    return res_data;
}

#endif // RESAMPLE_H
