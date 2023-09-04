#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <string>
#include <unordered_map>

class Portfolio
{
public:
  Portfolio();
  bool isEmpty() const;
  void purchase(std::string const& symbol, unsigned int shares);
  unsigned int shareCount(std::string const& symbol) const;
  void sell(std::string const& symbol, unsigned int shares);

private:
  bool mIsEmpty;
  std::unordered_map<std::string, unsigned int> mHoldings;
};

#endif