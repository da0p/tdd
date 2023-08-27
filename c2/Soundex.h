#ifndef SOUNDEX_H
#define SOUNDEX_H

#include <numeric>
#include <ranges>
#include <string>
#include <unordered_map>

namespace {
constexpr size_t MaxCodeLength{4};

const std::string NotADigit{"*"};
}

class Soundex
{
public:
  std::string encode(std::string const& word) const
  {
    return zeroPad(upperFront(word) + encodedDigits(tail(word)));
  }

private:
  std::string upperFront(std::string const& s) const
  {
    return std::string(1, std::toupper(static_cast<unsigned char>(s.front())));
  }

  std::string tail(std::string const& word) const { return word.substr(1); }

  std::string encodedDigits(std::string const& word) const
  {
    std::string encoding;
    for(auto letter : word) {
      if(isComplete(encoding)) {
        break;
      }

      auto digit = encodedDigit(letter);
      if(digit != NotADigit && digit != lastDigit(encoding)) {
        encoding += digit;
      }
    }
    return encoding;
  }

  std::string lastDigit(std::string const& encoding) const
  {
    if(encoding.empty()) {
      return NotADigit;
    }
    return std::string(1, encoding.back());
  }

  bool isComplete(std::string const& encoding) const
  {
    return encoding.length() == MaxCodeLength - 1;
  }

  std::string encodedDigit(char letter) const
  {
    const std::unordered_map<char, std::string> encodings{
      {'b', "1"},
      {'f', "1"},
      {'p', "1"},
      {'v', "1"},
      {'c', "2"},
      {'g', "2"},
      {'j', "2"},
      {'k', "2"},
      {'q', "2"},
      {'s', "2"},
      {'x', "2"},
      {'z', "2"},
      {'d', "3"},
      {'t', "3"},
      {'l', "4"},
      {'m', "5"},
      {'n', "5"},
      {'r', "6"}};

    auto found = encodings.find(letter);
    return found == encodings.end() ? "*" : found->second;
  }

  std::string zeroPad(std::string const& word) const
  {
    auto zerosNeeded = MaxCodeLength - word.length();

    return word + std::string(zerosNeeded, '0');
  }
};

#endif