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
    receiver.setFrequency(200e6);
    receiver.setSampleRate(8e6);
    receiver.setGain(0);
    receiver.setGainTxvga(0);
    receiver.setAMPGain(false);
    // receiver.setBasebandFilterBandwidth(10e6);

    HackRFTransferControl transferControl(_dev);
    auto zise = transferControl.getPacketSize();
    transferControl.setCallBack([](void* data, uint32_t size) -> void { std::cerr << "CALLBACk"; });

    TransferParams params;

    params.packetCount     = 1024 * 1024 * 10 / zise;
    params.typeTransaction = TypeTransaction::single;

    transferControl.setTransferParams(params);

    auto start = std::chrono::steady_clock::now();
    transferControl.start();
    transferControl.stop();
    auto end = std::chrono::steady_clock::now();

    std::cout << "разница по времени между callback "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
}
