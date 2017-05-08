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

  const long seed = (argc > 1)?atoi(argv[1]):5;
  std::cout << "seed=" << seed << std::endl;
  //const long seed = 2019;
  
  const std::string image_filename("../resources/dummy_image_10x10.png");
  cv::Mat original_image = cv::imread(image_filename, CV_LOAD_IMAGE_GRAYSCALE);

  if( original_image.data == nullptr )
    std::cerr << "Original image is null" << std::endl;

  const std::string window_name = "original_image";
  cv::namedWindow(window_name);
  //cv::imshow(window_name, original_image);
  //cv::waitKey(0);

  Random& random = Random::getInstance();
  random.setSeed(seed);

  TreeRectangulation rectangulation(original_image.rows, original_image.cols, 0.1);

  cv::Mat splitMatrix = rectangulation.getSplitMatrix();
  cv::Mat visualizable = rectangulation.prepareForVisualization(splitMatrix);

  if( visualizable.data == nullptr )
    std::cerr << "Visualizable is null" << std::endl;

  const std::string map_window = "map";
  cv::namedWindow(map_window, CV_WINDOW_NORMAL);
  cv::imshow(map_window, visualizable);
  cv::imwrite("sm1.png", visualizable);
  cv::waitKey(50); // for /l %x in (1, 1, 1000) do TRTest.exe %x
  
  return 0;
}
