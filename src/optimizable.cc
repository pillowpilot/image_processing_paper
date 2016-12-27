#include <optimizable.h>

int Optimizable::getNumberOfParameters() const
{
  int number_of_parameters = doGetNumberOfParameters();
  assert(0 <= number_of_parameters);
  return number_of_parameters;
}

double Optimizable::getParameter(int index) const
{
  assert(0 <= index);
  assert(index < getNumberOfParameters());
  return doGetParameter(index);
}

void Optimizable::setParameter(int index, double value)
{
  assert(0 <= index);
  assert(index < getNumberOfParameters());
  return doSetParameter(index, value);
}
