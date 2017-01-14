#ifndef _MEAN_SQUARE_ERROR_H_
#define _MEAN_SQUARE_ERROR_H_

#include <cmath>
#include <opencv2/opencv.hpp>
#include <metric.h>

class MeanSquaredError: public Metric
{
 protected:
  const cv::Mat image_a, image_b;
  
 public:
  MeanSquaredError(const cv::Mat image_a, const cv::Mat image_b);

 protected:
  double calculateMetric();
  
};

#endif
