#include <mean_squared_error.h>

MeanSquaredError::MeanSquaredError(const cv::Mat reference_image)
  :reference_image_(reference_image)
{
}

double MeanSquaredError::calculateMetric(const cv::Mat image)
{
  const int rows = reference_image_.rows;
  const int columns = reference_image_.cols;

  double mean_squared_error = 0;
  for(int row = 0; row < rows; row++)
    {
      for(int column = 0; column < columns; column++)
	{
	  // TODO Fast access
	  const int intensity_a = reference_image_.at<uchar>(row, column);
	  const int intensity_b = image.at<uchar>(row, column);
	  
	  double diference_squared = std::pow(intensity_a - intensity_b, 2);

	  mean_squared_error += diference_squared;
	}
    }

  mean_squared_error /= (double)(rows) * columns;
  
  return mean_squared_error;
}
