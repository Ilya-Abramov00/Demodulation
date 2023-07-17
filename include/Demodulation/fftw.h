#ifndef FFTW_H
#define FFTW_H

#include <vector>
#include <cstdint>
#include <fftw3.h>
#include "../../libs/Base/include/Base/complex.h"

class Fftw {
public:
	template < typename Type >
	std::vector< Complex< Type > > Forward( std::vector< Complex< Type > > data );
	template < typename Type >
	std::vector< Complex< Type > > Backward( std::vector< Complex< Type > > data );
private:
};

template < typename Type >
std::vector< Complex< Type > > Fftw::Forward( std::vector< Complex< Type > > data ) {
	uint64_t N = data.size();
	fftw_complex* in, * out;
	fftw_plan p;
	in = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * N );
	out = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * N );
	for( uint64_t i = 0; i < N; i++ ) {
		in[ i ][ 0 ] = data[ i ].re();
		in[ i ][ 1 ] = data[ i ].im();
	}
	p = fftw_plan_dft_1d( N, in, out, FFTW_FORWARD, FFTW_ESTIMATE );
	fftw_execute( p );
	for( uint64_t i = 0; i < N; i++ ) {
		data[ i ].re() = out[ i ][ 0 ];
		data[ i ].im() = out[ i ][ 1 ];
	}
	return data;
}

template < typename Type >
std::vector< Complex< Type > > Fftw::Backward( std::vector< Complex< Type > > data ) {
	uint64_t N = data.size();
	fftw_complex* in, * out;
	fftw_plan p;
	in = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * N );
	out = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * N );
	for( uint64_t i = 0; i < N; i++ ) {
		in[ i ][ 0 ] = data[ i ].re();
		in[ i ][ 1 ] = data[ i ].im();
	}
	p = fftw_plan_dft_1d( N, in, out, FFTW_BACKWARD, FFTW_ESTIMATE );
	fftw_execute( p );
	for( uint64_t i = 0; i < N; i++ ) {
		data[ i ].re() = out[ i ][ 0 ] / N;
		data[ i ].im() = out[ i ][ 1 ] / N;
	}
	return data;
}

#endif