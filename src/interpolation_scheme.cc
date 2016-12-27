#include <interpolation_scheme.h>

cv::Mat InterpolationScheme::interpolate(const cv::Mat original_image, const cv::Mat split_matrix, const Transformation& transformation){
  assert(!original_image.empty());
  assert(!split_matrix.empty());
  assert(original_image.rows == split_matrix.rows);
  assert(original_image.cols == split_matrix.cols);
  
  return doInterpolation(original_image, split_matrix, transformation);
}
