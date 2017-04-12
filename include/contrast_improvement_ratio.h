#ifndef CONTRAST_IMPROVEMENT_RATIO_H
#define CONTRAST_IMPROVEMENT_RATIO_H

#include <cassert>
#include <opencv2/opencv.hpp>
#include <metric.h>

class ContrastImprovementRatio: public Metric
{
 protected:
  const cv::Mat reference_image_;

 public:
  ContrastImprovementRatio(const cv::Mat reference_image);

 protected:
  double calculateMetric(const cv::Mat image) override;
  double calculateLocalContrast(const cv::Mat image, int row, int column) const;
  
};

#endif
