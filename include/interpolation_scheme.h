#ifndef INTERPOLATION_SCHEME_H
#define INTERPOLATION_SCHEME_H

#include <cassert>
#include <opencv2/opencv.hpp>
#include <transformation.h>

class InterpolationScheme{
 public:
  cv::Mat interpolate(const cv::Mat original_image, const cv::Mat split_matrix, const Transformation& transformation);
  
 private:
  virtual cv::Mat doInterpolation(const cv::Mat original_image, const cv::Mat split_matrix, const Transformation& transformation) = 0;
    
};

#endif
