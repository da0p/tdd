#include "Portfolio.h"

Portfolio::Portfolio()
  : mIsEmpty{true}
  , mShareCount(0u)
{
}

bool
Portfolio::isEmpty() const
{
  return mIsEmpty;
}

void
Portfolio::purchase(std::string const& symbol, unsigned int shareCount)
{
  mIsEmpty = false;
  mShareCount = shareCount;
}

unsigned int
Portfolio::shareCount(std::string const& symbol) const
{
  return mShareCount;
}