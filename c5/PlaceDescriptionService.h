#ifndef PLACE_DESCRIPTION_SERVICE_H_
#define PLACE_DESCRIPTION_SERVICE_H_

#include "Http.h"
#include <rapidjson/document.h>
#include <stdexcept>
#include <vector>

struct AddressExtractor
{
  AddressExtractor() = default;

  std::string addressFrom(std::string const& jsonResponse)
  {
    rapidjson::Document document;
    if(document.Parse(jsonResponse.c_str()).HasParseError()) {
      return EmptyString;
    }

    if(!document.HasMember(MandatoryFirstWord)) {
      return EmptyString;
    }

    std::string address;
    rapidjson::Value const& addressInfo = document[MandatoryFirstWord];
    for(auto const& field : requiredFields) {
      if(!addressInfo.HasMember(field.c_str())) {
        return EmptyString;
      }
      address += addressInfo[field.c_str()].GetString();
      address += ", ";
    }
    address.pop_back();
    address.pop_back();

    return address;
  }

  std::vector<std::string> requiredFields{};
  static constexpr auto MandatoryFirstWord{"address"};
  static constexpr auto EmptyString = "";
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