#include "ThreadPool.h"

#include <chrono>
#include <condition_variable>
#include <gmock/gmock.h>

class AThreadPool : public ::testing::Test
{
public:
  ThreadPool mPool;
  std::mutex m;
  std::condition_variable wasExecuted;
  unsigned int count{0};
  std::vector<std::shared_ptr<std::thread>> mThreads;

  void SetUp() override { mPool.start(); }

  void TearDown() override
  {
    for(auto& t : mThreads) {
      t->join();
    }
  }

  void incrementCountAndNotify()
  {
    std::unique_lock<std::mutex> lock{m};
    ++count;
    wasExecuted.notify_all();
  }

  void waitForCountAndFailOnTimeout(
    unsigned int expectedCount,
    const std::chrono::milliseconds time = std::chrono::milliseconds(100)
  )
  {
    std::unique_lock<std::mutex> lock{m};
    ASSERT_TRUE(wasExecuted.wait_for(lock, time, [&] {
      return expectedCount == count;
    }));
  }
};

TEST_F(AThreadPool, HasNoWorkOnCreation)
{
  ASSERT_FALSE(mPool.hasWork());
}

TEST_F(AThreadPool, HasWorkAfterAdd)
{
  mPool.add(Work{});
  ASSERT_TRUE(mPool.hasWork());
}

TEST_F(AThreadPool, AnswersWorkAddedOnPull)
{
  mPool.add(Work{1});
  auto work = mPool.pullWork();

  ASSERT_THAT(work.id(), ::testing::Eq(1));
}

TEST_F(AThreadPool, PullElementsInFIFOOrder)
{
  mPool.add(Work{1});
  mPool.add(Work{2});

  auto work = mPool.pullWork();

  ASSERT_THAT(work.id(), ::testing::Eq(1));
}

TEST_F(AThreadPool, HasNoWorkAfterLastElementRemoved)
{
  mPool.add(Work{});
  mPool.pullWork();

  ASSERT_FALSE(mPool.hasWork());
}

TEST_F(AThreadPool, HasWorkAfterRemovedButWorkRemains)
{
  mPool.add(Work{});
  mPool.add(Work{});
  mPool.pullWork();

  ASSERT_TRUE(mPool.hasWork());
}

TEST_F(AThreadPool, PullsWorkInAThread)
{
  Work work{[&] { incrementCountAndNotify(); }};
  unsigned int NumberOfWorkItems{1};

  mPool.add(work);

  waitForCountAndFailOnTimeout(NumberOfWorkItems);
}

TEST_F(AThreadPool, ExecutesAllWork)
{
  Work work{[&] { incrementCountAndNotify(); }};
  unsigned int NumberOfWorkItems{3};

  for(unsigned int i = 0; i < NumberOfWorkItems; i++) {
    mPool.add(work);
  }

  waitForCountAndFailOnTimeout(NumberOfWorkItems);
}

TEST_F(AThreadPool, HoldsUpUnderClientStress)
{
  Work work{[&] { incrementCountAndNotify(); }};
  unsigned int NumberOfWorkItems{200};
  unsigned int NumberOfThreads{200};

  for(unsigned int i{0}; i < NumberOfThreads; i++)
    mThreads.push_back(std::make_shared<std::thread>([&] {
      for(unsigned int j{0}; j < NumberOfWorkItems; j++)
        mPool.add(work);
    }));

  waitForCountAndFailOnTimeout(NumberOfThreads * NumberOfWorkItems);
}

class MultipleThreadsPool : public AThreadPool
{
public:
  std::set<std::thread::id> mThreadIds;

  void SetUp() override {}

  void TearDown() override {}

  void addThreadIfUnique(std::thread::id const& id)
  {
    std::unique_lock<std::mutex> lock{m};
    mThreadIds.insert(id);
  }

  size_t numberOfThreadsProcessed() { return mThreadIds.size(); }
};

TEST_F(MultipleThreadsPool, DispatchesWorkToMultipleThreads)
{
  unsigned int numberOfThreads{2};
  mPool.start(numberOfThreads);
  Work work{[&] {
    addThreadIfUnique(std::this_thread::get_id());
    incrementCountAndNotify();
  }};
  unsigned int NumberOfWorkItems{500};

  for(unsigned int i = 0; i < NumberOfWorkItems; i++) {
    mPool.add(work);
  }

  waitForCountAndFailOnTimeout(NumberOfWorkItems);
  ASSERT_THAT(numberOfThreadsProcessed(), ::testing::Eq(numberOfThreads));
}