//
// Created by gts on 06.05.2024.
//
#include "hackrf/receiver.h"

#include <cstddef>
#include <cstdio>
#include <iostream>

static constexpr std::size_t size = 16;
static constexpr uint32_t bandwidthsKHz[size]
    = {1'750'000, 2'500'000,  3'500'000,  5'000'000,  5'500'000,  6'000'000,  7'000'000,  8'000'000,
       9'000'000, 10'000'000, 12'000'000, 14'000'000, 15'000'000, 20'000'000, 24'000'000, 28'000'000};

HackRFReceiver::HackRFReceiver(hackrf_device* _dev) {
    if(!_dev) {
        printf("dev empty");
    }
    dev = _dev;
}

void HackRFReceiver::setFrequency(uint64_t freg) {
    auto rc = static_cast<hackrf_error>(hackrf_set_freq(dev, freg));

    if(rc != HACKRF_SUCCESS) {
        std::cerr << "Failed " << __FUNCTION__ << (int)rc << " " << hackrf_error_name(rc);
        exit(-1);
    }
}

void HackRFReceiver::setGain(float gain) {
    auto rc = static_cast<hackrf_error>(hackrf_set_txvga_gain(dev, uint32_t(gain)));

    if(rc != HACKRF_SUCCESS) {
        std::cerr << "Failed " << __FUNCTION__ << (int)rc << " " << hackrf_error_name(rc);
        exit(-1);
    }
}
void HackRFReceiver::setGainTxvga(float gain) {
    auto rc = static_cast<hackrf_error>(hackrf_set_txvga_gain(dev, uint32_t(gain)));

    if(rc != HACKRF_SUCCESS) {
        std::cerr << "Failed " << __FUNCTION__ << (int)rc << " " << hackrf_error_name(rc);
        exit(-1);
    }
}

void HackRFReceiver::setAMP(bool enableamp) {
    auto rc = static_cast<hackrf_error>(hackrf_set_amp_enable(dev, enableamp));

    if(rc != HACKRF_SUCCESS) {
        std::cerr << "Failed " << __FUNCTION__ << (int)rc << " " << hackrf_error_name(rc);
        exit(-1);
    }
}

void HackRFReceiver::setSampleRate(uint32_t sample_rate) {
    std::cout << "set sampleRate :" << sample_rate / 1e6 << " MHz" << std::endl;

    auto rc = static_cast<hackrf_error>(hackrf_set_sample_rate(dev, sample_rate));

    if(rc != HACKRF_SUCCESS) {
        std::cerr << "Failed " << __FUNCTION__ << (int)rc << " " << hackrf_error_name(rc);
        exit(-1);
    }
}
void HackRFReceiver::setBasebandFilterBandwidth(float hz) {
    auto standart = getBandwidth(hz);
    if(standart != hz) {
        std::cerr << __FUNCTION__ << " set " << standart << " Hz" << std::endl;
    }
    auto rc = static_cast<hackrf_error>(hackrf_set_baseband_filter_bandwidth(dev, hz));

    if(rc != HACKRF_SUCCESS) {
        std::cerr << "Failed " << __FUNCTION__ << (int)rc << " " << hackrf_error_name(rc);
        exit(-1);
    }
}
uint32_t HackRFReceiver::getBandwidth(unsigned int bandwidthHz) {
    for(std::size_t i = 0; i < size; i++) {
        if(bandwidthsKHz[i] >= bandwidthHz) {
            return bandwidthsKHz[i];
        }
    }
    return bandwidthsKHz[size - 1];
}
