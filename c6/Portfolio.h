#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <boost/date_time/gregorian/gregorian.hpp>
#include <string>
#include <unordered_map>
#include <vector>

#include "Holding.h"

class InsufficientSharesException : public std::exception
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

template<typename T>
T
Find(std::unordered_map<std::string, T> map, std::string const& key)
{
  auto it = map.find(key);
  return it == map.end() ? T{} : it->second;
}

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
  void sell(
    std::string const& symbol,
    unsigned int shares,
    boost::gregorian::date const& transactionDate
  );
  std::vector<PurchaseRecord> purchases(std::string const& symbol) const;

private:
  void transact(
    std::string const& symbol,
    int shareChange,
    boost::gregorian::date const& transactionDate
  );

  void throwIfShareCountIsZero(int shareChange) const;

  void addPurchaseRecord(
    std::string const& symbol,
    int shareChange,
    boost::gregorian::date const& date
  );

  void add(std::string const& symbol, PurchaseRecord&& record);

  void initializePurchaseRecords(std::string const& symbol);

  bool containSymbol(std::string const& symbol) const;

  bool mIsEmpty;
  std::unordered_map<std::string, Holding> mHoldings;
};

#endif