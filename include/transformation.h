#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <cassert>
#include <opencv2/opencv.hpp>

class Transformation
{
 public:
  int getTransformation(int region_id, int intensity) const;
  bool isRegionIdValid(int region_id) const;

 private:
  virtual int doGetTransformation(int region_id, int intensity) const = 0;
  virtual bool doCheckRegionId(int region_id) const = 0;
  
};

#endif
