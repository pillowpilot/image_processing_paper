#include <contrast_improvement_ratio.h>
#include <iostream>

#define debug(x) std::cout << (#x) << " = " << (x) << std::endl;

ContrastImprovementRatio::ContrastImprovementRatio(const cv::Mat reference_image):
  reference_image_(reference_image)
{
  
}

double ContrastImprovementRatio::calculateMetric(const cv::Mat image)
{
  assert(image.rows == reference_image_.rows);
  assert(image.cols == reference_image_.cols);
  
  const int rows = image.rows;
  const int columns = image.cols;

  // TODO Expand to all pixels!
  double sum_of_differences = 0;
  double sum_of_squares = 0;
  for(int row = 1; row < rows-1; row++)
    {
      for(int column = 1; column < columns-1; column++)
	{
	  const double image_lc = calculateLocalContrast(image, row, column);
	  const double reference_image_lc = calculateLocalContrast(reference_image_, row, column);

	  sum_of_differences += std::pow(image_lc - reference_image_lc, 2);
	  sum_of_squares += std::pow(image_lc, 2);

	}
    }

  const double cir = sum_of_differences / sum_of_squares;
  return cir;
}

double ContrastImprovementRatio::calculateLocalContrast(const cv::Mat image, int row, int column) const
{
  const int number_of_neighbors = 8;
  const int row_offsets[] = {-1, -1, -1, 0, 1, 1, 1, 0};
  const int column_offsets[] = {-1, 0, 1, 1, 1, 0, -1, -1};

  int surrounding_region_sum = 0;
  for(int neighbor = 0; neighbor < number_of_neighbors; neighbor++)
    {
      const int neighbor_row = row + row_offsets[neighbor];
      const int neighbor_column = column + column_offsets[neighbor];
      // TODO Fast access
      const int neighbor_intensity = image.at<uchar>(neighbor_row, neighbor_column);
      surrounding_region_sum += neighbor_intensity;
    }

  const double surrounding_region_average = (double)surrounding_region_sum / number_of_neighbors;
  const double center_region_average = (double)image.at<uchar>(row, column);

  double local_contrast = std::abs(center_region_average - surrounding_region_average);

  if( std::abs(center_region_average + surrounding_region_average) != 0.0 )
    local_contrast /= std::abs(center_region_average + surrounding_region_average);
  else
    local_contrast = 0.0;

  assert(0.0 <= local_contrast && local_contrast <= 1.0);
  return local_contrast;
}

