#include "base/complex.h"
#include "base/moveFreq.h"
#include "base/writeWAV.h"
#include "demodulation/demod.h"
#include "dsp/filter.h"
#include "dsp/resample.h"
#include "receiver/receiverfactory.h"
#include "rtl-sdr.h"

#include <cmath>
#include <iostream>

struct InputParams {
    InputParams() = default;
    uint32_t Fc   = 0;
    uint32_t Fw   = 0;
    uint32_t time = 0;
};
std::ostream& operator<<(std::ostream& os, InputParams const& m) {
    return os << "\nВходные параметры \n"
              << "Fc= " << m.Fc << std::endl
              << "Fw= " << m.Fw << std::endl
              << "time= " << m.time << std::endl;
}
uint32_t getNumArg(char* exArg) {
    uint32_t param = 0;
    if(exArg != nullptr) {
        uint16_t numSize = 0;
        for(numSize = 0; exArg[numSize] != 0; numSize++) { }
        for(uint16_t i = 0; i < numSize; i++) {
            param += (exArg[i] - 48) * pow(10, numSize - 1 - i);
        }
    }
    return param;
} // эта функция преобразует С-строку в число, так как входные аргументы передаются в виде массива строк

InputParams initialization(int argc, char** argv) {
    uint32_t Fc   = 0;
    uint32_t Fw   = 0;
    uint32_t time = 0;
    switch(argc) {
        case 4:
            time = getNumArg(argv[3]);
            __attribute__((fallthrough));
        case 3:
            Fw = getNumArg(argv[2]);
            __attribute__((fallthrough));
        case 2:
            Fc = getNumArg(argv[1]);
            __attribute__((fallthrough));
        case 1:
            break;
    }
    return {Fc, Fw, time};
}

void processing(InputParams&& inputParams) { }

int main(int argc, char** argv) {
    processing(initialization(argc, argv));
}