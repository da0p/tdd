#include "Location.h"
#include <iostream>
#include <limits>
#include <math.h>
#include <ostream>

std::ostream&
operator<<(std::ostream& output, Location const& location)
{
  output << "(" << location.latitude() << "," << location.longitude() << ")";
  return output;
}

Location::Location()
  : mLatitude(std::numeric_limits<double>::infinity())
  , mLongitude(std::numeric_limits<double>::infinity())
{
}

Location::Location(double latitude, double longitude)
  : mLatitude(latitude)
  , mLongitude(longitude)
{
}

double
Location::latitude() const
{
  return mLatitude;
}

double
Location::longitude() const
{
  return mLongitude;
}

bool
Location::operator==(Location const& that)
{
  return mLongitude == that.mLongitude && mLatitude == that.mLatitude;
}

bool
Location::operator!=(Location const& that)
{
  return !(*this == that);
}

Location
Location::go(double meters, double bearing) const
{
  bearing = toRadians(bearing);
  double distance{meters / RadiusOfEarthInMeters};
  double newLat{asin(
    sin(latitudeAsRadians()) * cos(distance) +
    cos(latitudeAsRadians()) * sin(distance) * cos(bearing)
  )};
  double newLong = longitudeAsRadians();
  if(cos(latitudeAsRadians()) != 0) {
    newLong = fmod(
                longitudeAsRadians() -
                  asin(sin(bearing) * sin(distance) / cos(newLat)) + Pi,
                2 * Pi
              ) -
              Pi;
  }

  return Location(toCoordinate(newLat), toCoordinate(newLong));
}

double
Location::distanceInMeters(Location const& there) const
{
  return RadiusOfEarthInMeters * haversineDistance(there);
}

bool
Location::isUnknown() const
{
  return mLatitude == std::numeric_limits<double>::infinity();
}

bool
Location::isVeryCloseTo(Location const& there) const
{
  return distanceInMeters(there) <= CloseMeters;
}

double
Location::haversineDistance(Location there) const
{
  double deltaLongitude{longitudeAsRadians() - there.longitudeAsRadians()};
  double deltaLatitude{latitudeAsRadians() - there.latitudeAsRadians()};

  double aHaversine{
    pow(sin(deltaLatitude / 2.0), 2.0) + cos(latitudeAsRadians()) *
                                           cos(there.latitudeAsRadians()) *
                                           pow(sin(deltaLongitude / 2), 2)};

  return 2 * atan2(sqrt(aHaversine), sqrt(1.0 - aHaversine));
}