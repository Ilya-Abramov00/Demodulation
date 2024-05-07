//
// Created by gts on 06.05.2024.
//
#include "hackrf/dev.h"

#include <cstddef>
#include <cstdio>

HackRFDevice::HackRFDevice() : dev(nullptr) {
    open();
}
hackrf_device* HackRFDevice::getDev() {
    return dev;
}

HackRFDevice::~HackRFDevice() {
    if(dev) {
        this->close();
    }
}

bool HackRFDevice::open() {
    hackrf_init();

    int ret = hackrf_open(&dev);
    if(ret != HACKRF_SUCCESS) {
        printf("Failed to open HackRF device");
        hackrf_close(dev);
        return false;
    }
    return true;
}

void HackRFDevice::close() {
    // hackrf_stop_tx(_dev);
    hackrf_close(dev);
    hackrf_exit();
    dev = nullptr;
}
