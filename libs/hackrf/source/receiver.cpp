//
// Created by gts on 06.05.2024.
//
#include "hackrf/receiver.h"

#include <cstddef>
#include <cstdio>
#include <iostream>

HackRFReceiver::HackRFReceiver(hackrf_device* _dev) {
    if(!_dev) {
        printf("dev empty");
    }
    dev = _dev;
}

void HackRFReceiver::setFrequency(uint64_t freg) {
    auto r = hackrf_set_freq(dev, freg);

    if(r) {
        std::cerr << "FAIL :" << r << " " << __FUNCTION__ << std::endl;
    }
}

void HackRFReceiver::setGain(float gain) {
    auto r = hackrf_set_txvga_gain(dev, uint32_t(gain));

    if(r) {
        std::cerr << "FAIL :" << r << " " << __FUNCTION__ << std::endl;
    }
}
void HackRFReceiver::setGainTxvga(float gain) {
    auto r = hackrf_set_txvga_gain(dev, gain);

    if(r) {
        std::cerr << "FAIL :" << r << " " << __FUNCTION__ << std::endl;
    }
}

void HackRFReceiver::setAMP(bool enableamp) {
    auto r = hackrf_set_amp_enable(dev, enableamp);

    if(r) {
        std::cerr << "FAIL :" << r << " " << __FUNCTION__ << std::endl;
    }
}

void HackRFReceiver::setSampleRate(uint32_t sample_rate) {
    auto r = hackrf_set_sample_rate(dev, sample_rate);

    if(r) {
        std::cerr << "FAIL :" << r << " " << __FUNCTION__ << std::endl;
    }
}
void HackRFReceiver::setBasebandFilterBandwidth(float hz) {
    auto r = hackrf_set_baseband_filter_bandwidth(dev, hz);

    if(r) {
        std::cerr << "FAIL :" << r << " " << __FUNCTION__ << std::endl;
    }
}
