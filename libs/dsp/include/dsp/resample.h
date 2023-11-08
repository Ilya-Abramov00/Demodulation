#ifndef RESAMPLE_H
#define RESAMPLE_H
#include <vector>
#include "base/complex.h"
#include <cmath>
#include <ctime>

class Resample {

public:

	template < typename Type >
	std::vector< Base::Complex< Type > > resampled( std::vector< Base::Complex< Type > > const& mData, uint32_t freqOLD, uint32_t freqNEW );
	template < typename Type >
	std::vector< Type > resampled( std::vector< Type > const& mData, uint32_t freqOLD, uint32_t freqNEW );

	template < typename Type >
	std::vector< Base::Complex< Type > > resampledRandom( std::vector< Base::Complex< Type > > const& mData, uint32_t freqOLD, uint32_t freqNEW );
	template < typename Type >
	std::vector< Type > resampledRandom( std::vector< Type > const& mData, uint32_t freqOLD, uint32_t freqNEW );

private:

};

template < typename Type >
std::vector< Type > Resample::resampled( std::vector< Type > const& mData, uint32_t freqOLD, uint32_t freqNEW ) {


	if( ( freqNEW == 0 ) || ( freqOLD == 0 ) ) {

		throw std::runtime_error( "Fd cant be equal 0" );
	}
	if( freqNEW >= freqOLD ) {
		throw std::runtime_error( "wrong new Fd" );
	}

	uint64_t size = mData.size();
	if( size == 0 ) {
		throw std::runtime_error( "size cant be equal 0" );
	}
	float k = ( float )freqOLD / freqNEW;
	if( k == 1 ) {
		throw std::runtime_error( "no resample with these frequencies" );
	}
	float maxW = 0;
	uint32_t maxQ = 0;
	float Wy = 0;

	uint64_t sizere = floor( size / k );
	std::vector< Type > res_data( sizere );
	if( k > size ) {
		throw std::runtime_error( "data length is too short" );
	}


	for( uint32_t q = 0; q < k; q++ ) {

		Wy = 0;

		for( uint32_t i = 0; i < ( sizere ); i++ ) {

			res_data[ i ] = mData[ i * k + q ];
			Wy = Wy + pow( res_data[ i ], 2 );
		}
		if( Wy > maxW ) {
			maxW = Wy;
			maxQ = q;
		}

	}
	for( uint32_t i = 0; i < ( sizere ); i++ ) {

		res_data[ i ] = mData[ i * k + maxQ ];

	}

	return res_data;
}

template < typename Type >
std::vector< Base::Complex< Type > > Resample::resampled( std::vector< Base::Complex< Type > > const& mData, uint32_t freqOLD, uint32_t freqNEW ) {
	std::cout << "old: " << freqOLD << '\t' << "new: " << freqNEW << std::endl;

	if( ( freqNEW == 0 ) || ( freqOLD == 0 ) ) {

		throw std::runtime_error( "Fd cant be equal 0" );
	}
	if( freqNEW > freqOLD ) {
		throw std::runtime_error( "wrong new Fd" );
	}

	uint64_t size = mData.size();
	if( size == 0 ) {
		throw std::runtime_error( "size cant be equal 0" );
	}
	float k = ( float )freqOLD / freqNEW;
	if( k == 1 ) {
		throw std::runtime_error( "no resample with these frequencies" );
	}
	double maxW = 0;
	uint32_t maxQ = 0;
	double Wy = 0;
	uint64_t sizere = floor( size / k );
	if( k > size ) {
		throw std::runtime_error( "data length is too short" );
	}

	std::vector< Base::Complex< Type > > res_data( sizere );


	for( uint32_t q = 0; q < k; q++ ) {

		Wy = 0;

		for( uint32_t mi = 0; mi < ( sizere ); mi++ ) {

			res_data[ mi ] = mData[ mi * k + q ];
			Wy += res_data[ mi ].abs() * res_data[ mi ].abs();
		}
		if( Wy > maxW ) {
			maxW = Wy;
			maxQ = q;
		}

	}
	for( uint32_t i = 0; i < ( sizere ); i++ ) {

		res_data[ i ] = mData[ i * k + maxQ ];

	}

	return res_data;
}


template < typename Type >
std::vector< Type > Resample::resampledRandom( std::vector< Type > const& mData, uint32_t freqOLD, uint32_t freqNEW ) {
	if( ( freqNEW == 0 ) || ( freqOLD == 0 ) ) {
		throw std::runtime_error( "Fd cant be equal 0" );
	}
	if( freqNEW >= freqOLD ) {
		throw std::runtime_error( "wrong new Fd" );
	}

	uint64_t size = mData.size();
	if( size == 0 ) {
		throw std::runtime_error( "size cant be equal 0" );
	}
	float k = ( float )freqOLD / freqNEW;
	if( k == 1 ) {
		throw std::runtime_error( "no resample with these frequencies" );
	}

	uint64_t sizere = floor( size / k );
	std::vector< Type > res_data( sizere );
	srand( time( NULL ) );
	if( k > size ) {
		throw std::runtime_error( "data length is too short" );
	}

	uint32_t ires = 0;
	for( uint32_t i = 0; i < size; i++ ) {
		if( ( ( sizere - ires ) == ( size - i ) ) || ( ( float )( rand() ) / RAND_MAX < 1 / k ) ) {
			res_data[ ires ] = mData[ i ];
			ires++;
			if( ires == sizere ) break;
		}
	}

	return res_data;
}

template < typename Type >
std::vector< Base::Complex< Type > > Resample::resampledRandom( std::vector< Base::Complex< Type > > const& mData, uint32_t freqOLD, uint32_t freqNEW ) {
	std::cout << "old: " << freqOLD << '\t' << "new: " << freqNEW << std::endl;
	if( ( freqNEW == 0 ) || ( freqOLD == 0 ) ) {
		throw std::runtime_error( "Fd cant be equal 0" );
	}
	if( freqNEW >= freqOLD ) {
		throw std::runtime_error( "wrong new Fd" );
	}

	uint64_t size = mData.size();
	if( size == 0 ) {
		throw std::runtime_error( "size cant be equal 0" );
	}
	float k = ( float )freqOLD / freqNEW;
	if( k == 1 ) {
		throw std::runtime_error( "no resample with these frequencies" );
	}

	uint64_t sizere = floor( size / k );
	std::vector< Base::Complex< Type > > res_data( sizere );
	srand( time( NULL ) );
	if( k > size ) {
		throw std::runtime_error( "data length is too short" );
	}

	uint32_t ires = 0;
	for( uint32_t i = 0; i < size; i++ ) {
		if( ( ( float )( rand() ) / RAND_MAX < 1 / k ) || ( ( sizere - ires ) == ( size - i ) ) ) {
			res_data[ ires ] = mData[ i ];
			ires++;
			if( ires == sizere ) break;
		}
	}

	return res_data;
}
#endif // RESAMPLE_H
