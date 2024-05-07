#pragma once

#include "hackrfAPI/hackrf.h"
#include "platform_SDR/transferparams.h"

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
class HackRFTransferControl {
public:
    using Handler = std::function<void(void* ptr, std::size_t sz)>;

    explicit HackRFTransferControl(hackrf_device* dev);

    void start();
    void stop();

    void setCallBack(Handler f);
    void setTransferParams(TransferParams setting);

private:
    bool flagStop();

    std::size_t counter{0};
    std::size_t packetCount{1};

    hackrf_device* dev{nullptr};

    hackrf_sample_block_cb_fn callback{nullptr};
    Handler process;

    std::atomic_bool needProcessing{false};

    TransferParams params;
};
