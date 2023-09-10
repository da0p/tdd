#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "Work.h"
#include <atomic>
#include <deque>
#include <memory>
#include <string>
#include <thread>

class ThreadPool
{
public:
  virtual ~ThreadPool() { stop(); }

  void stop()
  {
    mDone = true;
    for(auto& thread : mThreads) {
      thread.join();
    }
  }

  void start(unsigned int numberOfThreads = 1)
  {
    for(unsigned int i = 0; i < numberOfThreads; i++) {
      mThreads.push_back(std::thread(&ThreadPool::worker, this));
    }
  }

  bool hasWork()
  {
    std::lock_guard<std::mutex> block(mMutex);
    return !mWorkQueue.empty();
  }

  void add(Work work)
  {
    std::lock_guard<std::mutex> block(mMutex);
    mWorkQueue.push_front(work);
  }

  Work pullWork()
  {
    std::lock_guard<std::mutex> block(mMutex);
    if(mWorkQueue.empty()) {
      return Work{};
    }
    auto work = mWorkQueue.back();
    mWorkQueue.pop_back();
    return work;
  }

private:
  void worker()
  {
    while(!mDone) {
      while(!mDone && !hasWork())
        ;
      if(mDone) {
        break;
      }
      pullWork().execute();
    }
  }

  std::deque<Work> mWorkQueue;
  std::atomic<bool> mDone{false};
  std::shared_ptr<std::thread> mWorkThread;
  std::mutex mMutex;
  std::vector<std::thread> mThreads;
};

#endif