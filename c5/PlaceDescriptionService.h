#ifndef PLACE_DESCRIPTION_SERVICE_H_
#define PLACE_DESCRIPTION_SERVICE_H_

#include "Http.h"
#include <rapidjson/document.h>
#include <stdexcept>

class AddressExtractor
{
public:
  AddressExtractor() = default;

  std::string addressFrom(std::string const& jsonResponse) { return ""; }
};

class PlaceDescriptionService
{
public:
  PlaceDescriptionService(Http* http)
    : mHttp(http)
  {
  }

  std::string summaryDescription(
    std::string const& lattitude,
    std::string const& longitude
  ) const
  {
    auto getRequestUrl = "lat=" + lattitude + "&lon=" + longitude;
    auto jsonResponse = mHttp->get(getRequestUrl);
  }

private:
  Http* mHttp;
};

#endif