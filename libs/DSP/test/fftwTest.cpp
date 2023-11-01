#include <gtest/gtest.h>
#include "../include/DSP/fft.h"
#include <vector>
#include <fstream>

using namespace Base;

TEST( TestFftw, fftwForwardFine ) {
    Fftw fftw;
	uint64_t N = 2000;

	std::vector< Complex< float > > data_( N );
	std::vector< Complex< float > > data( N );

	std::ifstream fileIn( "dat/testDataFftwFin", std::fstream::binary );
	for( uint64_t i = 0; i < N; i++ ) {
        fileIn.read( reinterpret_cast< char* >( &data_[ i ].re ), sizeof( float ) );
        fileIn.read( reinterpret_cast< char* >( &data_[ i ].im ), sizeof( float ) );
	}
	fileIn.close();
	std::ifstream fileOut( "dat/testDataFftwFout", std::fstream::binary );
	for( uint64_t i = 0; i < N; i++ ) {
        fileOut.read( reinterpret_cast< char* >( &data[ i ].re ), sizeof( float ) );
        fileOut.read( reinterpret_cast< char* >( &data[ i ].im ), sizeof( float ) );
	}
	fileOut.close();

    fftw.Forward< float >( data_.data(), data_.data(), data_.size());

	for( uint64_t i = 0; i < N; i++ ) {
        ASSERT_NEAR( data_[ i ].re, data[ i ].re, 1e-4 );
        ASSERT_NEAR( data_[ i ].im, data[ i ].im, 1e-4 );
	}
}

TEST( TestFftw, fftwBackwardFine ) {
	Fftw fftw;
	uint64_t N = 2000;

	std::vector< Complex< float > > data_( N );
	std::vector< Complex< float > > data( N );

	std::ifstream fileIn( "dat/testDataFftwBin", std::fstream::binary );
	for( uint64_t i = 0; i < N; i++ ) {
        fileIn.read( reinterpret_cast< char* >( &data_[ i ].re ), sizeof( float ) );
        fileIn.read( reinterpret_cast< char* >( &data_[ i ].im ), sizeof( float ) );
	}
	fileIn.close();
	std::ifstream fileOut( "dat/testDataFftwBout", std::fstream::binary );
	for( uint64_t i = 0; i < N; i++ ) {
        fileOut.read( reinterpret_cast< char* >( &data[ i ].re ), sizeof( float ) );
        fileOut.read( reinterpret_cast< char* >( &data[ i ].im ), sizeof( float ) );
	}
	fileOut.close();

    fftw.Backward< float >( data_.data(), data_.data(), data_.size());

	for( uint64_t i = 0; i < N; i++ ) {
        ASSERT_NEAR( data_[ i ].re, data[ i ].re, 1e-4 );
        ASSERT_NEAR( data_[ i ].im, data[ i ].im, 1e-4 );
	}
}
