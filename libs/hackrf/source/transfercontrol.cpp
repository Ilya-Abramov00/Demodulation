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
    auto rc = static_cast<hackrf_error>(hackrf_set_tx_underrun_limit(_dev, 2));

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
    check();
    startWaitMutex.lock();
    run();
   // while(!needProcessing)
        //startWaiter.wait(&std::unique_lock<std::mutex>(startWaitMutex, 100);

    startWaitMutex.unlock();
}

void HackRFTransferControl::run() {
    check();
    needProcessing = true;

    auto rc = static_cast<hackrf_error>(hackrf_start_rx(dev, this->callback, this));

    if(rc != HACKRF_SUCCESS) {
        std::cerr << "Failed to start TX streaming: error " << (int)rc << " " << hackrf_error_name(rc);
        hackrf_close(dev);
        exit(-1);
    }

    startWaiter.notify_all();

    if(hackrf_is_streaming(dev) == HACKRF_TRUE) {
        std::cerr << "HackRFInputThread::run: HackRF is streaming already" << std::endl;
    } else {
        std::cerr << "HackRFInputThread::run: HackRF is not streaming" << std ::endl;

        auto rc = static_cast<hackrf_error>(hackrf_start_rx(dev, this->callback, this));

        if(rc == HACKRF_SUCCESS) {
            std::cout << "HackRFOutputThread::run: started HackRF Rx" << std::endl;
        } else {
            std::cerr << "HackRFOutputThread::run: failed to start HackRF Rx: " << hackrf_error_name(rc) << std::endl;
        }
    }

    while((needProcessing) && (hackrf_is_streaming(dev) == HACKRF_TRUE)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    if(hackrf_is_streaming(dev) == HACKRF_TRUE) {
        rc = static_cast<hackrf_error>(hackrf_stop_rx(dev));

        if(rc == HACKRF_SUCCESS) {
            std::cout << "HackRFOutputThread::run: stopped HackRF Rx" << std::endl;
        } else {
            std::cerr << "HackRFOutputThread::run: failed to stop HackRF Rx: " << hackrf_error_name(rc) << std::endl;
        }
    }

    needProcessing = false;
}

void HackRFTransferControl::stop() {
    std::cout << "HackRFOutputThread::stopWork" << std::endl;
    needProcessing = false;
    //startWaiter.wait(startWaitMutex);
}

void HackRFTransferControl::setCallBack(Handler f) {
    process = std::move(f);
}

void HackRFTransferControl::setTransferParams(TransferParams setting) {
    params = std::move(setting);
}

void HackRFTransferControl::check() {
    if(!process) {
        printf("callback no set");
        exit(-1);
    }

    callback = [](hackrf_transfer* transfer) -> int {
        auto* obj = static_cast<HackRFTransferControl*>(transfer->rx_ctx);
        obj->process(transfer->buffer, transfer->valid_length);

        if(obj->params.typeTransaction == TypeTransaction::single) {
            return 1;
        } else {
            return 0;
        }
    };

    if(needProcessing) {
        printf("Failed to Start RX. Stream already running.");
        exit(-1);
    }
}
