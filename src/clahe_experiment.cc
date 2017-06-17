#include <iostream>

#include <clahe_transformation.h>
#include <grid_rectangulation.h>
#include <image_streamer.h>
#include <bilinear_interpolation.h>
#include <ahe_transformation.h>

#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>

void optimizeImage(const cv::Mat image)
{
  const int max_grid_rows = image.rows/2;
  const int max_grid_columns = image.cols/2;

  for(int grid_rows = 1; grid_rows <= max_grid_rows; grid_rows++)
    {
      for(int grid_columns = 1; grid_columns <= max_grid_columns; grid_columns++)
	{
	  double clip_limit = 0.1;
	  while( clip_limit <= 1.0 ){
	    
	    
	    clip_limit += 0.1;
	  }
	}
    }
}

int main(){

  using namespace boost::filesystem;

  const path root_path("../resources/xray_dataset");
  ImageStreamer imageStreamer(root_path);
  while(!imageStreamer.isEmpty()){
    const path image_path = imageStreamer.next();

    const cv::Mat image = cv::imread(image_path.string(), CV_LOAD_IMAGE_GRAYSCALE);

    const std::string window_name = image_path.string();
    cv::namedWindow(window_name, cv::WINDOW_NORMAL);
    //cv::namedWindow(window_name);
    //cv::imshow("Preview", image);

    GridRectangulation r(image.rows, image.cols, 10, 7);
    cv::Mat sm = r.getSplitMatrix();
    cv::Mat smv = r.prepareForVisualization(sm);
    cv::imshow(window_name, smv);
    
    cv::waitKey(1000);

    //GridRectangulation r(image.rows, image.cols, 10, 7);
    AHETransformation t(image, r.getSplitMatrix());
    BilinearInterpolation i(10, 7);
    cv::Mat interpolated = i.interpolate(image, r.getSplitMatrix(), t);

    cv::imshow(window_name, interpolated);
    cv::waitKey(5000);
  }
  
  return 0;
}
