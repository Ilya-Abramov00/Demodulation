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
    void setGainVga(float gain);
    void setGainTxvga(float gain);
    void setAMPGain(bool enableamp);

    void setSampleRate(uint64_t sample_rate);
    void setBasebandFilterBandwidth(uint64_t hz);

private:
    hackrf_device* dev;

    static uint32_t getBandwidth(unsigned int bandwidth);
};
