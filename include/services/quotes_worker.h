#ifndef QUOTES_WORKER
#define QUOTES_WORKER

class QuotesWorker {
  public:
    explicit QuotesWorker();
    ~QuotesWorker();

    void parse_quotes();

  private:
    void fetch_quotes();
};

#endif