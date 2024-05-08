#pragma once

#include "hackrfAPI/hackrf.h"
#include "platform_SDR/transferparams.h"

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
class HackRFTransferControl {

public:
    ~HackRFTransferControl();
    using Handler = std::function<void(void* ptr, std::size_t sz)>;

    explicit HackRFTransferControl(hackrf_device* dev);

    void start();
    void stop();

    std::size_t getPacketSize() const;

    void setCallBack(Handler f);
    void setTransferParams(TransferParams setting);

private:
    bool flagStop();
    void run();

    std::size_t counter{0};

    hackrf_device* dev{nullptr};

    hackrf_sample_block_cb_fn callback{nullptr};
    Handler process;

    std::atomic_bool needProcessing{false};

    TransferParams params;

    std::unique_ptr<std::thread> thread;
};
