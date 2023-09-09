#include "GeoServer.h"
#include "Location.h"

void
GeoServer::track(std::string const& user)
{
  mLocations[user] = Location();
}

void
GeoServer::stopTracking(std::string const& user)
{
  mLocations.erase(user);
}

bool
GeoServer::isTracking(std::string const& user) const
{
  return find(user) != mLocations.end();
}

void
GeoServer::updateLocation(std::string const& user, Location const& location)
{
  mLocations[user] = location;
}

Location
GeoServer::locationOf(std::string const& user) const
{
  if(!isTracking(user)) {
    return Location{};
  }
  return find(user)->second;
}

std::unordered_map<std::string, Location>::const_iterator
GeoServer::find(std::string const& user) const
{
  return mLocations.find(user);
}

bool
GeoServer::isDifferentUserInBounds(
  std::pair<std::string, Location> const& each,
  std::string const& user,
  Area const& box
) const
{
  if(each.first == user) {
    return false;
  }

  return box.inBounds(each.second);
}

std::vector<User>
GeoServer::usersInBox(
  std::string const& user,
  double widthInMeters,
  double heightInMeters
) const
{
  auto location = mLocations.find(user)->second;
  Area box{location, widthInMeters, heightInMeters};

  std::vector<User> users;
  for(auto& each : mLocations) {
    if(isDifferentUserInBounds(each, user, box)) {
      users.push_back(User{each.first, each.second});
    }
  }
  return users;
}