#ifndef AREA_H
#define AREA_H

#include "Location.h"

class Area
{
public:
  Area(Location const& location, double width, double height);
  Location upperLeft() const;
  Location upperRight() const;
  Location lowerRight() const;
  Location lowerLeft() const;
  bool inBounds(Location const&) const;

private:
  double mLeft;
  double mRight;
  double mTop;
  double mBottom;
};

#endif