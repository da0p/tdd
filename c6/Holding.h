#ifndef HOLDING_H
#define HOLDING_H

#include <boost/date_time/gregorian/gregorian.hpp>
#include <numeric>
#include <unordered_map>

struct PurchaseRecord
{
  PurchaseRecord(int shareCount, boost::gregorian::date const& date)
    : mShareCount{shareCount}
    , mDate{date}
  {
  }

  int mShareCount;
  boost::gregorian::date mDate;
};

class Holding
{
public:
  Holding() = default;

  void add(PurchaseRecord& record) { mPurchaseRecords.push_back(record); }

  std::vector<PurchaseRecord> purchases() const { return mPurchaseRecords; }

  unsigned int shareCount() const
  {
    return std::accumulate(
      mPurchaseRecords.begin(),
      mPurchaseRecords.end(),
      0,
      [](int total, PurchaseRecord record) {
        return total + record.mShareCount;
      }
    );
  }

private:
  std::vector<PurchaseRecord> mPurchaseRecords;
};

#endif