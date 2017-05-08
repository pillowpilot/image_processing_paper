#include <rectangulation.h>

cv::Mat Rectangulation::getRandomSplitMatrix(int rows, int columns) const
{
  return doRandomSplitMatrix(rows, columns);
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
