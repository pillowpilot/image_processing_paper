#include <mean_squared_error.h>

MeanSquaredError::MeanSquaredError(const cv::Mat image_a, const cv::Mat image_b)
  :image_a(image_a), image_b(image_b)
{
  // TODO If image_a and image_b are incompatible, exception.
  assert(!image_a.empty());
  assert(!image_b.empty());
  assert(image_a.rows == image_b.rows);
  assert(image_a.cols == image_b.cols);
}

double MeanSquaredError::calculateMetric()
{
  const int rows = image_a.rows;
  const int columns = image_a.cols;

  double mean_squared_error = 0;
  for(int row = 0; row < rows; row++)
    {
      for(int column = 0; column < columns; column++)
	{
	  // TODO Fast access
	  const int intensity_a = image_a.at<uchar>(row, column);
	  const int intensity_b = image_b.at<uchar>(row, column);
	  
	  double diference_squared = std::pow(intensity_a - intensity_b, 2);

	  mean_squared_error += diference_squared;
	}
    }

  mean_squared_error /= (double)(rows) * columns;
  
  return mean_squared_error;
}
