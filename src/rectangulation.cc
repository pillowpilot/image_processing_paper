#include <rectangulation.h>

#include <iostream>

cv::Mat Rectangulation::getRandomSplitMatrix(const cv::Mat original_image) const
{
  assert(!original_image.empty());
  return doRandomSplitMatrix(original_image);
}

cv::Mat Rectangulation::getSplitMatrix() const
{
  return doSplitMatrix();
}

cv::Mat Rectangulation::prepareForVisualization(const cv::Mat split_matrix) const
{
  double min, max;
  cv::minMaxIdx(split_matrix, &min, &max);
  cv::Mat colorMapping;
  split_matrix.convertTo(colorMapping, CV_8UC1, 255 / (max-min), -min);
  cv::applyColorMap(colorMapping, colorMapping, cv::COLORMAP_JET);
  return colorMapping;
}
