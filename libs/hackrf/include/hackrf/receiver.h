//
// Created by gts on 06.05.2024.
//

#pragma once

#include "hackrfAPI/hackrf.h"

#include <list>
class HackRFReceiver {
public:
    explicit HackRFReceiver(hackrf_device* dev);

    void setFrequency(uint64_t freg);
    void setGain(float gain);
    void setGainTxvga(float gain);
    void setAMP(bool enableamp);
    void setSampleRate(uint32_t sample_rate);
    void setBasebandFilterBandwidth(float hz);

private:
    hackrf_device* dev;

    uint32_t getBandwidth(unsigned int bandwidth);
};
