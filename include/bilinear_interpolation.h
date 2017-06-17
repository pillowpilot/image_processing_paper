#ifndef BILINEAR_INTERPOLATION
#define BILINEAR_INTERPOLATION

#include <map>
#include <cmath>

#include <interpolation_scheme.h>
#include <transformation.h>

#include <opencv2/opencv.hpp>

class BilinearInterpolation: public InterpolationScheme
{
 private:
  enum class PointClassification {Single, DoubleHorizontal , DoubleVertical, Quadruple};
  
  std::map<int, std::pair<int, int> > region_point_mapping_;
  std::map<int, int> region_left_mapping_;
  std::map<int, int> region_right_mapping_;
  std::map<int, int> region_up_mapping_;
  std::map<int, int> region_down_mapping_;
  
  int grid_rows_, grid_columns_;
  int left_limit_, right_limit_, bottom_limit_, upper_limit_;
  
 public:
  BilinearInterpolation(int grid_rows, int grid_columns);
  
 private:
  cv::Mat doInterpolation(const cv::Mat original_image, const cv::Mat split_matrix, const Transformation& transformation) override;

  void buildRegionPointMapping(const cv::Mat split_matrix);
  void calculateLimits(const cv::Mat split_matrix);
  void calculateNeighbors(const cv::Mat split_matrix);
  PointClassification classifyPoint(int row, int column) const;
  int interpolateSingle(const cv::Mat split_matrix, const Transformation& transformation, int original_intensity, int row, int column) const;
  int interpolateDoubleVertical(const cv::Mat split_matrix, const Transformation& transformation, int original_intensity, int row, int column) const;
  int interpolateDoubleHorizontal(const cv::Mat split_matrix, const Transformation& transformation, int original_intensity, int row, int column) const;
  int interpolateQuadruple(const cv::Mat split_matrix, const Transformation& transformation, int original_intensity, int row, int column) const;  
};

#endif
