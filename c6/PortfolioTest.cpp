#include "Portfolio.h"
#include "gmock/gmock.h"

using namespace ::testing;

class APortfolio : public Test
{
public:
  Portfolio mPortfolio;
};

TEST_F(APortfolio, IsEmptyWhenCreated)
{
  ASSERT_TRUE(mPortfolio.isEmpty());
}

TEST_F(APortfolio, IsNotEmptyAfterPurchase)
{
  mPortfolio.purchase("IBM", 1u);
  ASSERT_FALSE(mPortfolio.isEmpty());
}

TEST_F(APortfolio, AnswersZeroForShareCountOfUnpurchasedSymbol)
{
  ASSERT_THAT(mPortfolio.shareCount("AAPL"), Eq(0u));
}

TEST_F(APortfolio, AnswersShareCountForPurchasedSymbol)
{
  mPortfolio.purchase("IBM", 2u);
  ASSERT_THAT(mPortfolio.shareCount("IBM"), Eq(2u));
}