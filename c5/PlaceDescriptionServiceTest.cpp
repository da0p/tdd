#include "PlaceDescriptionService.h"
#include "Http.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::Eq;

class HttpStub : public Http
{
public:
  void initialize() override {}
  std::string get(std::string const& url) const override
  {
    return R"({"address": {"road": "Drury Ln", "city": "Fountain", "state": "CO", "country": "US"}
  }})";
  }
};

class APlaceDescriptionService : public ::testing::Test
{
public:
  static auto constexpr ValidLattitude{"11.22"};
  static auto constexpr ValidLongitude{"22.33"};
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