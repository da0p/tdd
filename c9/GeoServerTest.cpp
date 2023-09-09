#include "GeoServer.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

class AGeoServer_UsersInBox : public ::testing::Test
{
public:
  GeoServer server;

  double const TenMeters{10};
  double const Width{2000 + TenMeters};
  double const Height{4000 + TenMeters};
  std::string const aUser{"auser"};
  std::string const bUser{"buser"};
  std::string const cUser{"cuser"};

  Location aUserLocation{38, -103};

  void SetUp() override
  {
    server.track(aUser);
    server.track(bUser);
    server.track(cUser);
    server.updateLocation(aUser, aUserLocation);
  }

  std::vector<std::string> userNames(std::vector<User> const& users)
  {
    std::vector<std::string> names;
    std::for_each(users.begin(), users.end(), [&names](auto user) {
      names.emplace_back(user.name());
    });
    return names;
  }
};

TEST_F(AGeoServer_UsersInBox, AnswersUsersInSpecifiedRange)
{
  server.updateLocation(
    bUser, Location{aUserLocation.go(Width / 2 - TenMeters, East)}
  );

  auto users = server.usersInBox(aUser, Width, Height);

  ASSERT_THAT()
}