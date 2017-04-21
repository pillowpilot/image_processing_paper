#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include <circle_rectangulation.h>
#include <tree_rectangulation.h>
#include <simple_transformation.h>
#include <simple_interpolator.h>
#include <circle_interpolation.h>
#include <ahe_transformation.h>
#include <clahe_transformation.h>
#include <histogram_spread.h>

#include <mean_squared_error.h>

#include <dlib/optimization.h>
#include <evaluator.h>
#include <iomanip>

int main(int argc, char* argv[]){

  //const long seed = (argc > 1)?atoi(argv[1]):2018;
  const long seed = 2018;
  
  const std::string image_filename("../resources/lake.tif");
  cv::Mat original_image = cv::imread(image_filename, CV_LOAD_IMAGE_GRAYSCALE);

  Random& random = Random::getInstance();
  random.setSeed(seed);
  
  TreeRectangulation rectangulation(original_image, 0.1);

  cv::Mat splitMatrix = rectangulation.getSplitMatrix();
  cv::Mat visualizable = rectangulation.prepareForVisualization(splitMatrix);
  cv::imshow("", visualizable);
  cv::imwrite("sm1.png", visualizable);

  for(int x = 2; x < 20; x++){
    dlib::matrix<double, 0, 1> parameters;
    parameters.set_size(rectangulation.getNumberOfParameters());
    
    for(int i = 0; i < rectangulation.getNumberOfParameters(); i++)
      {
	const double parameter = random.nextDouble();
	//rectangulation.setParameter(i, parameter);
	parameters(i) = parameter;
      }
    for(int i = 0; i < rectangulation.getNumberOfParameters(); i++)
      {
	//const double parameter = rectangulation.getParameter(i);
	const double parameter = parameters(i);
	std::cout << parameter << std::endl;
      }
    std::cout << std::endl;
    cv::Mat sm = rectangulation.getSplitMatrix();
    //cv::Mat v = rectangulation.prepareForVisualization(sm);
    //cv::imwrite("sm"+std::to_string(x)+".png", v);

    AHETransformation transformation(original_image, sm);
    CircleInterpolation interpolation_scheme(20);
    HistogramSpread metric;
    Evaluator evaluator(original_image, rectangulation, transformation, interpolation_scheme, metric);

    double score = evaluator(parameters);
    
    //std::cout << "sm"+std::to_string(x)+".png" << std::endl;
    std::cout << score << std::endl;
  }

  return 0;
  
  
  //const int index_ = atoi(argv[1]);
  //const double value_ = atof(argv[2]);
  //rectangulation.setParameter(index_, value_);

  
  
  /*splitMatrix = rectangulation.getSplitMatrix();
  visualizable = rectangulation.prepareForVisualization(splitMatrix);
  cv::imshow("", visualizable);
  cv::imwrite("sm2.png", visualizable);
  
  return 0;

  Evaluator evaluator(original_image, rectangulation);
  dlib::matrix<double, 0, 1> args;
  args.set_size(rectangulation.getNumberOfParameters());
  
  for(int i = 0; i < args.nr(); i++)
    {
      const double parameter = random.nextDouble();
      args(i) = parameter;
    }
  
  std::cout << std::endl;
  std::cout << "Arguments: " << args << std::endl;

  std::cout << "Evaluation: " << std::fixed << std::setprecision(15) << evaluator(args) << std::endl;

  const int numberOfParameters = rectangulation.getNumberOfParameters();
  dlib::matrix<double, 0, 1> point;
  point.set_size(numberOfParameters);
  
  for(int i = 0; i < args.nr(); i++)
    {
      const double parameter = random.nextDouble();
      point(i) = parameter;
    }

  const double eps = 1e-6;
  dlib::find_max_bobyqa(evaluator,
			 point,
			 2*numberOfParameters,
			 dlib::uniform_matrix<double>(numberOfParameters, 1, 0.0+eps),
			 dlib::uniform_matrix<double>(numberOfParameters, 1, 1.0-eps),
			 0.1,
			 eps,
			 100
			 );

  std::cout << "Solution: " << point << std::endl;
			 
  /*{
  //SimpleTransformation transformation;
  AHETransformation transformation(original_image, split_matrix);
  //CLAHETransformation transformation(original_image, split_matrix, 0.3);
  //SimpleInterpolator interpolator;
  CircleInterpolation interpolator(20);

  cv::Mat interpolated_image = interpolator.interpolate(original_image, split_matrix, transformation);

  cv::imshow("Interpolated Image", interpolated_image);
  cv::waitKey();
  }*/

  // Minimum Squared Error exp

  /*const int max_radius = 20;
  const int divisions = 10;
  
  AHETransformation transformation(original_image, split_matrix);
  CircleInterpolation interpolator_reference(max_radius);
  
  cv::Mat interpolated_reference = interpolator_reference.interpolate(original_image, split_matrix, transformation);

  CircleInterpolation interpolator_compare(5);
  cv::Mat interpolated_compare = interpolator_compare.interpolate(original_image, split_matrix, transformation);

  {
  MeanSquaredError metric(interpolated_reference, interpolated_reference);
  std::cout << "MSE(ref, ref) = " << metric.getMetric() << std::endl;
  }
  {
  MeanSquaredError metric(interpolated_reference, interpolated_compare);
  std::cout << "MSE(ref, cmp) = " << metric.getMetric() << std::endl;
  }
  cv::imshow("Reference", interpolated_reference);
  cv::imshow("Comparing", interpolated_compare);
  cv::waitKey();*/
  
  /*
  for(int multiplier = 1; multiplier <= divisions; multiplier++)
    {
      const int radius = max_radius * multiplier / divisions;
      CircleInterpolation inner_interpolator(radius);

      cv::Mat interpolated_comparable = inner_interpolator.interpolate(original_image, split_matrix, transformation);

      //MeanSquaredError metric(interpolated_reference, interpolated_comparable);
      //double mse = metric.getMetric();
      double mse = 10.0;

      std::cout << mse << std::endl;
    }*/

  return 0;
}
