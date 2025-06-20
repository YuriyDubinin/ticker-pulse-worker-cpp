#include <fmt/core.h>
#include "global_config.h"
#include "quotes_worker.h"
#include "thread_pool.h"

int main() {
    fmt::print("\n[TICKER_PULSE_WORKER]: started, version: {}\n", VERSION);

    QuotesWorker quotes_worker;
    ThreadPool   thread_pool(4);

    thread_pool.enqueue_task([&quotes_worker]() {
        quotes_worker.parse_quotes();
    });

    return 0;
}