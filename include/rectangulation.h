#ifndef RECTANGULATION_H
#define RECTANGULATION_H

#include <cassert>
#include <opencv2/opencv.hpp>
#include <optimizable.h>

class Rectangulation: public Optimizable
{
 public:
  cv::Mat getRandomSplitMatrix(int rows, int column) const;
  cv::Mat getSplitMatrix() const;
  cv::Mat prepareForVisualization(const cv::Mat split_matrix) const;
  
 private:
  virtual cv::Mat doRandomSplitMatrix(int rows, int columns) const = 0;
  virtual cv::Mat doSplitMatrix() const = 0;
  
};

#endif
