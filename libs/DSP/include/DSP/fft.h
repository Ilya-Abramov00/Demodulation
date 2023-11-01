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


