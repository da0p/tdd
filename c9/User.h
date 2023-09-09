#ifndef USER_H
#define USER_H

#include "Location.h"

#include <string>

class User
{
public:
  User(std::string const& name, Location location)
    : mName{name}
    , mLocation{location}
  {
  }

  [[nodiscard]] std::string const& name() { return mName; }

private:
  std::string mName;
  Location mLocation;
};
#endif