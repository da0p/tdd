#include "Soundex.h"
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

TEST_F(SoundexEncoding, ReplacesConsonantsWithAppropriateDigits)
{
  ASSERT_THAT(soundex.encode("Ax"), Eq("A200"));
}

TEST_F(SoundexEncoding, IgnoresNonAlphabetics)
{
  ASSERT_THAT(soundex.encode("A#"), Eq("A000"));
}

TEST_F(SoundexEncoding, ReplacesMultipleConsonantsWithDigits)
{
  ASSERT_THAT(soundex.encode("Acdl"), Eq("A234"));
}

TEST_F(SoundexEncoding, LimitsLengthToFourCharacters)
{
  ASSERT_THAT(soundex.encode("Dcdln").length(), Eq(4u));
}

TEST_F(SoundexEncoding, IgnoresVowelLikeLetters)
{
  ASSERT_THAT(soundex.encode("BaAeEiIoOuUhHyYcCdDlL"), Eq("B234"));
}

TEST_F(SoundexEncoding, IgnoresCaseWhenEncodingConsonants)
{
  ASSERT_THAT(soundex.encode("BCDL"), Eq(soundex.encode("Bcdl")));
}

TEST_F(SoundexEncoding, CombinesDuplicateCodesWhen2ndLetterDuplicates1st)
{
  ASSERT_THAT(soundex.encode("Bbcd"), Eq("B230"));
}

TEST_F(SoundexEncoding, DoesNotCombineDuplicateEncodingsSeparatedByVowels)
{
  ASSERT_THAT(soundex.encode("Jbob"), Eq("J110"));
}