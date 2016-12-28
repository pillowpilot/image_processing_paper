#include <ahe_transformation.h>

AHETransformation::AHETransformation(const cv::Mat original_image, const cv::Mat split_matrix):
  CLAHETransformation(original_image, split_matrix, 1.0)
{
  // TODO Exception for original_image and split_matrix are valid
  // TODO Exception for split_matrix is valid for original_image
}
