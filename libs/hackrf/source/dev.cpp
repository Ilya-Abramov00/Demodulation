//
// Created by gts on 06.05.2024.
//
#include "hackrf/dev.h"

#include <iostream>

HackRFDevice::HackRFDevice(size_t sequence) : dev(nullptr) {
    auto rc = static_cast<hackrf_error>(hackrf_init());

    if(rc != HACKRF_SUCCESS) {
        std::cerr << "DeviceHackRF::open_hackrf: failed to initiate HackRF library " << hackrf_error_name(rc);
    }
    dev = open(static_cast<int>(sequence));
}
hackrf_device* HackRFDevice::getDev() {
    return dev;
}

HackRFDevice::~HackRFDevice() {
    hackrf_close(dev);
    hackrf_exit();
}

hackrf_device* HackRFDevice::open(int sequence) {
    auto* hackrf_devices = hackrf_device_list();

    if(hackrf_devices == nullptr) {
        std::cerr << "fail";
        exit(-1);
    }
    auto id = *hackrf_devices->usb_board_ids;

    hackrf_device* hackrf_ptr;

    auto rc = static_cast<hackrf_error>(hackrf_device_list_open(hackrf_devices, sequence, &hackrf_ptr));
    hackrf_device_list_free(hackrf_devices);

    if(rc == HACKRF_SUCCESS) {
        std::cout << "open " << hackrf_usb_board_id_name(id) << std::endl;
        return hackrf_ptr;
    } else {
        std::cerr << "DeviceHackRF::open: error " << (int)rc << " " << hackrf_error_name(rc);
        exit(-1);
    }
}

hackrf_device* HackRFDevice::open(const char* const serial) {
    hackrf_device* hackrf_ptr;

    auto rc = static_cast<hackrf_error>(hackrf_open_by_serial(serial, &hackrf_ptr));

    if(rc == HACKRF_SUCCESS) {
        return hackrf_ptr;
    } else {
        std::cerr << "DeviceHackRF::open_hackrf: error " << (int)rc << " " << hackrf_error_name(rc);
        exit(-1);
    }
}
