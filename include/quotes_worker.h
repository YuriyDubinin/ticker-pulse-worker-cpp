#ifndef QUOTES_WORKER
#define QUOTES_WORKER

class QuotesWorker {
    public:
      explicit QuotesWorker();
      ~QuotesWorker();
    
    private:
      void fetch_quotes();
};

#endif