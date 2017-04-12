#ifndef _MEAN_SQUARE_ERROR_H_
#define _MEAN_SQUARE_ERROR_H_

#include <cmath>
#include <opencv2/opencv.hpp>
#include <metric.h>

class MeanSquaredError: public Metric
{
 protected:
  const cv::Mat reference_image_;
  
 public:
  MeanSquaredError(const cv::Mat reference_image);

 protected:
  double calculateMetric(const cv::Mat image);
  
};

#endif
