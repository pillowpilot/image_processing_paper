#ifndef _NORMALIZED_MEAN_SQUARED_ERROR_H_
#define _NORMALIZED_MEAN_SQUARED_ERROR_H_

#include <opencv2/opencv.hpp>
#include <mean_squared_error.h>

class NormalizedMeanSquaredError: public Metric
{
 protected:
  const cv::Mat image_a_, image_b_;
  
 public:
  NormalizedMeanSquaredError(const cv::Mat image_a, const cv::Mat image_b);

 protected:
  double calculateMetric();
  
};

#endif
