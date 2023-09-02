#include "PlaceDescriptionService.h"
#include "Http.h"
#include "gmock/gmock.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::EndsWith;
using ::testing::Eq;

class AnAddressExtractor : public ::testing::Test
{
public:
  AddressExtractor mAddressExtractor;
};

TEST_F(AnAddressExtractor, ReturnEmptyStringIfEmptyInput)
{
  auto address = mAddressExtractor.addressFrom("");

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
  void initialize() override {}
  std::string get(std::string const& url) const override
  {
    verify(url);
    return R"({"address": {"road": "Drury Ln", "city": "Fountain", "state": "CO", "country": "US"}
  }})";
  }

  void verify(std::string const& url) const
  {
    auto expectedArgs{
      "lat=" + std::string(APlaceDescriptionService::ValidLattitude) + "&" +
      "lon=" + std::string(APlaceDescriptionService::ValidLongitude)};

    ASSERT_THAT(url, EndsWith(expectedArgs));
  }
};

TEST_F(APlaceDescriptionService, ReturnsDescriptionForValidLocation)
{
  HttpStub httpStub;
  PlaceDescriptionService service{&httpStub};

  auto description = service.summaryDescription(
    APlaceDescriptionService::ValidLattitude,
    APlaceDescriptionService::ValidLongitude
  );

  ASSERT_THAT(description, Eq("Drury Ln, Fountain, CO, US"));
}