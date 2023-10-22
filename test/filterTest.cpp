#include <gtest/gtest.h>
#include "../include/Demodulation/filter.h"
#include "../libs/Base/include/Base/reader.h"
#include "../libs/Base/include/Base/writer.h"
#include <vector>
#include <fstream>

using namespace Base;

TEST( TestFilter, filterFine ) {
	Reader readFile;
	Filter filter;
	SignalFile< float > fileIn = readFile.readFile< float >( "dat/testDataFilterIn" );

	uint64_t Fw = 100;
	uint64_t N = fileIn.size();
	std::vector< Complex< float > > data_( N );
	for( uint64_t i = 0; i < N; i++ ) {
		data_[ i ] = fileIn.data( i );
	}
	std::vector< Complex< float > > data( N );
	std::ifstream fileOut( "dat/testDataFilterOut", std::fstream::binary );
	for( uint64_t i = 0; i < N; i++ ) {
		fileOut.read( reinterpret_cast< char* >( &data[ i ].re() ), sizeof( float ) );
		fileOut.read( reinterpret_cast< char* >( &data[ i ].im() ), sizeof( float ) );
	}
	fileOut.close();

    filter.Filt( data_.data(), data_.data(), data_.size(), fileIn.freqSample(), fileIn.freqCenter(), Fw );

	Writer write;
	std::vector< float > vec( 2 * N );
	for( uint64_t i = 0; i < N; i++ ) {
		vec[ 2 * i ] = data_[ i ].re();
		vec[ 2 * i + 1 ] = data_[ i ].im();
	}

	write.writeFile( "/home/maksim/work/w_ml/test", vec, fileIn.freqSample() );

	for( uint64_t i = 0; i < N; i++ ) {
		ASSERT_NEAR( data_[ i ].re(), data[ i ].re(), 1e-4 );
		ASSERT_NEAR( data_[ i ].im(), data[ i ].im(), 1e-4 );
	}
}

TEST( TestFilter, filterVec1 ) {
	Filter filter;
	uint64_t N = 1;
	std::vector< Complex< float > > data_( N );
	data_[ 0 ].re() = 1;
	data_[ 0 ].im() = 0;
	uint32_t Fs = 1000;
	uint64_t Fc = 0;
	uint64_t Fw = 0;

	std::vector< Complex< float > > data( N );

    filter.Filt( data_.data(), data_.data(), data_.size(), Fs, Fc, Fw );

	ASSERT_EQ( data_[ 0 ].re(), 0 );
}

TEST( TestFilter, filterC0 ) {
	Reader readFile;
	Filter filter;
	SignalFile< float > fileIn = readFile.readFile< float >( "dat/testDataFilterInC0" );

	uint64_t Fw = 200;
	uint64_t N = fileIn.size();
	std::vector< Complex< float > > data_( N );
	for( uint64_t i = 0; i < N; i++ ) {
		data_[ i ] = fileIn.data( i );
	}
	std::vector< Complex< float > > data( N );
	std::ifstream fileOut( "dat/testDataFilterOutC0", std::fstream::binary );
	for( uint64_t i = 0; i < N; i++ ) {
		fileOut.read( reinterpret_cast< char* >( &data[ i ].re() ), sizeof( float ) );
		fileOut.read( reinterpret_cast< char* >( &data[ i ].im() ), sizeof( float ) );
	}
	fileOut.close();

    filter.Filt( data_.data(), data_.data(), data_.size(), fileIn.freqSample(), fileIn.freqCenter(), Fw );

	for( uint64_t i = 0; i < N; i++ ) {
		// if( data_[ i ].re() != data[ i ].re() ) std::cout << i << std::endl;
		ASSERT_NEAR( data_[ i ].re(), data[ i ].re(), 1e-4 );
		ASSERT_NEAR( data_[ i ].im(), data[ i ].im(), 1e-4 );
	}
}

TEST( TestFilter, filterW0 ) {
	Reader readFile;
	Filter filter;
	SignalFile< float > fileIn = readFile.readFile< float >( "dat/testDataFilterIn" );

	uint64_t Fw = 0;
	uint64_t N = fileIn.size();
	std::vector< Complex< float > > data_( N );
	for( uint64_t i = 0; i < N; i++ ) {
		data_[ i ] = fileIn.data( i );
	}

    filter.Filt( data_.data(), data_.data(), data_.size(), fileIn.freqSample(), fileIn.freqCenter(), Fw );

	for( uint64_t i = 0; i < N; i++ ) {
		ASSERT_EQ( data_[ i ].re(), 0 );
		ASSERT_EQ( data_[ i ].im(), 0 );
	}
}
