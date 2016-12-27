#ifndef SIMPLE_INTERPOLATOR_H
#define SIMPLE_INTERPOLATOR_H

#include <opencv2/opencv.hpp>
#include <interpolation_scheme.h>

class SimpleInterpolator: public InterpolationScheme
{  
 private:
  cv::Mat doInterpolation(const cv::Mat original_image, const cv::Mat split_matrix, const Transformation& transformation) override;
};

#endif
