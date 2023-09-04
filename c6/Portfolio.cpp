#include "Portfolio.h"
#include <stdexcept>
#include <unistd.h>

class InvalidPurchaseException : public std::exception
{
public:
  char const* what() const noexcept override
  {
    return "Can't purchase 0 share.";
  }
};

class InvalidSellException : public std::exception
{
public:
  char const* what() const noexcept override
  {
    return "Can't sell more shares than remaining shares.";
  }
};

Portfolio::Portfolio()
  : mIsEmpty{true}
{
}

bool
Portfolio::isEmpty() const
{
  return 0 == mHoldings.size();
}

void
Portfolio::purchase(
  std::string const& symbol,
  unsigned int shares,
  boost::gregorian::date const& transactionDate
)
{
  if(0 == shares) {
    throw InvalidPurchaseException();
  }
  mHoldings[symbol] = shares + shareCount(symbol);
  mPurchases.push_back(PurchaseRecord(shares, transactionDate));
}

void
Portfolio::sell(std::string const& symbol, unsigned int shares)
{
  if(shares > shareCount(symbol)) {
    throw InvalidSellException();
  }
  mHoldings[symbol] = shareCount(symbol) - shares;
}

unsigned int
Portfolio::shareCount(std::string const& symbol) const
{
  auto it = mHoldings.find(symbol);
  if(it == mHoldings.end()) {
    return 0;
  }
  return it->second;
}

std::vector<PurchaseRecord>
Portfolio::purchases(std::string const& symbol) const
{
  return mPurchases;
}