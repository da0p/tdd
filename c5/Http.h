#ifndef HTTP_H_
#define HTTP_H_

#include <string>

class Http
{
public:
  virtual ~Http() = default;
  virtual void initialize() = 0;
  virtual std::string get(std::string const& url) const = 0;
};

#endif