#include <transformation.h>

int Transformation::getTransformation(int region_id, int intensity) const
{
  assert(0 <= intensity);
  assert(intensity < 256);
  assert(isRegionIdValid(region_id));
  const int transformated_intensity = doGetTransformation(region_id, intensity);
  assert(0 <= transformated_intensity);
  assert(transformated_intensity < 256);
  
  return transformated_intensity;
}

bool Transformation::isRegionIdValid(int region_id) const
{
  return doCheckRegionId(region_id);
}
