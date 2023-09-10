#include "ThreadPool.h"

#include <gmock/gmock.h>

class AThreadPool : public ::testing::Test
{
public:
  ThreadPool mPool;
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