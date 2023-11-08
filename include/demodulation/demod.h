#ifndef DEMOD_H
#define DEMOD_H
#include <vector>
#include "base/moveFreq.h"
#include "base/complex.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

class Demodulation {

public:
	/**
	 * @brief demodAM dfgh
	 * @param sigFile xcvb
	 * @return dftuygubni
	 */
	template < typename Type >
	std::vector< Type > demodAM( std::vector< Base::Complex< Type > > const& mData );
	template < typename Type >
	std::vector< Type > demodFM( std::vector< Base::Complex< Type > > const& mData );
private:
};

template < typename Type >
std::vector< Type > Demodulation::demodAM( std::vector< Base::Complex< Type > > const& mData ) {
	uint64_t size = mData.size();
	std::vector< Type > data( size );
	for( uint64_t i = 0; i < size; i++ ) {
		data[ i ] = mData[ i ].abs() - 1;
	}
	return data;
}

template < typename Type >
std::vector< Type > Demodulation::demodFM( std::vector< Base::Complex< Type > > const& mData ) {
	uint64_t size = mData.size();
	std::vector< Type > data( size );
	for( uint64_t i = 0; i < size; i++ ) {
		data[ i ] = mData[ i ].arg() * 0.5 * M_1_PI;
	}
	Type temp = data[ 0 ];
	for( uint64_t i = 1; i < size; i++ ) {
		Type dif = data[ i ] - temp;
		temp = data[ i ];
		if( dif > 0.5 ) dif--;
		if( dif < -0.5 ) dif++;
		data[ i ] = dif;
	}
	return data;
}
#endif // DEMOD_H
