#include "Area.h"
#include "Location.h"

Area::Area(Location const& location, double width, double height)
{
  mLeft = location.go(width / 2, West).longitude();
  mRight = location.go(width / 2, East).longitude();
  mTop = location.go(height / 2, North).latitude();
  mBottom = location.go(height / 2, South).latitude();
}

Location
Area::upperLeft() const
{
  return Location{mTop, mLeft};
}

Location
Area::upperRight() const
{
  return Location{mTop, mRight};
}

Location
Area::lowerRight() const
{
  return Location{mBottom, mRight};
}

Location
Area::lowerLeft() const
{
  return Location{mBottom, mLeft};
}

bool
Area::inBounds(Location const& location) const
{
  return location.latitude() <= mTop && location.latitude() >= mBottom &&
         location.longitude() <= mRight && location.longitude() >= mLeft;
}