#include "Portfolio.h"
#include <stdexcept>

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
Portfolio::purchase(std::string const& symbol, unsigned int shares)
{
  if(0 == shares) {
    throw InvalidPurchaseException();
  }
  mHoldings[symbol] = shares + shareCount(symbol);
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