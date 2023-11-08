#include <gtest/gtest.h>
#include "demodulation/demod.h"
#include "base/reader.h"
#include "base/moveFreq.h"
#include <vector>
#include <fstream>

using namespace Base;

TEST( TestDemodFM, DemodFine ) {
	Reader fileFM;
	SignalFile< float > fileM = fileFM.readFile< float >( "dat/testDataFM" );
	Demodulation demod;

	uint64_t size = fileM.size();
	std::vector< float > data_( size );

	if( fileM.freqCenter() ) {
		MoveFreq freqMv;
		std::vector< Complex< float > > mData = freqMv.ToZero< float >( fileM );
		data_ = demod.demodFM< float >( mData ); // vector
	} else {
		data_ = demod.demodFM< float >( fileM.data() ); // vector
	}
	std::ifstream file( "dat/testDataDFM", std::fstream::binary );
	std::vector< float > data( size );
	for( uint64_t i = 0; i < size; i++ ) {
		file.read( reinterpret_cast< char* >( &data[ i ] ), sizeof( float ) );
	}
	file.close();

	for( uint64_t i = 5; i < size; i++ ) {
		ASSERT_NEAR( data_[ i ], data[ i ], 1e-4 );
	}
}

