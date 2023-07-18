#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include <cstdint>
#include "../../libs/Base/include/Base/complex.h"
#include "fftw.h"


/**
 * @brief Filt фильтрация комплексного спектра на 0 частоте
 * @param data вектор отсчётов сигнала
 * @param Fs частота дискретизации
 * @param Fc центральная частота фильтрации
 * @param Fw ширина полосы фильтрации
 */


class Filter {
public:
	template < typename Type >
	std::vector< Complex< Type > > Filt( std::vector< Complex< Type > > data, uint32_t Fs, uint64_t Fc, uint64_t Fw );
private:
};

template < typename Type >
std::vector< Complex< Type > > Filter::Filt( std::vector< Complex< Type > > data, uint32_t Fs, uint64_t Fc, uint64_t Fw ) {
	if( Fw >= Fs ) return data;
	Fftw fft;
	data = fft.Forward( data );
	uint64_t size = data.size();
	uint64_t Fh = Fc + Fw / 2;
	uint64_t Fl = Fc + Fs - Fw / 2;
	Fh %= Fs;
	Fl %= Fs;
	uint64_t fh = Fh * size / Fs;
	uint64_t fl = Fl * size / Fs;
	if( Fw < Fs ) {
		if( fh < fl ) {
			for( uint64_t i = fh; i < fl + 1; i++ ) {
				data[ i ].re() = 0;
				data[ i ].im() = 0;
			}
		} else {
			for( uint64_t i = 0; i < fl + 1; i++ ) {
				data[ i ].re() = 0;
				data[ i ].im() = 0;
			}
			for( uint64_t i = fh; i < size; i++ ) {
				data[ i ].re() = 0;
				data[ i ].im() = 0;
			}
		}
	}
	data = fft.Backward( data );
	return data;
}


#endif