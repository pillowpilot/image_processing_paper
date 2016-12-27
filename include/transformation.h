#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <opencv2/opencv.hpp>

class Transformation
{
 public:
  cv::Mat getTransformation();

 private:
  virtual cv::Mat doTransformation() = 0;
  
};

#endif
