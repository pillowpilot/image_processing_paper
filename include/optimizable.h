#ifndef OPTIMIZABLE_H
#define OPTIMIZABLE_H

#include <cassert>

class Optimizable
{
 public:
  int getNumberOfParameters() const;
  double getParameter(int index) const;
  void setParameter(int index, double value);

 private:
  virtual int doGetNumberOfParameters() const = 0;
  virtual double doGetParameter(int index) const = 0;
  virtual void doSetParameter(int index, double value) = 0;
};

#endif
