#ifndef AHE_TRANSFORMATION_H
#define AHE_TRANSFORMATION_H

#include <opencv2/opencv.hpp>
#include <clahe_transformation.h>

class AHETransformation: public CLAHETransformation
{
 public:
  AHETransformation(const cv::Mat original_image, const cv::Mat split_matrix);
  
};

#endif
