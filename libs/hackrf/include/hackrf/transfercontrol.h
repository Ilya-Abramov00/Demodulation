#pragma once

#include "hackrfAPI/hackrf.h"

#include <atomic>
#include <functional>
#include "platform_SDR/transferparams.h"
class HackRFTransferControl {
public:
    using Handler = std::function<void(void* ptr, std::size_t sz)>;

    explicit HackRFTransferControl(hackrf_device* dev);

    void start();
    void stop();

    void setCallBack(Handler f);
    void setTransferParams(TransferParams setting);
private:
    hackrf_device* dev{nullptr};

    hackrf_sample_block_cb_fn callback;
    Handler process;

    std::atomic_bool needProcessing{false};

    TransferParams params;
};
