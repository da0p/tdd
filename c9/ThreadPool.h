#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "Work.h"
#include <deque>
#include <string>

class ThreadPool
{
public:
  bool hasWork() { return !mWorkQueue.empty(); }

  void add(Work work) { mWorkQueue.push_front(work); }

  Work pullWork()
  {
    auto work = mWorkQueue.back();
    mWorkQueue.pop_back();
    return work;
  }

private:
  std::deque<Work> mWorkQueue;
};

#endif