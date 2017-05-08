#include <iostream>
#include <string>
#include <tree_rectangulation_zero.h>
#include <clahe_transformation.h>
#include <circle_interpolation.h>
#include <contrast_improvement_ratio.h>
#include <evaluator.h>
#include <image_streamer.h>

#include <dlib/optimization.h>

std::pair<double, cv::Mat> optimize(const cv::Mat original_image){
  const long long seed = 2017;
  
  Random& random = Random::getInstance();
  random.setSeed(seed);
  
  if( original_image.data == nullptr ){    
    std::cerr << "Image is null" << std::endl;
    return std::make_pair(0.0, cv::Mat::zeros(1, 1, CV_64F));
  }
  
  TreeRectangulationZero r(original_image.rows, original_image.cols, 2);
  cv::Mat split_matrix = r.getSplitMatrix();

  /*{
    double min, max;
    cv::minMaxIdx(split_matrix, &min, &max);
    cv::Mat colorMapping;
    split_matrix.convertTo(colorMapping, CV_8UC1, 255 / (max-min), -min);
    cv::applyColorMap(colorMapping, colorMapping, cv::COLORMAP_JET);

    const std::string smwname = "splitmatrix";
    //cv::namedWindow(smwname, CV_WINDOW_NORMAL);
    //cv::imshow(smwname, colorMapping);
    //cv::waitKey(0);
    }*/

  const double clip_limit = 0.65;
  const double radius = 4;
  CLAHETransformation transformation(original_image, r, clip_limit);
  CircleInterpolation interpolation(radius);
  ContrastImprovementRatio metric(original_image);

  Evaluator evaluator(original_image, r, transformation, interpolation, metric);
  double evaluation;

  const int number_of_parameters = r.getNumberOfParameters();
  dlib::matrix<double, 0, 1> starting_point(number_of_parameters);

  for(int index = 0; index < number_of_parameters; index++){
    double value = random.nextDouble();
    starting_point(index) = value;
  }

  evaluation = evaluator(starting_point);
  std::cout << "Initial: " << evaluation << std::endl;

  dlib::find_min_bobyqa(evaluator,
			starting_point,
			2*number_of_parameters,
			dlib::uniform_matrix<double>(number_of_parameters, 1, 0.0),
			dlib::uniform_matrix<double>(number_of_parameters, 1, 1.0),
			0.4,
			1e-6,
			500);

  evaluation = evaluator(starting_point);
  std::cout << "Final: " << evaluation << std::endl;

  cv::Mat optimized_image = evaluator.getOptimizedImage(starting_point);

  return std::make_pair(evaluation, optimized_image);
}

int main(){

  boost::filesystem::path path("..\\resources\\test_set\\");
  ImageStreamer imageStreamer(path);

  //cv::namedWindow("preview", CV_WINDOW_NORMAL);
  while( !imageStreamer.isEmpty() ){
    boost::filesystem::path imagepath = imageStreamer.next();
    std::cout << "Image: " << imagepath.string() << std::endl;

    cv::Mat image = cv::imread(imagepath.string(), CV_LOAD_IMAGE_GRAYSCALE);

    std::pair<double, cv::Mat> optimization_pair = optimize(image);

    boost::filesystem::path optimizedpath;
    optimizedpath /= imagepath.parent_path().parent_path();
    optimizedpath /= "optimized";
    optimizedpath /= imagepath.filename();

    std::cout << "Optimized: " << optimizedpath.string() << std::endl;

    cv::imwrite(optimizedpath.string(), optimization_pair.second);
    
  }
  
  return 0;
}
  
