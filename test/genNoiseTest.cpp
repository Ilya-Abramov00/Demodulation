#include <gtest/gtest.h>
#include "../include/Demodulation/fftw.h"
#include "../include/Demodulation/genNoise.h"
#include "../libs/Base/include/Base/writer.h"
#include <vector>
#include <fstream>


TEST( TestNoise, WNFine ) {
	Fftw fftw;
	GenNoise GenN;
	float W = 10;
	uint64_t N = 4096 * 16;

	std::vector< Complex< float > > data_ = GenN.GenWN< float >( W, N );

	data_ = fftw.Forward< float >( data_ );

	float avr = 0;
	for( uint64_t i = 0; i < N; i++ ) {
		avr += ( data_[ i ].abs() * data_[ i ].abs() );
	}
	avr /= N;
	ASSERT_NEAR( avr, W, W * 1e-2 );

	Writer write;
	std::vector< float > vec( 2 * N );
	for( uint64_t i = 0; i < N; i++ ) {
		vec[ 2 * i ] = data_[ i ].re();
		vec[ 2 * i + 1 ] = data_[ i ].im();
	}

	write.writeFile( "/home/maksim/work/w_ml/testNoise", vec, 1 );
}