//
// Created by gts on 06.05.2024.
//
#include "hackrf/transfercontrol.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
HackRFTransferControl::HackRFTransferControl(hackrf_device* _dev) {
    if(!_dev) {
        printf("dev empty");
    }
    auto rc = static_cast<hackrf_error>(hackrf_set_rx_overrun_limit(_dev, 1));

    if(rc != HACKRF_SUCCESS) {
        std::cerr << __FUNCTION__ << " : error " << (int)rc << " " << hackrf_error_name(rc);
        exit(-1);
    }
    // непонятно насколько она вообще работает
    // Установите предел пропускной способности передачи
    //
    //  Если этот предел установлен, то после пропуска указанного количества отсчетов (байт, а не целых пар IQ)
    //  устройство автоматически перейдет в режим ожидания, тем самым прекращая работу. Полезно для обработки таких
    //  случаев, как сбои программы/компьютера или другие проблемы. Значение по умолчанию 0 означает отсутствие
    //  ограничений.

    dev = _dev;
}
void HackRFTransferControl::start() {
    if(!callback) {
        printf("callback no set");
        exit(-1);
    }

    if(needProcessing) {
        printf("Failed to Start RX. Stream already running.");
        exit(-1);
    }

    needProcessing = true;

    auto rc = static_cast<hackrf_error>(hackrf_start_rx(dev, this->callback, this));

    if(rc != HACKRF_SUCCESS) {
        std::cerr << "Failed to start RX streaming: error " << (int)rc << " " << hackrf_error_name(rc);
        exit(-1);
    }
    std::cerr << "started HackRF Rx";
}
HackRFTransferControl::~HackRFTransferControl() { }
void HackRFTransferControl::run() {
    auto rc = static_cast<hackrf_error>(hackrf_start_rx(dev, this->callback, this));

    if(rc != HACKRF_SUCCESS) {
        std::cerr << "Failed to start RX streaming: error " << (int)rc << " " << hackrf_error_name(rc);
        hackrf_close(dev);
        exit(-1);
    }
    std::cout << "started HackRF Rx" << std::endl;

    while((needProcessing) && (hackrf_is_streaming(dev) == HACKRF_TRUE)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    std::cout << "Pre stop" << std::endl;
    if(hackrf_is_streaming(dev) == HACKRF_TRUE) {
        rc = static_cast<hackrf_error>(hackrf_stop_rx(dev));

        if(rc == HACKRF_SUCCESS) {
            std::cout << "stopped HackRF RX" << std::endl;
        } else {
            std::cerr << "failed to stop HackRF RX: " << hackrf_error_name(rc) << std::endl;
        }
    }
}
bool HackRFTransferControl::flagStop() {
    std::unique_lock<std::mutex> lk(mutex);
    if((params.typeTransaction == TypeTransaction::single) && (++counter == params.packetCount)) {
        stopComplite = true;
    }
    else if(params.typeTransaction == TypeTransaction::loop && needStop) {
        stopComplite = true;
    }

    lk.unlock();
    if(stopComplite) {
        cv.notify_one();
    }

    return stopComplite;
}

void HackRFTransferControl::stop() {
    std::unique_lock<std::mutex> lk(mutex);
    needStop = true;
    cv.wait(lk, [this] { return stopComplite; });

    auto rc = static_cast<hackrf_error>(hackrf_stop_rx(dev));

    if(rc != HACKRF_SUCCESS) {
        std::cerr << "failed to stop HackRF RX: " << hackrf_error_name(rc);
        exit(-1);
    }

    needProcessing = false;
    needStop       = false;
    stopComplite   = false;
    counter        = 0;
}

void HackRFTransferControl::setTransferParams(TransferParams setting) {
    params = setting;
}

void HackRFTransferControl::setCallBack(Handler f) {
    handler  = std::move(f);
    callback = [](hackrf_transfer* transfer) -> int {
        auto* obj = static_cast<HackRFTransferControl*>(transfer->rx_ctx);

        obj->handler(transfer->buffer, transfer->valid_length);

        return obj->flagStop();
    };
}
std::size_t HackRFTransferControl::getPacketSize() const {
    return hackrf_get_transfer_buffer_size(dev);
}
