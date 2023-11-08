#include "dsp/fft.h"


//Функция для прямого преобразовния Фурье
//void fft(fftw_complex *in, fftw_complex *out,int N)
//{

//     fftw_plan plan=fftw_plan_dft_1d(N,in,out,FFTW_FORWARD,FFTW_ESTIMATE);
//     fftw_execute(plan);
//     fftw_destroy_plan(plan);
//     fftw_cleanup();
//}

void fft(std::vector<Complex<double>> &in, std::vector<Complex<double>> &out,int N)
{
     fftw_complex *inFFTW=(fftw_complex *)in.data();
     fftw_complex *outFFTW=(fftw_complex*)out.data();
     fftw_plan plan=fftw_plan_dft_1d(N,inFFTW,outFFTW,FFTW_FORWARD,FFTW_ESTIMATE);
     fftw_execute(plan);
     fftw_destroy_plan(plan);
     fftw_cleanup();
}


// Функция для обратного преобразовния Фурье
void ifft(fftw_complex *in, fftw_complex *out,int N)
{
    fftw_plan plan=fftw_plan_dft_1d(N,in,out,FFTW_BACKWARD,FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
    fftw_cleanup();
    for(int i = 0; i < N; ++i)
    {
        out[i][REAL] /= N;
        out[i][IMAG] /= N;
    }
}




void fftShift(std::vector<Complex<double>> &data)
{
    if(data.size() % 2 == 0)
        std::rotate(&data[0], &data[data.size() >> 1], &data[data.size()]);
    else
        std::rotate(&data[0], &data[(data.size()>>1)+1], &data[data.size()]);
}
