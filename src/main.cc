#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include <circle_rectangulation.h>

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
  cv::waitKey();

  return 0;
}
