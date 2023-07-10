#ifndef DEMOD_H
#define DEMOD_H
#include <vector>
#include "../../libs/Base/include/Base/signalFile.h"
#include "../../libs/Base/include/Base/complex.h"
#define _USE_MATH_DEFINES
#include <cmath>

class Demodulation {

public:
	/**
	 * @brief demodAM dfgh
	 * @param sigFile xcvb
	 * @return dftuygubni
	 */
	template < typename Type >
	std::vector< Type > demodAM( SignalFile< Type > const& sigFile );
	template < typename Type >
	std::vector< Type > demodFM( SignalFile< Type > const& sigFile );
private:
};

template < typename Type >
std::vector< Type > Demodulation::demodAM( SignalFile< Type > const& sigFile ) {
	uint64_t size = sigFile.size();
	std::vector< Type > data( size );
	for( uint64_t i = 0; i < size; i++ ) {
		data[ i ] = sigFile.data( i ).abs() - 1;
	}
	return std::move( data );
}

template < typename Type >
std::vector< Type > Demodulation::demodFM( SignalFile< Type > const& sigFile ) {
	uint64_t size = sigFile.size();
	uint64_t fCen = sigFile.freqCenter();
	uint32_t fSam = sigFile.freqSample();
	std::vector< Type > data( size );
	for( uint64_t i = 0; i < size; i++ ) {
		data[ i ] = sigFile.data( i ).arg();
	}

	for( uint64_t i = 0; i < size - 1; i++ ) {
		Type dif = ( data[ i + 1 ] - data[ i ] ) * 0.5 * M_1_PI;
		if( dif > 0.5 ) dif--;
		if( dif < -0.5 ) dif++;
		data[ i ] = dif * fSam - fCen;

	}
	data[ size - 1 ] = 0;
	return std::move( data );
}

#endif // DEMOD_H
