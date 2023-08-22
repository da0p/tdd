#include <string>

class Soundex
{
public:
  std::string encode(std::string const& word) const { return word; }
};

#include "gmock/gmock.h"
using ::testing::Eq;

TEST(SoundexEncoding, RetainsSoleLetterOfOneLetterWord)
{
  Soundex soundex;

  auto encoded = soundex.encode("A");

  ASSERT_THAT(encoded, Eq("A"));
}