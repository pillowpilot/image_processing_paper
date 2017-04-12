#ifndef OPTIMIZATOR_H
#define OPTIMIZATOR_H

#include <dlib/optimization.h>
#include <opencv2/opencv.hpp>

#include <rectangulation.h>
#include <interpolation_scheme.h>
#include <metric.h>
#include <transformation.h>
#include <evaluator.h>

class Optimizator
{
 protected:
  double last_evaluation_;
  cv::Mat optimized_image_;
  dlib::matrix<double, 0, 1> parameters_;
  
 public:
  void optimize(const cv::Mat original_image,
		Rectangulation& rectangulation,
		Transformation& transformation,
		InterpolationScheme& interpolation_scheme,
		Metric& metric);
  
  cv::Mat getImage() const;
  dlib::matrix<double, 0, 1> getParameters() const;
  double getLastEvaluation() const;
};


#endif
