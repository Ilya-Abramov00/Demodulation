#include <gtest/gtest.h>
#include "../include/Demodulation/fftw.h"
#include "../libs/Base/include/Base/writer.h"
#include <vector>
#include <fstream>


TEST( TestFftw, fftwForwardFine ) {
	Fftw fftw;
	uint64_t N = 2000;

	std::vector< Complex< float > > data_( N );
	std::vector< Complex< float > > data( N );

	std::ifstream file( "dat/testDataFftwF", std::fstream::binary );
	for( uint64_t i = 0; i < N; i++ ) {
		file.read( reinterpret_cast< char* >( &data_[ i ].re() ), sizeof( float ) );
		file.read( reinterpret_cast< char* >( &data_[ i ].im() ), sizeof( float ) );
	}
	for( uint64_t i = 0; i < N; i++ ) {
		file.read( reinterpret_cast< char* >( &data[ i ].re() ), sizeof( float ) );
		file.read( reinterpret_cast< char* >( &data[ i ].im() ), sizeof( float ) );
	}

	file.close();

	data_ = fftw.Forward< float >( data_ );

	for( uint64_t i = 0; i < N; i++ ) {
		ASSERT_NEAR( data_[ i ].re(), data[ i ].re(), 1e-4 );
		ASSERT_NEAR( data_[ i ].im(), data[ i ].im(), 1e-4 );
	}
}

TEST( TestFftw, fftwBackwardFine ) {
	Fftw fftw;
	uint64_t N = 2000;

	std::vector< Complex< float > > data_( N );
	std::vector< Complex< float > > data( N );

	std::ifstream file( "dat/testDataFftwB", std::fstream::binary );
	for( uint64_t i = 0; i < N; i++ ) {
		file.read( reinterpret_cast< char* >( &data_[ i ].re() ), sizeof( float ) );
		file.read( reinterpret_cast< char* >( &data_[ i ].im() ), sizeof( float ) );
	}
	for( uint64_t i = 0; i < N; i++ ) {
		file.read( reinterpret_cast< char* >( &data[ i ].re() ), sizeof( float ) );
		file.read( reinterpret_cast< char* >( &data[ i ].im() ), sizeof( float ) );
	}

	file.close();

	data_ = fftw.Backward< float >( data_ );

	Writer Write;

	std::vector< float > newVec( 2 * N );
	for( uint64_t i = 0; i < N; i++ ) {
		newVec[ 2 * i ] = data_[ i ].re();
		newVec[ 2 * i + 1 ] = data_[ i ].im();
	}

	Write.writeFile< float >( "/home/maksim/work/w_ml/testing.mp3", newVec, 1 );



	for( uint64_t i = 0; i < N; i++ ) {
		ASSERT_NEAR( data_[ i ].re(), data[ i ].re(), 1e-4 );
		ASSERT_NEAR( data_[ i ].im(), data[ i ].im(), 1e-4 );
	}
}