#include "Portfolio.h"
#include "gmock/gmock.h"

using namespace ::testing;

namespace {
void
ASSERT_PURCHASE(
  PurchaseRecord& purchase,
  int shares,
  boost::gregorian::date const& date
)
{
  ASSERT_THAT(purchase.mShareCount, Eq(shares));
  ASSERT_THAT(purchase.mDate, Eq(date));
}
}

class APortfolio : public Test
{
public:
  static const std::string IBM;
  static const std::string SAMSUNG;
  Portfolio mPortfolio;
  static const boost::gregorian::date ArbitraryDate;

  void purchase(
    std::string const& symbol,
    unsigned int shares,
    boost::gregorian::date const& transactionDate = APortfolio::ArbitraryDate
  )
  {
    mPortfolio.purchase(symbol, shares, transactionDate);
  }

  void sell(
    std::string const& symbol,
    unsigned int shares,
    boost::gregorian::date const& transactionDate = APortfolio::ArbitraryDate
  )
  {
    mPortfolio.sell(symbol, shares, transactionDate);
  }
};

const std::string APortfolio::IBM{"IBM"};
const std::string APortfolio::SAMSUNG{"SAMSUNG"};
const boost::gregorian::date APortfolio::ArbitraryDate{
  boost::gregorian::date(2014, boost::date_time::Jan, 1)};

TEST_F(APortfolio, IsEmptyWhenCreated)
{
  ASSERT_TRUE(mPortfolio.isEmpty());
}

TEST_F(APortfolio, IsNotEmptyAfterPurchase)
{
  purchase(IBM, 1u);
  ASSERT_FALSE(mPortfolio.isEmpty());
}

TEST_F(APortfolio, AnswersZeroForShareCountOfUnpurchasedSymbol)
{
  ASSERT_THAT(mPortfolio.shareCount("AAPL"), Eq(0u));
}

TEST_F(APortfolio, AnswersShareCountForPurchasedSymbol)
{
  purchase(IBM, 2u);
  ASSERT_THAT(mPortfolio.shareCount(IBM), Eq(2u));
}

TEST_F(APortfolio, AnswersThePurchaseRecordForASinglePurchase)
{
  purchase(SAMSUNG, 5);

  auto purchases = mPortfolio.purchases(SAMSUNG);
  ASSERT_PURCHASE(purchases[0], 5, ArbitraryDate);
}

TEST_F(APortfolio, IncludesSalesInPurchaseRecords)
{
  purchase(SAMSUNG, 10);
  sell(SAMSUNG, 5, ArbitraryDate);

  auto sales = mPortfolio.purchases(SAMSUNG);
  ASSERT_PURCHASE(sales[1], -5, ArbitraryDate);
}

TEST_F(APortfolio, ReducesShareCountOfSymbolOnSell)
{
  purchase(SAMSUNG, 30);
  sell(SAMSUNG, 13);

  ASSERT_THAT(mPortfolio.shareCount(SAMSUNG), Eq(30u - 13));
}

TEST_F(APortfolio, ThrowsWhenSellingMoreSharesThanPurchased)
{
  ASSERT_THROW(sell(SAMSUNG, 1), InsufficientSharesException);
}
