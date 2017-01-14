#ifndef _METRIC_H_
#define _METRIC_H_

#include <opencv2/opencv.hpp>

class Metric{
 private:
  virtual double calculateMetric() = 0;

 public:
  double getMetric();
  
};

#endif
