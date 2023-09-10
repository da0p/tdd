#include "GeoServer.h"
#include "ThreadPool.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
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

  struct GeoServerUserTrackingListener : public GeoServerListener
  {
    void updated(User const& user) { mUsers.push_back(user); }

    std::vector<User> mUsers;
  };

  GeoServerUserTrackingListener mTrackingListener;

  struct SingleThreadedPool : public ThreadPool
  {
    void add(Work work) override { work.execute(); }
  };

  std::shared_ptr<ThreadPool> mPool;

  void SetUp() override
  {
    mPool = std::make_shared<SingleThreadedPool>();
    server.useThreadPool(mPool);
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
  mPool->start(0);
  server.updateLocation(
    bUser, Location{aUserLocation.go(Width / 2 - TenMeters, East)}
  );

  server.usersInBox(aUser, Width, Height, &mTrackingListener);

  ASSERT_THAT(
    userNames(mTrackingListener.mUsers), ::testing::ElementsAre(bUser)
  );
}

TEST_F(AGeoServer_UsersInBox, AnswersOnlyUsersWithinSpecifiedRange)
{
  mPool->start(0);
  server.updateLocation(
    bUser, Location{aUserLocation.go(Width / 2 + TenMeters, East)}
  );
  server.updateLocation(
    cUser, Location{aUserLocation.go(Width / 2 - TenMeters, East)}
  );

  server.usersInBox(aUser, Width, Height, &mTrackingListener);

  ASSERT_THAT(
    userNames(mTrackingListener.mUsers), ::testing::ElementsAre(cUser)
  );
}

TEST_F(AGeoServer_UsersInBox, HandlesLargeNumbersOfUsers)
{
  mPool->start(0);
  Location anotherLocation{aUserLocation.go(10, West)};
  unsigned int const lots{500000};
  for(unsigned int i = 0; i < lots; i++) {
    std::string user{"user" + std::to_string(i)};
    server.track(user);
    server.updateLocation(user, anotherLocation);
  }

  server.usersInBox(aUser, Width, Height, &mTrackingListener);
  ASSERT_THAT(mTrackingListener.mUsers.size(), ::testing::Eq(lots));
}