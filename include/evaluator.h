#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <dlib/optimization.h>
#include <opencv2/opencv.hpp>
#include <rectangulation.h>
#include <interpolation_scheme.h>
#include <metric.h>
#include <transformation.h>

#include <iostream> //TODO remove

class Evaluator
{
 private:
  const cv::Mat original_image_;
  Rectangulation& rectangulation_;
  Transformation& transformation_;
  InterpolationScheme& interpolation_scheme_;
  Metric& metric_;
  
 public:
 Evaluator(const cv::Mat original_image,
	   Rectangulation& rectangulation,
	   Transformation& transformation,
	   InterpolationScheme& interpolation_scheme,
	   Metric& metric):
  original_image_(original_image),
    rectangulation_(rectangulation),
    transformation_(transformation),
    interpolation_scheme_(interpolation_scheme),
    metric_(metric)
    {
      
    }

  double operator()(const dlib::matrix<double, 0, 1>& args) const
  {
    for(int index = 0; index < args.nc(); index++)
      {
	const double parameter = args(index);
	rectangulation_.setParameter(index, parameter);
      }
    
    const cv::Mat split_matrix = rectangulation_.getSplitMatrix();
    
    const cv::Mat interpolated_image = interpolation_scheme_.interpolate(original_image_, split_matrix, transformation_);

    const double evaluation = metric_.getMetric(interpolated_image);
    
    return -evaluation;
  }

  cv::Mat getOptimizedImage(const dlib::matrix<double, 0, 1>& args) const
    {
      for(int index = 0; index < args.nc(); index++)
	{
	  const double parameter = args(index);
	  rectangulation_.setParameter(index, parameter);
	}
    
      const cv::Mat split_matrix = rectangulation_.getSplitMatrix();
    
      const cv::Mat interpolated_image = interpolation_scheme_.interpolate(original_image_, split_matrix, transformation_);

      return interpolated_image;
    }
};

#endif
