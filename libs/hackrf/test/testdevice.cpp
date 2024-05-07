#include "hackrf/dev.h"
#include "hackrf/receiver.h"
#include "hackrf/transfercontrol.h"
#include "hackrf/utils.h"

#include <cmath>
#include <fstream>
#include <gtest/gtest.h>
#include <pthread.h>
#include <unistd.h>

class HACKRFDeviceTest : public testing::Test {
public:
};

TEST_F(HACKRFDeviceTest, Device_creating) {
    ASSERT_EQ(1, deviceHackRfSearch());

    HackRFDevice dev(0);
    auto _dev = dev.getDev();

    HackRFReceiver receiver(_dev);
    receiver.setFrequency(10e6);
    receiver.setSampleRate(10e6);
    receiver.setGain(0);
    receiver.setGainTxvga(0);
    receiver.setAMPGain(false);
    receiver.setBasebandFilterBandwidth(10e6);

    HackRFTransferControl transferControl(_dev);
    transferControl.setCallBack([](void* data, uint32_t size) -> void {
        std::cerr << "CALL" << std::endl;

        std::string dir = "rawDumpsSignal";
        auto filename   = dir + "/stream:" + ".bin";
        std::ofstream file(filename);
        file.write((const char*)data, size);
    });

    TransferParams params;
    params.typeTransaction = TypeTransaction::single;

    transferControl.setTransferParams(params);
    transferControl.start();
    transferControl.stop();
}
