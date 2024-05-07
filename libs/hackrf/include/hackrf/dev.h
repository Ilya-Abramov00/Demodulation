//
// Created by gts on 06.05.2024.
//

#pragma once

#include "hackrfAPI/hackrf.h"

class HackRFDevice {
public:
    HackRFDevice();

    hackrf_device* getDev();
    ~HackRFDevice();

private:
    bool open();
    void close();

    hackrf_device* dev;
};
