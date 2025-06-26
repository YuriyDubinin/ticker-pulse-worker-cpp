#include <chrono>
#include <fmt/core.h>
#include <thread>
#include "global_config.h"
#include "news_worker.h"
#include "quotes_worker.h"
#include "thread_pool.h"

int main() {
  fmt::print("\n[TICKER_PULSE_WORKER]: started, version: {}\n", VERSION);

  ThreadPool   thread_pool(4);
  QuotesWorker quotes_worker;
  NewsWorker   news_worker;

  // news_worker.fetch_news();

  // thread_pool.enqueue_task([&quotes_worker]() {
  //     while (true) {
  //         quotes_worker.parse_quotes();

  //         std::this_thread::sleep_for(std::chrono::minutes(15));
  //     }
  // });

  return 0;
}