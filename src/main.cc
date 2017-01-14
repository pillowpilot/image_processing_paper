#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include <circle_rectangulation.h>
#include <simple_transformation.h>
#include <simple_interpolator.h>
#include <circle_interpolation.h>
#include <ahe_transformation.h>
#include <clahe_transformation.h>

#include <mean_squared_error.h>

int main(int argc, char* argv[]){

  const long seed = 2017;
  const std::string image_filename("../resources/lake.tif");
  cv::Mat original_image = cv::imread(image_filename, CV_LOAD_IMAGE_GRAYSCALE);
  const int number_of_circles = 40;

  Random& random = Random::getInstance();
  random.setSeed(seed);
  
  CircleRectangulation rectangulation(original_image, number_of_circles);
  cv::Mat split_matrix = rectangulation.getRandomSplitMatrix(original_image);
  cv::Mat visualizable = rectangulation.prepareForVisualization(split_matrix);
  
  cv::imshow("Original Image", original_image);
  cv::imshow("Split Matrix", visualizable);

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

  const int max_radius = 20;
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
  cv::waitKey();
  
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
