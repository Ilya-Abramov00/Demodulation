
#include "hackrf/dev.h"
#include "hackrf/receiver.h"
#include "hackrf/transfercontrol.h"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <unistd.h>

struct InputParams {
    InputParams() = default;
    uint32_t Fc   = 0;
    uint32_t Fw   = 0;
    uint32_t time = 0;
};
std::ostream& operator<<(std::ostream& os, InputParams const& m) {
    return os << "\nВходные параметры \n"
              << "Fc= " << m.Fc / 1'000'000 << " MГц" << std::endl
              << "Fd= " << m.Fw / 1000 << " кГц" << std::endl
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
            Fw = getNumArg(argv[2]) * 1'000;
            __attribute__((fallthrough));
        case 2:
            Fc = getNumArg(argv[1]) * 1'000'000;
            __attribute__((fallthrough));
        case 1:
            break;
    }
    return {Fc, Fw, time};
}

volatile int should_stop = 0;

pthread_cond_t cond   = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void flush_callback(hackrf_transfer* transfer) {
    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

void processing(InputParams&& inputParams) {
    std::cerr << inputParams;

    HackRFDevice dev;
    auto _dev = dev.getDev();

    HackRFReceiver receiver(_dev);
    receiver.setFrequency(inputParams.Fc);
    receiver.setSampleRate(inputParams.Fw);
    receiver.setGain(0);
    receiver.setGainTxvga(0);
    receiver.setAMP(true);
    receiver.setBasebandFilterBandwidth(inputParams.Fw);

    HackRFTransferControl transferControl(_dev);
    transferControl.setCallBack([](void* data, uint32_t size) -> void {
        std::cerr << "CALL" << std::endl;

        std::string dir = "rawDumpsSignal";
        std::filesystem::create_directory(dir);
        auto filename = dir + "/stream:" + ".bin";
        std::ofstream file(filename);
        file.write((const char*)data, size);
    });

    TransferParams params;
    params.typeTransaction = TypeTransaction::loop;

    transferControl.setTransferParams(params);
    transferControl.start();
    sleep(1);
    transferControl.stop();

    // hackrf_set_tx_underrun_limit(device, 100000); // new-ish library function, not always available
    // hackrf_enable_tx_flush(device, (hackrf_flush_cb_fn)flush_callback, NULL);
    //  hackrf_start_rx(device, transfer_callback, NULL);

    // pthread_mutex_lock(&mutex);
    //  pthread_cond_wait(&cond, &mutex); // wait fo transfer to complete
    // sleep(1);
    // std::cerr << "stop player";
    // hackrf_stop_tx(device);
    // hackrf_close(device);
    //  hackrf_exit();*/
}

int main(int argc, char** argv) {
    processing(initialization(argc, argv));
}