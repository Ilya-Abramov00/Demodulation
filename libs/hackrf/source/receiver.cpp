//
// Created by gts on 06.05.2024.
//
#include "hackrf/receiver.h"

#include <cstddef>
#include <cstdio>
#include <iostream>

static constexpr std::size_t size = 16;
static constexpr uint64_t bandwidthsKHz[size]
    = {1'750'000, 2'500'000,  3'500'000,  5'000'000,  5'500'000,  6'000'000,  7'000'000,  8'000'000,
       9'000'000, 10'000'000, 12'000'000, 14'000'000, 15'000'000, 20'000'000, 24'000'000, 28'000'000};

static constexpr uint64_t freqSampleHzMin       = 2'000'000;
static constexpr uint64_t freqSampleHzMinStable = 8'000'000;
static constexpr uint64_t freqSampleHzMax       = 20'000'000;

static constexpr uint64_t freqHzMin = 1'000'000;
static constexpr uint64_t freqHzMax = 6'000'000'000;

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
/**
 * Set LNA gain
 * Set the RF RX gain of the MAX2837 transceiver IC ("IF" gain setting) in decibels. Must be in range 0-40dB, with 8dB
 * steps.
 * @param value RX IF gain value in dB
 */
void HackRFReceiver::setGain(float gain) {
    auto rc = static_cast<hackrf_error>(hackrf_set_lna_gain(dev, uint32_t(gain)));

    if(rc != HACKRF_SUCCESS) {
        std::cerr << "Failed " << __FUNCTION__ << (int)rc << " " << hackrf_error_name(rc);
        exit(-1);
    }
}

/**
 * Set baseband RX gain of the MAX2837 transceiver IC ("BB" or "VGA" gain setting) in decibels. Must be in range 0-62dB
 * with 2dB steps.
 * @param value RX BB gain value in dB
 */

void HackRFReceiver::setGainVga(float gain) {
    auto rc = static_cast<hackrf_error>(hackrf_set_vga_gain(dev, uint32_t(gain)));

    if(rc != HACKRF_SUCCESS) {
        std::cerr << "Failed " << __FUNCTION__ << (int)rc << " " << hackrf_error_name(rc);
        exit(-1);
    }
}
/**
 * Set RF TX gain of the MAX2837 transceiver IC ("IF" or "VGA" gain setting) in decibels. Must be in range 0-47dB in 1dB
 * steps.
 * @param value TX IF gain value in dB
 */
void HackRFReceiver::setGainTxvga(float gain) {
    auto rc = static_cast<hackrf_error>(hackrf_set_txvga_gain(dev, uint32_t(gain)));

    if(rc != HACKRF_SUCCESS) {
        std::cerr << "Failed " << __FUNCTION__ << (int)rc << " " << hackrf_error_name(rc);
        exit(-1);
    }
}
/**
 * Enable/disable 14dB RF amplifier
 *
 * Enable / disable the ~11dB RF RX/TX amplifiers U13/U25 via controlling switches U9 and U14.
 * @param value enable (1) or disable (0) amplifier
 **/

void HackRFReceiver::setAMPGain(bool enableamp) {
    auto rc = static_cast<hackrf_error>(hackrf_set_amp_enable(dev, enableamp));

    if(rc != HACKRF_SUCCESS) {
        std::cerr << "Failed " << __FUNCTION__ << (int)rc << " " << hackrf_error_name(rc);
        exit(-1);
    }
}
void HackRFReceiver::setSampleRate(uint64_t sample_rate) {
    if((sample_rate < freqSampleHzMin) || (sample_rate > freqSampleHzMax)) {
        std::cerr << "Failed " << __FUNCTION__ << " " << sample_rate << " Hz" << std::endl;
     //   exit(-1);
    }

    if(sample_rate < freqSampleHzMinStable) {
        std::cerr << "WARN " << __FUNCTION__ << " " << sample_rate << " Hz" << std::endl;
    }

    std::cout << "set sampleRate :" << sample_rate / 1e6 << " MHz" << std::endl;

    auto rc = static_cast<hackrf_error>(hackrf_set_sample_rate(dev, sample_rate));

    if(rc != HACKRF_SUCCESS) {
        std::cerr << "Failed " << __FUNCTION__ << (int)rc << " " << hackrf_error_name(rc);
        exit(-1);
    }
}
void HackRFReceiver::setBasebandFilterBandwidth(uint64_t hz) {
    auto bandwidth = getBandwidth(hz);
    if(bandwidth != hz) {
        std::cerr << __FUNCTION__ << " set " << bandwidth << " Hz" << std::endl;
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
