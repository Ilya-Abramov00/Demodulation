//
// Created by gts on 06.05.2024.
//
#include "hackrf/transfercontrol.h"

#include <cstdio>
#include <cstdlib>
HackRFTransferControl::HackRFTransferControl(hackrf_device* _dev) {
    if(!_dev) {
        printf("dev empty");
    }
    dev = _dev;
}

void HackRFTransferControl::start() {

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
        printf("Failed to Start TX. Stream already running.");
        exit(-1);
    }

    int ret = hackrf_start_rx(dev, this->callback, this);

    if(ret != HACKRF_SUCCESS) {
        printf("Failed to start TX streaming");
        hackrf_close(dev);
        exit(-1);
    }

    needProcessing = true;
}

void HackRFTransferControl::stop() {
    if(!needProcessing)
        return;

    int ret = hackrf_stop_rx(dev);
    if(ret != HACKRF_SUCCESS) {
        printf("Failed to stop TX streaming");
        hackrf_close(dev);
        exit(-1);
    }

    needProcessing = false;
}

void HackRFTransferControl::setCallBack(Handler f) {
    process = std::move(f);
}
void HackRFTransferControl::setTransferParams(TransferParams setting) {
    params = std::move(setting);
}
