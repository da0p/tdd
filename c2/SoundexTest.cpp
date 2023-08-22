#include <string>

class Soundex
{
public:
  std::string encode(std::string const& word) const { return zeroPad(word); }

private:
  std::string zeroPad(std::string const& word) const { return word + "000"; }
};

#include "gmock/gmock.h"
using ::testing::Eq;

class SoundexEncoding : public testing::Test
{
public:
  Soundex soundex;
};

TEST_F(SoundexEncoding, RetainsSoleLetterOfOneLetterWord)
{
  auto encoded = soundex.encode("A");

  ASSERT_THAT(encoded, Eq("A000"));
}

TEST_F(SoundexEncoding, PadsWithZerosToEnsureThreeDigits)
{
  auto encoded = soundex.encode("I");

  ASSERT_THAT(encoded, Eq("I000"));
}