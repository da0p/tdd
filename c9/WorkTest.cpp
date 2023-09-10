#include "Work.h"

#include <functional>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>

class AWorkObject : public ::testing::Test
{
public:
  Work work;
};

TEST_F(AWorkObject, DefaultsFunctionToNullObject)
{
  ASSERT_NO_THROW(work.execute());
}

TEST_F(AWorkObject, DefaultsFunctionToNullObjectWhenConstructedWithId)
{
  Work work(1);
  ASSERT_NO_THROW(work.execute());
}

TEST_F(AWorkObject, CanBeConstructedWithAnId)
{
  Work work(1);
  ASSERT_THAT(work.id(), ::testing::Eq(1));
}

TEST_F(AWorkObject, DefaultsIdTo0)
{
  Work work;
  ASSERT_THAT(work.id(), ::testing::Eq(0));
}

TEST_F(AWorkObject, DefaultsIdTo0WhenFunctionSpecified)
{
  Work work{[] {}};
  ASSERT_THAT(work.id(), ::testing::Eq(0));
}

TEST_F(AWorkObject, CanBeConstructedWithAFunctionAndId)
{
  Work work{[] {}, 1};
  ASSERT_THAT(work.id(), ::testing::Eq(1));
}

TEST_F(AWorkObject, ExecutesFunctionStored)
{
  bool wasExecuted{false};
  auto executionFunction = [&]() { wasExecuted = true; };
  Work work{executionFunction};
  work.execute();
  ASSERT_TRUE(wasExecuted);
}

TEST_F(AWorkObject, CanExecuteOnDataCapturedWithFunction)
{
  std::vector<std::string> data{"a", "b"};
  std::string result;
  auto callbackFunction = [&](std::string s) { result.append(s); };

  auto executeFunction = [&]() {
    std::stringstream s;
    s << data[0] << data[1];
    callbackFunction(s.str());
  };
  Work work(executeFunction);
  work.execute();
  ASSERT_THAT(result, ::testing::Eq("ab"));
}