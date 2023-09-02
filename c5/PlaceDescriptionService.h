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
    if(jsonResponse.empty()) {
      return "";
    }

    std::string address;
    rapidjson::Document document;
    document.Parse(jsonResponse.c_str());

    if(!document.HasMember("address")) {
      return "";
    }

    rapidjson::Value const& addressInfo = document["address"];
    if(addressInfo.HasMember(requiredFields[0].c_str())) {
      address += addressInfo[requiredFields[0].c_str()].GetString();
    }

    return address;
  }

  std::vector<std::string> requiredFields{};
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