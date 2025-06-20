#include "news_worker.h"
#include "global_config.h"

NewsWorker::NewsWorker() : postgres_connection(DB_CONNECTION_INFO) {
}

NewsWorker::~NewsWorker() {
}

void NewsWorker::parse_news() {
}

int NewsWorker::fetch_news(unsigned int offset, unsigned int step) {
}