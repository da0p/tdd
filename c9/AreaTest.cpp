#include "Area.h"
#include "Location.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class AnArea : public ::testing::Test
{
public:
  double const Tolerance{0.005};
  const Location Zero{0, 0};
  double const OneKm{1000};
  double const HalfOneKm{500};
  double const TwoKm{2000};
  double const HalfTwoKm{1000};
  const Area OneByTwoAreaAtZero{Zero, OneKm, TwoKm};
};

TEST_F(AnArea, AnswersUpperLeftForBoundingRectangle)
{
  Location upperLeft{OneByTwoAreaAtZero.upperLeft()};

  ASSERT_NEAR(
    Zero.go(HalfOneKm, West).longitude(), upperLeft.longitude(), Tolerance
  );
  ASSERT_NEAR(
    Zero.go(HalfTwoKm, North).latitude(), upperLeft.latitude(), Tolerance
  );
}

TEST_F(AnArea, AnswersUpperRightForBoundingRectangle)
{
  Location upperRight{OneByTwoAreaAtZero.upperRight()};

  ASSERT_NEAR(
    Zero.go(HalfOneKm, East).longitude(), upperRight.longitude(), Tolerance
  );
  ASSERT_NEAR(
    Zero.go(HalfTwoKm, North).latitude(), upperRight.latitude(), Tolerance
  );
}

TEST_F(AnArea, AnswersLowerRightForBoundingRectangle)
{
  Location lowerRight{OneByTwoAreaAtZero.lowerRight()};

  ASSERT_NEAR(
    Zero.go(HalfOneKm, East).longitude(), lowerRight.longitude(), Tolerance
  );
  ASSERT_NEAR(
    Zero.go(HalfTwoKm, South).latitude(), lowerRight.latitude(), Tolerance
  );
}

TEST_F(AnArea, AnswersLowerLeftForBoundingRectangle)
{
  Location lowerLeft{OneByTwoAreaAtZero.lowerLeft()};

  ASSERT_NEAR(
    Zero.go(HalfOneKm, West).longitude(), lowerLeft.longitude(), Tolerance
  );
  ASSERT_NEAR(
    Zero.go(HalfTwoKm, South).latitude(), lowerLeft.latitude(), Tolerance
  );
}

TEST_F(AnArea, DesignatesAPointOutOfBoundsWhenTwooFarNorth)
{
  Location tooNorth{Zero.go(HalfTwoKm + 10, North).latitude(), 0};

  ASSERT_FALSE(OneByTwoAreaAtZero.inBounds(tooNorth));
}

TEST_F(AnArea, DesignatesAPointInBoundsWhenAtSouth)
{
  Location southEdge{Zero.go(HalfTwoKm, South).latitude(), 0};

  ASSERT_TRUE(OneByTwoAreaAtZero.inBounds(southEdge));
}

TEST_F(AnArea, DesignatesAPointOutOfBoundsWhenTooFarWest)
{
  Location tooWest{0, Zero.go(HalfOneKm + 10, West).longitude()};

  ASSERT_FALSE(OneByTwoAreaAtZero.inBounds(tooWest));
}

TEST_F(AnArea, DesignatesAPointOutOfBoundsWhenTooFarEast)
{
  Location tooEast{0, Zero.go(HalfOneKm + 10, East).longitude()};

  ASSERT_FALSE(OneByTwoAreaAtZero.inBounds(tooEast));
}

TEST_F(AnArea, DesignatesAPointInBoundsWhenAtWest)
{
  Location westEdge{0, Zero.go(HalfOneKm, West).longitude()};

  ASSERT_TRUE(OneByTwoAreaAtZero.inBounds(westEdge));
}

TEST_F(AnArea, DesignatesAPointInBoundsWhenAtEast)
{
  Location eastEdge{0, Zero.go(HalfOneKm, East).longitude()};

  ASSERT_TRUE(OneByTwoAreaAtZero.inBounds(eastEdge));
}