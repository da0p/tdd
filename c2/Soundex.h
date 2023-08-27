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
    return zeroPad(upperFront(word) + tail(encodedDigits(word)));
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
    encodeHead(encoding, word);
    encodeTail(encoding, word);

    return encoding;
  }

  void encodeHead(std::string& encoding, std::string const& word) const
  {
    encoding += encodedDigit(word.front());
  }

  void encodeTail(std::string& encoding, std::string const& word) const
  {
    for(auto letter : tail(word)) {
      if(!isComplete(encoding)) {
        encodeLetter(encoding, letter);
      }
    }
  }

  void encodeLetter(std::string& encoding, char letter) const
  {
    auto digit = encodedDigit(letter);
    if(digit != NotADigit && digit != lastDigit(encoding)) {
      encoding += digit;
    }
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
    return encoding.length() == MaxCodeLength;
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

    auto found = encodings.find(lower(letter));
    return found == encodings.end() ? "*" : found->second;
  }

  char lower(char c) const
  {
    return std::tolower(static_cast<unsigned char>(c));
  }

  std::string zeroPad(std::string const& word) const
  {
    auto zerosNeeded = MaxCodeLength - word.length();

    return word + std::string(zerosNeeded, '0');
  }
};

#endif