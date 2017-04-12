#include <optimizator.h>

void Optimizator::optimize(const cv::Mat original_image,
	      Rectangulation& rectangulation,
	      Transformation& transformation,
	      InterpolationScheme& interpolation_scheme,
	      Metric& metric)
{
  Evaluator evaluator(original_image, rectangulation, transformation, interpolation_scheme, metric);

  const int number_of_parameters = rectangulation.getNumberOfParameters();
  parameters_ = dlib::randm(number_of_parameters, 1);

  const double rho_begin = 0.4;
  
  dlib::find_min_bobyqa(evaluator,
			parameters_, 
			2*number_of_parameters,
			dlib::uniform_matrix<double>(number_of_parameters, 1, 0),
			dlib::uniform_matrix<double>(number_of_parameters, 1, 1),
			rho_begin, 
			1e-6,
			100);

  last_evaluation_ = evaluator(parameters_);
  // TODO Apply parameters_ to image and store it in optimized_image_
}

cv::Mat Optimizator::getImage() const
{
  return optimized_image_;
}

dlib::matrix<double, 0, 1> Optimizator::getParameters() const
{
  return parameters_;
}

double Optimizator::getLastEvaluation() const
{
  return last_evaluation_;
}
