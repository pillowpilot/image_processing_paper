#ifndef SIMPLE_TRANSFORMATION_H
#define SIMPLE_TRANSFORMATION_H

#include <vector>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include <transformation.h>

class SimpleTransformation: public Transformation
{
 private:
  int doGetTransformation(int region_id, int intensity) const override;
  bool doCheckRegionId(int region_id) const override;
  
};

#endif
