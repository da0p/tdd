#ifndef WORK_H
#define WORK_H

#include <functional>

class Work
{
public:
  static int const DefaultId{0};
  Work(int id = DefaultId)
    : mId{id}
    , mExecuteFunction{[] {}}
  {
  }
  Work(std::function<void()> executionFunction, int id = DefaultId)
    : mId{id}
    , mExecuteFunction{executionFunction}
  {
  }

  void execute() { mExecuteFunction(); }

  int id() const { return mId; }

private:
  int mId;
  std::function<void()> mExecuteFunction;
};

#endif