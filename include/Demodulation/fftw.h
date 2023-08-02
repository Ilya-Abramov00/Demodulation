#ifndef FFTW_H
#define FFTW_H

#include <vector>
#include <cstdint>
#include <fftw3.h>
#include "../../libs/Base/include/Base/complex.h"

class Fftw {
public:
	template < typename Type >
	void Forward( Base::Complex< Type >* dataIn, Base::Complex< Type >* dataOut, uint32_t N );
	template < typename Type >
	void Backward( Base::Complex< Type >* dataIn, Base::Complex< Type >* dataOut, uint32_t N );
private:
};

template < typename Type >
void Fftw::Forward( Base::Complex< Type >* dataIn, Base::Complex< Type >* dataOut, uint32_t N ) {
	fftw_complex* data;
	fftw_plan p;
	data = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * N );
	for( uint64_t i = 0; i < N; i++ ) {
		data[ i ][ 0 ] = dataIn[ i ].re();
		data[ i ][ 1 ] = dataIn[ i ].im();
	}
	p = fftw_plan_dft_1d( N, data, data, FFTW_FORWARD, FFTW_ESTIMATE );
	fftw_execute( p );
	for( uint64_t i = 0; i < N; i++ ) {
		dataOut[ i ].re() = data[ i ][ 0 ];
		dataOut[ i ].im() = data[ i ][ 1 ];
	}
}

template < typename Type >
void Fftw::Backward( Base::Complex< Type >* dataIn, Base::Complex< Type >* dataOut, uint32_t N ) {
	fftw_complex* data;
	fftw_plan p;
	data = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * N );
	for( uint64_t i = 0; i < N; i++ ) {
		data[ i ][ 0 ] = dataIn[ i ].re();
		data[ i ][ 1 ] = dataIn[ i ].im();
	}
	p = fftw_plan_dft_1d( N, data, data, FFTW_BACKWARD, FFTW_ESTIMATE );
	fftw_execute( p );
	for( uint64_t i = 0; i < N; i++ ) {
		dataOut[ i ].re() = data[ i ][ 0 ] / N;
		dataOut[ i ].im() = data[ i ][ 1 ] / N;
	}
}

#endif