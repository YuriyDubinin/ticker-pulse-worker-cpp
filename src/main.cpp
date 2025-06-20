#include <chrono>
#include <fmt/core.h>
#include <thread>
#include "global_config.h"
#include "quotes_worker.h"
#include "thread_pool.h"

int main() {
    fmt::print("\n[TICKER_PULSE_WORKER]: started, version: {}\n", VERSION);

    QuotesWorker quotes_worker;
    ThreadPool   thread_pool(4);

    thread_pool.enqueue_task([&quotes_worker]() {
        while (true) {
            quotes_worker.parse_quotes();

            std::this_thread::sleep_for(std::chrono::minutes(15));
        }
    });

    return 0;
}