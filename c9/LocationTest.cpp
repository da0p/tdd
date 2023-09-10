#include "Location.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class ALocation : public ::testing::Test
{
public:
  Location aLocation;
  double const Tolerance{0.005};
  const Location ArbitraryLocation{38.2, -104.5};
};

TEST_F(ALocation, AnswersLatitudeAndLongitude)
{
  Location location{10, 20};

  ASSERT_THAT(location.latitude(), ::testing::Eq(10));
  ASSERT_THAT(location.longitude(), ::testing::Eq(20));
}

TEST_F(ALocation, IsNotUnknownWhenLatitudeAndLongitudeProvided)
{
  Location location{10, 20};
  ASSERT_FALSE(location.isUnknown());
}

TEST_F(ALocation, IsUnknwonWhenLatitudeAndLongitudeNotProvided)
{
  Location location;
  ASSERT_TRUE(location.isUnknown());
}

TEST_F(ALocation, AnswersDistanceFromAnotherInMeters)
{
  Location point1{38.017, -104.84};
  Location point2{38.025, -104.99};

  ASSERT_NEAR(point1.distanceInMeters(point2), 13171.7981, 0.0001);
}

TEST_F(ALocation, IsNotEqualToAnotherWhenLatDiffers)
{
  Location point1{10, 11};
  Location point2{11, 11};

  ASSERT_TRUE(point1 != point2);
}

TEST_F(ALocation, IsNotEqualToAnotherWhenLatAndLongMatch)
{
  Location point1{10, 11};
  Location point2{10, 11};

  ASSERT_TRUE(point1 == point2);
}

TEST_F(ALocation, AnswersNewLocationGivenDistanceAndBearing)
{
  Location start{0, 0};
  auto newLocation = start.go(MetersPerDegreeAtEquator, East);

  Location expectedEnd{0, 1};
  ASSERT_NEAR(newLocation.longitude(), 1, 0.001);
  ASSERT_NEAR(newLocation.latitude(), 0, 0.001);
}

TEST_F(ALocation, AnswersNewLocationGivenDistanceAndBearingVerifiedByHaversine)
{
  double distance{100};
  Location start{38, -78};

  auto end = start.go(distance, 35);

  ASSERT_NEAR(distance, start.distanceInMeters(end), 0.001);
}

TEST_F(ALocation, CanBePole)
{
  Location start{90, 0};

  auto end = start.go(MetersPerDegreeAtEquator, South);
  ASSERT_NEAR(0, end.longitude(), 0.001);
  ASSERT_NEAR(89, end.latitude(), 0.001);
}

TEST_F(ALocation, IsVeryCloseToAnotherWhenSmallDistanceApart)
{
  Location threeMetersAway{ArbitraryLocation.go(3, South)};
  ASSERT_TRUE(ArbitraryLocation.isVeryCloseTo(threeMetersAway));
}

TEST_F(ALocation, IsNotVeryCloseToAnotherWhenNotSmallDistanceApart)
{
  Location fourMetersAway{ArbitraryLocation.go(4, South)};
  ASSERT_FALSE(ArbitraryLocation.isVeryCloseTo(fourMetersAway));
}

TEST_F(ALocation, ProvidesPrintableRepresentation)
{
  Location location{-32, -105};
  std::stringstream s;
  s << location;

  ASSERT_THAT(s.str(), "(-32,-105)");
}