#include "PlaceDescriptionService.h"
#include "Http.h"
#include "gmock/gmock.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::NiceMock;
using ::testing::Return;

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
  MOCK_METHOD0(initialize, void());
  MOCK_CONST_METHOD1(get, std::string(std::string const&));
};

TEST_F(APlaceDescriptionService, ReturnsDescriptionForValidLocation)
{
  NiceMock<HttpStub> httpStub;
  EXPECT_CALL(httpStub, get(_))
    .WillOnce(Return(
      R"({"address":{"road":"Drury Ln", "city":"Fountain", "state":"CO", "country":"US"}})"
    ));
  PlaceDescriptionService service{&httpStub};

  auto description = service.summaryDescription(
    APlaceDescriptionService::ValidLattitude,
    APlaceDescriptionService::ValidLongitude
  );

  ASSERT_THAT(description, Eq("Drury Ln, Fountain, CO, US"));
}

TEST_F(APlaceDescriptionService, MakesHttpRequestToObtainAddress)
{
  InSequence forceExpectationOrder;
  HttpStub httpStub;
  std::string urlStart{
    "http://open.mapquestapi.com/nominatim/v1/reverse?format=json&"};
  auto expectedURL = urlStart +
                     "lat=" + APlaceDescriptionService::ValidLattitude + "&" +
                     "lon=" + APlaceDescriptionService::ValidLongitude;
  EXPECT_CALL(httpStub, initialize());
  EXPECT_CALL(httpStub, get(expectedURL));
  PlaceDescriptionService service{&httpStub};
  service.summaryDescription(
    APlaceDescriptionService::ValidLattitude,
    APlaceDescriptionService::ValidLongitude
  );
}