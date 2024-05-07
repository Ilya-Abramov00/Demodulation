//
// Created by ilya on 07.05.2024.
//

#include "hackrf/utils.h"

#include "hackrfAPI/hackrf.h"

#include <iostream>

size_t deviceHackRfSearch() {
    auto rc = static_cast<hackrf_error>(hackrf_init());

    if(rc != HACKRF_SUCCESS) {
        std::cerr << "DeviceHackRF::open_hackrf: failed to initiate HackRF library " << hackrf_error_name(rc);
        return 0;
    }

    hackrf_device_list_t* hackrf_devices = hackrf_device_list();

    if(hackrf_devices == nullptr) {
        std::cerr << "fail";
        exit(-1);
    }
    auto deviceCount = static_cast<size_t>(hackrf_devices->devicecount);
    hackrf_device_list_free(hackrf_devices);

    return deviceCount;
}
