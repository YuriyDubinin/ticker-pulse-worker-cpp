#include <chrono>
#include <fmt/core.h>
#include <memory>
#include <thread>
#include "global_config.h"
#include "news_worker.h"
#include "quotes_worker.h"
#include "thread_pool.h"

int main() {
  fmt::print("\n[TICKER_PULSE_WORKER]: started, version: {}\n", VERSION);

  ThreadPool thread_pool(4);
  auto       quotes_worker = std::make_shared<QuotesWorker>();
  auto       news_worker   = std::make_shared<NewsWorker>();

  // thread_pool.enqueue_task([quotes_worker]() {
  //   while (true) {
  //     quotes_worker->parse_quotes();
  //     std::this_thread::sleep_for(std::chrono::minutes(20));
  //   }
  // });

  thread_pool.enqueue_task([news_worker]() {
    unsigned int category_index = 0;

    while (true) {
      news_worker->fetch_news(category_index);

      if (category_index < 1) {
        category_index++;
      } else {
        category_index = 0;
      }

      std::this_thread::sleep_for(std::chrono::minutes(PARSE_NEWS_INTERVAL));
    }
  });

  return 0;
}