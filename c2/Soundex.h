#ifndef SOUNDEX_H
#define SOUNDEX_H

#include <string>

class Soundex
{
public:
  std::string encode(std::string const& word) const { return zeroPad(word); }

private:
  std::string zeroPad(std::string const& word) const { return word + "000"; }
};

#endif