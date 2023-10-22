#include <gtest/gtest.h>
#include "../include/Demodulation/demod.h"
#include "../libs/Base/include/Base/reader.h"
#include "../libs/Base/include/Base/moveFreq.h"
#include <vector>
#include <fstream>

using namespace Base;

TEST( demodAM_test, output_values ) {

	Reader reader;
	SignalFile< float > fileM = reader.readFile< float >( "dat/testDataAM" ); // что сравнить
	Demodulation test_obj;

	uint64_t size = fileM.size();
	std::vector< float > data_( size );

	if( fileM.freqCenter() ) {
		MoveFreq freqMv;
		std::vector< Complex< float > > mData = freqMv.ToZero< float >( fileM );
		data_ = test_obj.demodAM< float >( mData ); // vector for compare
	} else {
		data_ = test_obj.demodAM< float >( fileM.data() ); // vector for compare
	}

// TEST_deAM
	std::ifstream file( "dat/testDataDAM", std::fstream::binary ); // с чем сравнить

	std::vector< float > data( size );
	for( uint64_t i = 0; i < size; i++ ) {
		file.read( reinterpret_cast< char* >( &data[ i ] ), sizeof( float ) );
	}
	file.close();

	for( uint64_t i = 0; i < data.size(); ++i ) {
		ASSERT_NEAR( data_[ i ], data[ i ], 1e-4 );
	}
}
