#ifndef GENNOISE_H
#define GENNOISE_H

#include <vector>
#include <ctime>
#include <cstdint>
#include "../../libs/Base/include/Base/complex.h"

class GenNoise {
public:
	template < typename Type >
	std::vector< Base::Complex< Type > > GenWN( float W, uint64_t size );
private:
};

template < typename Type >
std::vector< Base::Complex< Type > > GenNoise::GenWN( float W, uint64_t size ) {
	srand( time( NULL ) );
	float A = sqrt( 1.5 / size ) * pow( 10, W / 20 );
	std::vector< Base::Complex< Type > > data( size );
	for( uint64_t i = 0; i < size; i++ ) {
		data[ i ].re() = ( ( float )( rand() ) / RAND_MAX * 2 - 1 ) * A;
		data[ i ].im() = ( ( float )( rand() ) / RAND_MAX * 2 - 1 ) * A;
	}
	return data;
}

#endif