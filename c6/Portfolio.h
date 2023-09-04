#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <boost/date_time/gregorian/gregorian.hpp>
#include <string>
#include <unordered_map>
#include <vector>

struct PurchaseRecord
{
  PurchaseRecord(unsigned int shareCount, boost::gregorian::date const& date)
    : mShareCount{shareCount}
    , mDate{date}
  {
  }

  unsigned int mShareCount;
  boost::gregorian::date mDate;
};

class Portfolio
{
public:
  Portfolio();
  bool isEmpty() const;
  void purchase(
    std::string const& symbol,
    unsigned int shares,
    boost::gregorian::date const& transactionDate
  );
  unsigned int shareCount(std::string const& symbol) const;
  void sell(std::string const& symbol, unsigned int shares);
  std::vector<PurchaseRecord> purchases(std::string const& symbol) const;

private:
  bool mIsEmpty;
  std::unordered_map<std::string, unsigned int> mHoldings;
  std::vector<PurchaseRecord> mPurchases;
};

#endif