#include <circle_rectangulation.h>
#include <iostream>

CircleRectangulation::CircleRectangulation(cv::Mat original_image, int number_of_circles):
  split_matrix_rows_(original_image.rows), split_matrix_columns_(original_image.cols), circles_(number_of_circles)
{
}

cv::Mat CircleRectangulation::doRandomSplitMatrix(const cv::Mat original_image) const
{
  Random& random = Random::getInstance();
  const int rows = original_image.rows;
  const int columns = original_image.cols;
  const int max_radius = 100; // TODO Magic number
  const int number_of_circles = random.nextInt(1, 100); // TODO Magic Numbers

  CircleRectangulation rectangulation(original_image, number_of_circles);

  const int number_of_parameters = rectangulation.getNumberOfParameters();
  for(int index = 0; index < number_of_parameters; index++)
    {
      double value;
      switch(index % 3)
	{
	case 0: // Row
	  value = random.nextDouble()*rows;
	  break;
	case 1: // Column
	  value = random.nextDouble()*columns;
	  break;
	case 2: // Radius;
	  value = random.nextDouble()*max_radius;
	  break;
	}
      rectangulation.setParameter(index, value);
    }

  return rectangulation.doSplitMatrix();
}

cv::Mat CircleRectangulation::doSplitMatrix() const
{
  // TODO Generalize type of matrix
  cv::Mat split_matrix(split_matrix_rows_, split_matrix_columns_, CV_32S);

  int region_id = 0;
  for(const Circle& circle: circles_){
    const int radius = circle.radius_;
    const int center_row = circle.row_;
    const int center_column = circle.column_;
    
    for(int row_offset = -radius; row_offset <= radius; row_offset++)
      {
	for(int column_offset = -radius; column_offset <= radius; column_offset++)
	  {
	    const double distanceSquared = std::pow(row_offset, 2) + std::pow(column_offset, 2);
	    const int row = center_row + row_offset;
	    const int column = center_column + column_offset;

	    // TODO Refactor (probably new wrapper class for cv::Mat)
	    if(distanceSquared <= std::pow(radius, 2) &&
	       0 <= row && 0 <= column && row < split_matrix_rows_ && column < split_matrix_columns_)
	      {
		// TODO Fast access
		split_matrix.at<int>(row, column) = region_id;
	      }
	  }
      }

    region_id++;
  }
  
  return split_matrix;
}

int CircleRectangulation::doGetNumberOfParameters() const
{
  return circles_.size() * 3;
}

double CircleRectangulation::doGetParameter(int index) const
{
  // TODO number_of_parameters must be divisible by 3 (every circle has 3 params)
  return circles_[index / 3].getParameter(index % 3);
}

void CircleRectangulation::doSetParameter(int index, double value)
{
  // TODO number_of_parameters must be divisible by 3 (every circle has 3 params)
  circles_[index / 3].setParameter(index % 3, value);
}
