#ifndef GEO_SERVER_H
#define GEO_SERVER_H

#include <string>
#include <unordered_map>
#include <vector>

#include "Area.h"
#include "Location.h"
#include "User.h"

class GeoServer
{
public:
  void track(std::string const& user);
  void stopTracking(std::string const& user);
  void updateLocation(std::string const& user, Location const& location);

  bool isTracking(std::string const& user) const;
  Location locationOf(std::string const& user) const;

  bool isDifferentUserInBounds(
    std::pair<std::string, Location> const& each,
    std::string const& user,
    Area const& box
  ) const;

  std::vector<User> usersInBox(
    std::string const& user,
    double widthInMeters,
    double heightInMeters
  ) const;

private:
  std::unordered_map<std::string, Location> mLocations;
  std::unordered_map<std::string, Location>::const_iterator find(
    std::string const& user
  ) const;
};

#endif