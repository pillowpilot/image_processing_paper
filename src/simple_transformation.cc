#include <simple_transformation.h>

int SimpleTransformation::doGetTransformation(int region_id, int intensity) const
{
  return intensity;
}

bool SimpleTransformation::doCheckRegionId(int region_id) const
{
  return true;
}
