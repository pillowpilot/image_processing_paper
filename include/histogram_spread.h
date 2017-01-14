#ifndef _HISTOGRAM_SPREAD_H_
#define _HISTOGRAM_SPREAD_H_

#include <vector>
#include <opencv2/opencv.hpp>
#include <metric.h>

class HistogramSpread: public Metric{
 protected:
  std::vector<double> histogram_;
  
 public:
  HistogramSpread(const cv::Mat image);
  
 protected:
  double calculateMetric();
  void buildHistogram(const cv::Mat image);
  void makeHistogramCumulative();
  void normalizeCumulativeHistogram();
  int getFirstQuartile() const;
  int getThirdQuartile() const;
  
};

#endif
