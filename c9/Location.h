#ifndef LOCATION_H
#define LOCATION_H

#include <cmath>
#include <limits>
#include <ostream>

double const Pi{4.0 * atan(1.0)};
double const ToRadiansConversionFactor{Pi / 180};
double const RadiusOfEarthInMeters{6372000};
double const MetersPerDegreeAtEquator{111111};

double const North{0};
double const West{90};
double const South{180};
double const East{270};
double const CloseMeters{3};

class Location
{
public:
  Location();
  Location(double latitude, double longitude);

  inline double toRadians(double degrees) const
  {
    return degrees * ToRadiansConversionFactor;
  }

  inline double toCoordinate(double radians) const
  {
    return radians * (180 / Pi);
  }

  inline double latitudeAsRadians() const { return toRadians(mLatitude); }

  inline double longitudeAsRadians() const { return toRadians(mLongitude); }

  double latitude() const;

  double longitude() const;

  bool operator==(Location const& that);
  bool operator!=(Location const& that);

  Location go(double meters, double bearing) const;
  double distanceInMeters(Location const& there) const;
  bool isUnknown() const;
  bool isVeryCloseTo(Location const& there) const;

private:
  double mLatitude;
  double mLongitude;

  double haversineDistance(Location there) const;
};

std::ostream&
operator<<(std::ostream& output, Location const& location);

#endif