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

class ShareCountCannotBeZeroException : public std::exception
{
public:
  char const* what() const noexcept override
  {
    return "Shares count can't be zero.";
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
  transact(symbol, shares, transactionDate);
}

void
Portfolio::sell(
  std::string const& symbol,
  unsigned int shares,
  boost::gregorian::date const& transactionDate
)
{
  if(shares > shareCount(symbol)) {
    throw InvalidSellException();
  }
  transact(symbol, -shares, transactionDate);
}

void
Portfolio::transact(
  std::string const& symbol,
  int shareChange,
  boost::gregorian::date const& transactionDate
)
{
  if(0 == shareChange) {
    throw ShareCountCannotBeZeroException();
  }
  mHoldings[symbol] = shareCount(symbol) + shareChange;
  mPurchases.push_back(PurchaseRecord(shareChange, transactionDate));
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