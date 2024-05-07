//
// Created by gts on 07.05.2024.
//
#pragma once
#include "hackrfAPI/hackrf.h"

class HackRFBandwidths {
public:
    static unsigned int getBandwidth(unsigned int bandwidth_index);
    static unsigned int getBandwidthIndex(unsigned int bandwidth);
    static const unsigned int m_nb_bw;
    static const unsigned int m_bw_k[];
};
