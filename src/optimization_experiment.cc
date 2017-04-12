#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include <tree_rectangulation.h>
#include <circle_interpolation.h>
#include <ahe_transformation.h>
#include <clahe_transformation.h>
#include <histogram_spread.h>
#include <contrast_improvement_ratio.h>

#include <dlib/optimization.h>
#include <evaluator.h>
#include <optimizator.h>

using namespace std;

int main(){
  const long seed = 2018;
  
  const std::string image_filename("../resources/lake.tif");
  cv::Mat original_image = cv::imread(image_filename, CV_LOAD_IMAGE_GRAYSCALE);

  Random& random = Random::getInstance();
  random.setSeed(seed);

  TreeRectangulation rectangulation(original_image, 0.1);
  AHETransformation transformation(original_image, rectangulation);
  CircleInterpolation interpolation_scheme(20);
  HistogramSpread metric;
  Evaluator evaluator(original_image, rectangulation, transformation, interpolation_scheme, metric);

  
  Optimizator optimizator;
  optimizator.optimize(original_image,
		       rectangulation,
		       transformation,
		       interpolation_scheme,
		       metric);
  const double score = optimizator.getLastEvaluation();

  std::cout << "Score :" << score << std::endl;
  return 0;

}
