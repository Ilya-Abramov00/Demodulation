#include <gtest/gtest.h>
#include "../include/Demodulation/demod.h"
#include "../libs/Base/include/Base/reader.h"
#include <vector>
#include <fstream>

TEST( demodAM_test, output_values ) {

	Reader reader;
	SignalFile< double > fileM = reader.readFile< double >( "dat/testDataAM" ); // что сравнить
	Demodulation test_obj;
	std::vector< double > data = test_obj.demodAM( fileM ); // vector for compare

// TEST_deAM
	std::ifstream file( "dat/testDataDAM", std::fstream::binary ); // с чем сравнить
	uint64_t size = data.size();
	std::vector< double > ex_data( size );
	for( uint64_t i = 0; i < size; i++ ) {
		file.read( reinterpret_cast< char* >( &ex_data[ i ] ), sizeof( double ) );
	}
	file.close();

	for( uint64_t i = 0; i < data.size(); ++i ) {
		ASSERT_EQ( data[ i ], ex_data[ i ] );
	}


}
