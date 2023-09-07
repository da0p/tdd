#include "Portfolio.h"
#include <numeric>
#include <stdexcept>
#include <unistd.h>

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
    throw InsufficientSharesException();
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
  throwIfShareCountIsZero(shareChange);
  addPurchaseRecord(symbol, shareChange, transactionDate);
}

void
Portfolio::throwIfShareCountIsZero(int shareChange) const
{
  if(0 == shareChange) {
    throw ShareCountCannotBeZeroException();
  }
}

void
Portfolio::addPurchaseRecord(
  std::string const& symbol,
  int shareChange,
  boost::gregorian::date const& date
)
{
  if(!containSymbol(symbol)) {
    initializePurchaseRecords(symbol);
  }
  add(symbol, {shareChange, date});
}

void
Portfolio::initializePurchaseRecords(std::string const& symbol)
{
  mHoldings[symbol] = Holding();
}

void
Portfolio::add(std::string const& symbol, PurchaseRecord&& record)
{
  mHoldings[symbol].add(record);
}

bool
Portfolio::containSymbol(std::string const& symbol) const
{
  return mHoldings.find(symbol) != mHoldings.end();
}

unsigned int
Portfolio::shareCount(std::string const& symbol) const
{
  return Find<Holding>(mHoldings, symbol).shareCount();
}

std::vector<PurchaseRecord>
Portfolio::purchases(std::string const& symbol) const
{
  return Find<Holding>(mHoldings, symbol).purchases();
}