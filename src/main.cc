#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include <circle_rectangulation.h>
#include <simple_transformation.h>
#include <simple_interpolator.h>
#include <circle_interpolation.h>
#include <ahe_transformation.h>
#include <clahe_transformation.h>

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

  //SimpleTransformation transformation;
  AHETransformation transformation(original_image, split_matrix);
  //CLAHETransformation transformation(original_image, split_matrix, 0.3);
  //SimpleInterpolator interpolator;
  CircleInterpolation interpolator(5);

  cv::Mat interpolated_image = interpolator.interpolate(original_image, split_matrix, transformation);

  cv::imshow("Interpolated Image", interpolated_image);
  cv::waitKey();

  return 0;
}
