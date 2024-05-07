//
// Created by gts on 06.05.2024.
//

#pragma once

#include "hackrfAPI/hackrf.h"

class HackRFDevice {
public:
    HackRFDevice(int sequence);

    hackrf_device* getDev();
    ~HackRFDevice();

private:
    [[nodiscard]] hackrf_device* open(int sequence);
    [[nodiscard]] hackrf_device* open(const char* const serial);
    void close();
    hackrf_device* dev;
};
