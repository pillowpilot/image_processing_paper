#include <simple_interpolator.h>

cv::Mat SimpleInterpolator::doInterpolation(const cv::Mat original_image, const cv::Mat split_matrix, const Transformation& transformation)
{
  const int rows = original_image.rows;
  const int columns = original_image.cols;

  cv::Mat interpolated_image(rows, columns, CV_8U);

  for(int row = 0; row < rows; row++)
    {
      for(int column = 0; column < columns; column++)
	{
	  // TODO Fast access
	  const int original_intensity = original_image.at<uchar>(row, column);
	  const int region_id = split_matrix.at<int>(row, column);
	  const int interpolated_intensity = transformation.getTransformation(region_id, original_intensity);
	  interpolated_image.at<uchar>(row, column) = interpolated_intensity;
	}
    }

  return interpolated_image;
}
