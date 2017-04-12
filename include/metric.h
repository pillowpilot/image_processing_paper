#ifndef _METRIC_H_
#define _METRIC_H_

#include <opencv2/opencv.hpp>

class Metric{
 private:
  virtual double calculateMetric(const cv::Mat image) = 0;

 public:
  double getMetric(const cv::Mat image);
  
};

#endif
