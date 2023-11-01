#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include <cstdint>
#include "../../Base/include/Base/complex.h"
#include "fft.h"


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
	void Filt( Base::Complex< Type >* dataIn, Base::Complex< Type >* dataOut, uint32_t N, uint32_t Fs, uint64_t Fc, uint64_t Fw );
private:
};

template < typename Type >
void Filter::Filt( Base::Complex< Type >* dataIn, Base::Complex< Type >* dataOut, uint32_t N, uint32_t Fs, uint64_t Fc, uint64_t Fw ) {
	std::cout << "Fww: " << Fw  << std::endl;
	if( Fw >= Fs ) {
		for( uint32_t i = 0; i < N; i++ ) {
            dataOut[ i ].re = dataIn[ i ].re;
            dataOut[ i ].im = dataIn[ i ].im;
		}
	}
	Fftw fft;
	fft.Forward( dataIn, dataOut, N );
	uint64_t Fh = Fc + Fw / 2;
	uint64_t Fl = Fc + Fs - Fw / 2;
	Fh %= Fs;
	Fl %= Fs;
	uint64_t fh = Fh * N / Fs;
	uint64_t fl = Fl * N / Fs;
	if( Fw < Fs ) {
		if( fh < fl ) {
			for( uint64_t i = fh; i < fl + 1; i++ ) {
                dataOut[ i ].re = 0;
                dataOut[ i ].im = 0;
			}
		} else {
			for( uint64_t i = 0; i < fl + 1; i++ ) {
                dataOut[ i ].re = 0;
                dataOut[ i ].im = 0;
			}
			for( uint64_t i = fh; i < N; i++ ) {
                dataOut[ i ].re = 0;
                dataOut[ i ].im = 0;
			}
		}
	}
	fft.Backward( dataOut, dataOut, N );
}


#endif
