#ifndef SOUNDEX_H
#define SOUNDEX_H

#include <string>

namespace {
constexpr size_t MaxCodeLength{4};
}

class Soundex
{
public:
  std::string encode(std::string const& word) const
  {
    return zeroPad(head(word) + encodedDigits(word));
  }

private:
  std::string head(std::string const& word) const { return word.substr(0, 1); }

  std::string encodedDigits(std::string const& word) const
  {
    if(word.length() > 1) {
      return encodedDigit(word[1]);
    }

    return "";
  }

  std::string encodedDigit(char letter) const
  {
    if(letter == 'b') {
      return "1";
    }

    if(letter == 'c') {
      return "2";
    }

    return "";
  }

  std::string zeroPad(std::string const& word) const
  {
    auto zerosNeeded = MaxCodeLength - word.length();

    return word + std::string(zerosNeeded, '0');
  }
};

#endif