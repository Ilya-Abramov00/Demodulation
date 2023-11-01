#pragma once

#include "Base/complex.h"
#include "fftw3.h"
#include <vector>
#include <algorithm>

const int REAL=0;
const int IMAG=1;

using namespace Base;

//void fft(fftw_complex *in, fftw_complex *out,int N);
void fft(std::vector<Complex<double>> &in, std::vector<Complex<double>> &out,int N);
void ifft(fftw_complex *in, fftw_complex *out, int N);

template < typename T >
void ComplexForFFTW_Complex(fftw_complex* sFFTW, std::vector<Complex<T>>& sComplex)
{
    for (size_t i = 0;i < sComplex.size(); i++)
    {
        sFFTW[i][0] = sComplex[i].real();
        sFFTW[i][1] = sComplex[i].imag();
    }
}
template<typename T>
void FFTW_ComplexForComplex(fftw_complex* sFFTW,std::vector<Complex<T>>& sComplex)
{
    for (size_t i=0;i<sComplex.size();i++)
    {
        Complex<T> t(sFFTW[i][0],sFFTW[i][1]);
        sComplex[i] = t;
    }
}
void fftShift(std::vector<Complex<double>> &data);

class Fftw {
public:
    template < typename Type >
    void Forward( Base::Complex< Type >* dataIn, Base::Complex< Type >* dataOut, uint32_t N );
    template < typename Type >
    void Backward( Base::Complex< Type >* dataIn, Base::Complex< Type >* dataOut, uint32_t N );
private:
};

template < typename Type >
void Fftw::Forward( Base::Complex< Type >* dataIn, Base::Complex< Type >* dataOut, uint32_t N ) {
    fftw_complex* data;
    fftw_plan p;
    data = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * N );
    for( uint64_t i = 0; i < N; i++ ) {
        data[ i ][ 0 ] = dataIn[ i ].re;
        data[ i ][ 1 ] = dataIn[ i ].im;
    }
    p = fftw_plan_dft_1d( N, data, data, FFTW_FORWARD, FFTW_ESTIMATE );
    fftw_execute( p );
    for( uint64_t i = 0; i < N; i++ ) {
        dataOut[ i ].re = data[ i ][ 0 ];
        dataOut[ i ].im = data[ i ][ 1 ];
    }
}

template < typename Type >
void Fftw::Backward( Base::Complex< Type >* dataIn, Base::Complex< Type >* dataOut, uint32_t N ) {
    fftw_complex* data;
    fftw_plan p;
    data = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * N );
    for( uint64_t i = 0; i < N; i++ ) {
        data[ i ][ 0 ] = dataIn[ i ].re;
        data[ i ][ 1 ] = dataIn[ i ].im;
    }
    p = fftw_plan_dft_1d( N, data, data, FFTW_BACKWARD, FFTW_ESTIMATE );
    fftw_execute( p );
    for( uint64_t i = 0; i < N; i++ ) {
        dataOut[ i ].re = data[ i ][ 0 ] / N;
        dataOut[ i ].im = data[ i ][ 1 ] / N;
    }
}

