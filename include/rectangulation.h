#ifndef RECTANGULATION_H
#define RECTANGULATION_H

#include <cassert>
#include <opencv2/opencv.hpp>
#include <optimizable.h>

class Rectangulation: public Optimizable
{
 public:
  cv::Mat getRandomSplitMatrix(const cv::Mat original_image) const;
  cv::Mat getSplitMatrix() const;
  cv::Mat prepareForVisualization(const cv::Mat split_matrix) const;
  
 private:
  virtual cv::Mat doRandomSplitMatrix(const cv::Mat original_image) const = 0;
  virtual cv::Mat doSplitMatrix() const = 0;
  
};

#endif
