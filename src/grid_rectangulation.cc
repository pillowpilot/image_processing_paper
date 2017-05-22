#include <grid_rectangulation.h>

GridRectangulation::GridRectangulation(int image_rows, int image_columns, int grid_rows, int grid_columns):
  image_rows_(image_rows), image_columns_(image_columns), grid_rows_(grid_rows), grid_columns_(grid_columns)
{
  if(image_rows_ < grid_rows_)
    grid_rows_ = image_rows_; // TODO Raise an exception
  if(image_columns_ < grid_columns_)
    grid_columns_ = image_columns_; // Raise an exception
  
  if(image_rows_%grid_rows_ != 0)
    grid_rows_--;
  if(image_columns_%grid_columns_ != 0)
    grid_columns_--;
}

cv::Mat GridRectangulation::doSplitMatrix() const
{
  const int region_rows = image_rows_/grid_rows_;
  const int region_columns = image_columns_/grid_columns_;

  cv::Mat split_matrix = cv::Mat::zeros(image_rows_, image_columns_, CV_32S);

  for(int row=0; row < image_rows_; row++)
    {
      for(int column=0; column < image_columns_; column++)
	{
	  const int region_id = row/region_rows*grid_rows_ + column/region_columns;
	  split_matrix.at<int>(row, column) = region_id;
	}
    }
  
  return split_matrix;
}

cv::Mat GridRectangulation::doRandomSplitMatrix(int image_rows, int image_columns) const
{
  Random& random = Random::getInstance();

  const int grid_rows = random.nextInt(1, 10); // TODO Magic number
  const int grid_columns = random.nextInt(1, 10); // TODO Magic number
  GridRectangulation rectangulation(image_rows, image_columns, grid_rows, grid_columns);

  cv::Mat split_matrix = rectangulation.getSplitMatrix();
  return split_matrix;
}

int GridRectangulation::doGetNumberOfParameters() const
{
  return 2;
}

double GridRectangulation::doGetParameter(int index) const
{
  if( index == 0 )
    return grid_rows_;
  else
    return grid_columns_;
}

void GridRectangulation::doSetParameter(int index, double value)
{
  int final_value = std::round(value);
  if( index == 0 )
    grid_rows_ = final_value;
  else
    grid_columns_ = final_value;
}
