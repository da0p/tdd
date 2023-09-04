#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <string>

class Portfolio
{
public:
  Portfolio();
  bool isEmpty() const;
  void purchase(std::string const& symbol, unsigned int shareCount);
  unsigned int shareCount(std::string const& symbol) const;

private:
  bool mIsEmpty;
  unsigned int mShareCount;
};

#endif