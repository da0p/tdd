#include "Portfolio.h"
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
  updateShareCount(symbol, shareChange);
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
Portfolio::updateShareCount(std::string const& symbol, int shareChange)
{
  mHoldings[symbol] = shareCount(symbol) + shareChange;
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
  mPurchaseRecords[symbol] = std::vector<PurchaseRecord>();
}

void
Portfolio::add(std::string const& symbol, PurchaseRecord&& record)
{
  mPurchaseRecords[symbol].push_back(record);
}

bool
Portfolio::containSymbol(std::string const& symbol) const
{
  return mPurchaseRecords.find(symbol) != mPurchaseRecords.end();
}

unsigned int
Portfolio::shareCount(std::string const& symbol) const
{
  return Find<unsigned int>(mHoldings, symbol);
}

std::vector<PurchaseRecord>
Portfolio::purchases(std::string const& symbol) const
{
  return mPurchaseRecords.find(symbol)->second;
}