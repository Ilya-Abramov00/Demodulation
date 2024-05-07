//
// Created by gts on 07.05.2024.
//

#include "hackrf/tools.h"

const unsigned int HackRFBandwidths::m_nb_bw = 16;
const unsigned int HackRFBandwidths::m_bw_k[HackRFBandwidths::m_nb_bw] = {
    1750,
    2500,
    3500,
    5000,
    5500,
    6000,
    7000,
    8000,
    9000,
   10000,
   12000,
   14000,
   15000,
   20000,
   24000,
   28000};

unsigned int HackRFBandwidths::getBandwidth(unsigned int bandwidth_index)
{
    if (bandwidth_index < m_nb_bw)
    {
        return m_bw_k[bandwidth_index];
    }
    else
    {
        return m_bw_k[0];
    }
}

unsigned int HackRFBandwidths::getBandwidthIndex(unsigned int bandwidth)
{
    for (unsigned int i=0; i < m_nb_bw; i++)
    {
        if (bandwidth == m_bw_k[i])
        {
            return i;
        }
    }

    return 0;
}