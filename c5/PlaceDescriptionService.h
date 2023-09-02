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
    if(requiredFields.empty()) {
      return EmptyString;
    }

    rapidjson::Document document;
    if(document.Parse(jsonResponse.c_str()).HasParseError() || !document.HasMember(MandatoryFirstWord)) {
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

  std::string
  summaryDescription(std::string const& lattitude, std::string const& longitude)
  {
    auto getRequestUrl = createGetRequestUrl(lattitude, longitude);
    auto jsonResponse = mHttp->get(getRequestUrl);

    AddressExtractor extractor;
    extractor.requiredFields = {"road", "city", "state", "country"};

    auto address = extractor.addressFrom(jsonResponse);

    return address;
  }

private:
  std::string
  createGetRequestUrl(std::string const& latitude, std::string const& longitude)
  {
    std::string server{"http://open.mapquestapi.com/"};
    std::string document{"nominatim/v1/reverse"};
    return server + document + "?" + keyValue("format", "json") + "&" +
           keyValue("lat", latitude) + "&" + keyValue("lon", longitude);
  }

  std::string keyValue(std::string const& key, std::string const& value)
  {
    return key + "=" + value;
  }

  Http* mHttp;
};

#endif