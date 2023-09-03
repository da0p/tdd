#include "PlaceDescriptionService.h"
#include "Http.h"
#include "gmock/gmock.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::Eq;

class AnAddressExtractor : public ::testing::Test
{
public:
  AddressExtractor mAddressExtractor;
};

TEST_F(AnAddressExtractor, ReturnEmptyStringIfInputDoesNotContainAddressField)
{
  auto address = mAddressExtractor.addressFrom(R"({"NotAnAddress": "XYZ"})");

  ASSERT_THAT(address, Eq(""));
}

TEST_F(AnAddressExtractor, ReturnStringContainsValuesTakenFromASpecifiedKey)
{
  mAddressExtractor.requiredFields = {"road"};

  auto address =
    mAddressExtractor.addressFrom(R"({"address":{"road": "Drury Ln"}})");

  ASSERT_THAT(address, Eq("Drury Ln"));
}

TEST_F(AnAddressExtractor, ReturnStringContainsValuesTakenFromMoreSpecifiedKeys)
{
  mAddressExtractor.requiredFields = {"road", "city", "state", "country"};
  auto address = mAddressExtractor.addressFrom(
    R"({"address": {"road": "Drury Ln", "city": "Fountain", "state": "CO", "country": "US"}})"
  );

  ASSERT_THAT(address, Eq("Drury Ln, Fountain, CO, US"));
}

TEST_F(AnAddressExtractor, ReturnEmptyStringIfInputDoesNotContainRequireFields)
{
  mAddressExtractor.requiredFields = {"road", "city", "state", "country"};
  auto address = mAddressExtractor.addressFrom(
    R"({"address": {"road": "Drury Ln", "city": "Fountain", "state": "CO", "landscape": "dessert"}})"
  );

  ASSERT_THAT(address, Eq(""));
}

TEST_F(AnAddressExtractor, ReturnEmptyStringIfInputIsNotAValidJson)
{
  auto address = mAddressExtractor.addressFrom("XYX: ");
  ASSERT_THAT(address, Eq(""));
}

TEST_F(AnAddressExtractor, ReturnEmptyStringIfRequiredFieldsIsEmpty)
{
  auto address = mAddressExtractor.addressFrom(
    R"({"address": {"road": "Drury Ln", "city": "Fountain", "state": "CO", "country": "US"}})"
  );

  ASSERT_THAT(address, Eq(""));
}

class APlaceDescriptionService : public ::testing::Test
{
public:
  static auto constexpr ValidLattitude{"11.22"};
  static auto constexpr ValidLongitude{"22.33"};
};

class HttpStub : public Http
{
public:
  std::string returnResponse;
  std::string expectedURL;

  void initialize() override {}
  std::string get(std::string const& url) const override
  {
    verify(url);
    return returnResponse;
  }

  void verify(std::string const& url) const
  {
    ASSERT_THAT(url, Eq(expectedURL));
  }
};

TEST_F(APlaceDescriptionService, ReturnsDescriptionForValidLocation)
{
  HttpStub httpStub;
  httpStub.returnResponse =
    R"({"address":{"road":"Drury Ln", "city":"Fountain", "state":"CO", "country":"US"}})";

  std::string urlStart{
    "http://open.mapquestapi.com/nominatim/v1/reverse?format=json&"};

  httpStub.expectedURL = {
    urlStart + "lat=" + std::string(APlaceDescriptionService::ValidLattitude) +
    "&" + "lon=" + std::string(APlaceDescriptionService::ValidLongitude)};

  PlaceDescriptionService service{&httpStub};

  auto description = service.summaryDescription(
    APlaceDescriptionService::ValidLattitude,
    APlaceDescriptionService::ValidLongitude
  );

  ASSERT_THAT(description, Eq("Drury Ln, Fountain, CO, US"));
}