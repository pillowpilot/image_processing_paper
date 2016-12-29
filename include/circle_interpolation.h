#ifndef CIRCLE_INTERPOLATION_H
#define CIRCLE_INTERPOLATION_H

#include <cmath>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include <interpolation_scheme.h>
#include <transformation.h>

class CircleInterpolation: public InterpolationScheme
{
 protected:
  std::unordered_map<int, int> region_area_mapping_;
  std::vector<std::pair<int, int>> area_offsets_;
  std::vector<std::pair<int, int>> right_offsets_;
  std::vector<std::pair<int, int>> bottom_offsets_;
  double radius_;
  
 public:
  CircleInterpolation(double radius);

 protected:
  cv::Mat doInterpolation(const cv::Mat original_image, const cv::Mat split_matrix, const Transformation& transformation);
  void calculateOffsets();
  int calculateInterpolation(int intensity, const Transformation& trasnformation) const;
  void calculateAreas(const cv::Mat split_matrix, const int center_row, const int center_column);
  void updateAreasGoRight(const cv::Mat split_matrix, const int center_row, const int center_column);
  void updateAreasGoDown(const cv::Mat split_matrix, const int center_row, const int center_column);
  int unlimitedFieldMapping(int x, int limit) const;
  
};

#endif
