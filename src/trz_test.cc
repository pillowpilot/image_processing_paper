#include <iostream>
#include <string>
#include <tree_rectangulation_zero.h>

int main(){

  const long long seed = 2017;
  Random& random = Random::getInstance();
  random.setSeed(seed);
  
  std::string image_filepath = "../resources/dummy_image_10x10.png";
  image_filepath = "../resources/lake.tif";
  cv::Mat original_image = cv::imread(image_filepath, CV_LOAD_IMAGE_GRAYSCALE);

  if( original_image.data == nullptr )
    std::cerr << "Image is null" << std::endl;

  const std::string wname = "image";
  //cv::namedWindow(wname, CV_WINDOW_NORMAL);
  //cv::imshow(wname, original_image);
  //cv::waitKey(0);
  
  TreeRectangulationZero r(7);
  std::cout << r << std::endl;
  cv::Mat split_matrix = r.getSplitMatrix(10, 10);

  //return 0;

  {
    double min, max;
    cv::minMaxIdx(split_matrix, &min, &max);
    cv::Mat colorMapping;
    split_matrix.convertTo(colorMapping, CV_8UC1, 255 / (max-min), -min);
    cv::applyColorMap(colorMapping, colorMapping, cv::COLORMAP_JET);

    const std::string smwname = "splitmatrix";
    cv::namedWindow(smwname, CV_WINDOW_NORMAL);
    cv::imshow(smwname, colorMapping);
    cv::waitKey(0);
  }

  return 0;
}
  
