#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <cassert>
#include <cmath>
#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>

#include <circle_rectangulation.h>
#include <ahe_transformation.h>
#include <circle_interpolation.h>
#include <normalized_mean_squared_error.h>
#include <image_streamer.h>

void printHeader(std::ostream& os, const std::vector<double> percentages)
{
  using namespace std;
  const string sep(",");
  vector<string> header{"id", "rows", "columns"};
  for(double percentage: percentages)
    {
      header.push_back("error_" + to_string(percentage));
      header.push_back("time_" + to_string(percentage));
    }
  
  os << header[0];
  for(int index = 1; index < header.size(); index++)
    {
      os << sep << header[index];
    }

  os << endl;
}

void printReport(std::ostream& os, const std::string id, const cv::Mat image, const std::vector<double> errors,
		 const std::vector<std::chrono::high_resolution_clock::time_point> time_points)
{
  const std::string sep(",");
  
  os << id << sep << image.rows << sep << image.cols;

  for(int index = 0; index < errors.size(); index++)
    {
      os << sep << errors[index];
      {
	using namespace std::chrono;
	os << sep << duration_cast<duration<double>>(time_points[index+1] - time_points[index]).count();
      }
    }

  os << std::endl;
}

int main(int argc, char** argv)
{
  using namespace boost::filesystem;
  using namespace std::chrono;
  
  assert(2 <= argc);
  const path root_path(argv[1]);
  std::cout << "Working with root directory: " << root_path.string() << std::endl;

  // TODO Add as optional parameter
  std::ofstream report;
  report.open(argv[2], std::ios::out); // TODO Add more checks (if any)

  // TODO Add as optional parameter
  const std::vector<double> percentages{0, 1, 2, 3, 4, 5, 7, 9, 10, 15, 20, 25};

  // TODO Add as optional parameter
  const long seed = 2017;
  const int number_of_circles = 40;
  
  Random& random = Random::getInstance();
  random.setSeed(seed);

  printHeader(std::cout, percentages);
  printHeader(report, percentages);

  ImageStreamer imageStreamer(root_path);
  while(!imageStreamer.isEmpty())
    {
      const path image_path = imageStreamer.next();

      const cv::Mat image = cv::imread(image_path.string(), CV_LOAD_IMAGE_GRAYSCALE);
      const int image_area = image.cols*image.rows;
      assert(!image.empty());

      CircleRectangulation rectangulation(image, number_of_circles);
      const cv::Mat split_matrix = rectangulation.getRandomSplitMatrix(image);
      AHETransformation transformation(image, split_matrix);

      const double reference_percentage = percentages[percentages.size()-1] / 100;
      const int radius = std::sqrt(reference_percentage*image_area/M_PI);
      CircleInterpolation reference_interpolator(radius);
      const cv::Mat interpolated_reference = reference_interpolator.interpolate(image, split_matrix, transformation);

      std::vector<double> errors;
      std::vector<high_resolution_clock::time_point> time_points;
      time_points.push_back(high_resolution_clock::now());

      for(double percentage: percentages)
	{
	  percentage /= 100;
	  const int radius = std::sqrt(percentage*image_area/M_PI);
	  CircleInterpolation interpolator(radius);
	  cv::Mat interpolated = interpolator.interpolate(image, split_matrix, transformation);

	  MeanSquaredError mse(interpolated_reference, interpolated);
	  double error = mse.getMetric();
	  
	  errors.push_back(error);
	  time_points.push_back(high_resolution_clock::now());
	}

      printReport(std::cout, image_path.string(), image, errors, time_points);
      printReport(report, image_path.string(), image, errors, time_points);

    }


  return 0;
}
