#ifndef SOUNDEX_H
#define SOUNDEX_H

#include <string>
#include <unordered_map>

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
    const std::unordered_map<char, std::string> encodings{
      {'b', "1"}, {'c', "2"}};

    return encodings.find(letter)->second;
  }

  std::string zeroPad(std::string const& word) const
  {
    auto zerosNeeded = MaxCodeLength - word.length();

    return word + std::string(zerosNeeded, '0');
  }
};

#endif