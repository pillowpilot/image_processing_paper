#include <normalized_mean_squared_error.h>

NormalizedMeanSquaredError::NormalizedMeanSquaredError(const cv::Mat image_a, const cv::Mat image_b):
  image_a_(image_a), image_b_(image_b)
{
  // TODO If image_a and image_b are incompatible, exception.
  assert(!image_a.empty());
  assert(!image_b.empty());
  assert(image_a.rows == image_b.rows);
  assert(image_a.cols == image_b.cols);
}

double NormalizedMeanSquaredError::calculateMetric()
{
  const int rows = image_a_.rows;
  const int columns = image_a_.cols;
    
  double sum_of_squares = 0;
  double sum_of_image_a = 0;
  double sum_of_image_b = 0;

  for(int row = 0; row < rows; row++)
    {
      for(int column = 0; column < columns; column++)
	{
	  // TODO Fast access
	  const int intensity_a = image_a_.at<uchar>(row, column);
	  const int intensity_b = image_b_.at<uchar>(row, column);

	  sum_of_squares += (intensity_a - intensity_b)*(intensity_a - intensity_b);
	  sum_of_image_a += intensity_a;
	  sum_of_image_b += intensity_b;
	}
    }

  return sum_of_squares/(sum_of_image_a*sum_of_image_b);
}
