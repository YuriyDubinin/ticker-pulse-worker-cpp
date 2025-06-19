#include <fmt/core.h>
#include "global_config.h"
#include "quotes_worker.h"

int main() {
    fmt::print("\n[TICKER_PULSE_WORKER]: started, version: {}\n", VERSION);

    QuotesWorker quotes_worker;

    quotes_worker.parse_quotes();

    return 0;
}