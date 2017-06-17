#include <bilinear_interpolation.h>

BilinearInterpolation::BilinearInterpolation(int grid_rows, int grid_columns):
  grid_rows_(grid_rows), grid_columns_(grid_columns)
{
}

cv::Mat BilinearInterpolation::doInterpolation(const cv::Mat original_image, const cv::Mat split_matrix, const Transformation& transformation)
{
  const int rows = split_matrix.rows;
  const int columns = split_matrix.cols;
  
  buildRegionPointMapping(split_matrix);
  calculateLimits(split_matrix);
  calculateNeighbors(split_matrix);

  cv::Mat interpolated_image = cv::Mat::zeros(rows, columns, CV_8U);
  for(int row = 0; row < rows; row++)
    {
      for(int column = 0; column < columns; column++)
	{
	  int original_intensity = original_image.at<uchar>(row, column);
	  int interpolated_intensity;
	  PointClassification classification = classifyPoint(row, column);
	  switch(classification)
	    {
	    case PointClassification::Single:
	      interpolated_intensity =
		interpolateSingle(split_matrix, transformation, original_intensity, row, column);
	      break;
	    case PointClassification::Quadruple:
	      interpolated_intensity =
		interpolateQuadruple(split_matrix, transformation, original_intensity, row, column);
	      break;
	    case PointClassification::DoubleHorizontal:
	      interpolated_intensity =
		interpolateDoubleHorizontal(split_matrix, transformation, original_intensity, row, column);
	      break;
	    case PointClassification::DoubleVertical:
	      interpolated_intensity =
		interpolateDoubleVertical(split_matrix, transformation, original_intensity, row, column);
	      break;	    
	    }
	  
	  if( interpolated_intensity < 0 || 255 < interpolated_intensity )
	    std::cout << "Intensity out of range!" << std::endl; // TODO Replace with exception
	  interpolated_image.at<uchar>(row, column) = (uchar)interpolated_intensity;
	}
    }

  return interpolated_image;
}

void BilinearInterpolation::buildRegionPointMapping(const cv::Mat split_matrix)
{
  const int rows = split_matrix.rows;
  const int columns = split_matrix.cols;
  
  for(int row = 0; row < rows; row++)
    {
      for(int column = 0; column < columns; column++)
	{
	  // TODO Fast access
	  const int region_id = split_matrix.at<int>(row, column);
	  if( region_point_mapping_.find(region_id) == region_point_mapping_.end() )
	    {
	      int region_rows = 1;
	      while( row + region_rows + 1 < rows &&
		     split_matrix.at<int>(row + region_rows + 1, column) == region_id )
		region_rows++;
	      
	      int region_columns = 1;
	      while( column + region_columns + 1 < columns &&
		     split_matrix.at<int>(row, column + region_columns + 1) == region_id )
		region_columns++;

	      const int region_center_row = row + region_rows/2;
	      const int region_center_column = column + region_columns/2;

	      const std::pair<int, int> region_center = std::make_pair(region_center_row, region_center_column);
	      region_point_mapping_[region_id] = region_center;
	    }
	}
    }
}

void BilinearInterpolation::calculateLimits(const cv::Mat split_matrix)
{
  const int rows = split_matrix.rows;
  const int columns = split_matrix.cols;

  int region_id;
  std::pair<int, int> region_center;
  // TODO Fast access
  region_id = split_matrix.at<int>(0, 0);
  region_center = region_point_mapping_[region_id];
  upper_limit_ = region_center.first; // region_center.first == region_center_row
  left_limit_ = region_center.second; // region_center.second == region_center_column

  region_id = split_matrix.at<int>(rows-1, columns-1);
  region_center = region_point_mapping_[region_id];
  bottom_limit_ = region_center.first; // region_center.first == region_center_row
  right_limit_ = region_center.second; // region_center.second == region_center_column
}

void BilinearInterpolation::calculateNeighbors(const cv::Mat split_matrix)
{
  const int rows = split_matrix.rows;
  const int columns = split_matrix.cols;
  
  for(int row = 0; row < rows; row++)
    {
      for(int column = 1; column < columns; column++)
	{
	  int previous_region_id = split_matrix.at<int>(row, column-1);
	  int region_id = split_matrix.at<int>(row, column);
	  if( previous_region_id != region_id )
	    {	
	    region_right_mapping_[previous_region_id] = region_id;
	    region_left_mapping_[region_id] = previous_region_id;
	    }	    
	}      
    }

  for(int column = 0; column < columns; column++)
    {
      for(int row = 1; row < rows; row++)
	{
	  int previous_region_id = split_matrix.at<int>(row-1, column);
	  int region_id = split_matrix.at<int>(row, column);
	  if( previous_region_id != region_id )
	    {
	      region_up_mapping_[region_id] = previous_region_id;
	      region_down_mapping_[previous_region_id] = region_id;
	    }
	}
    }
}

BilinearInterpolation::PointClassification BilinearInterpolation::classifyPoint(int row, int column) const
{
  if( left_limit_ < column && column < right_limit_ &&
      upper_limit_ < row && row < bottom_limit_)
    return PointClassification::Quadruple;

  if( row <= upper_limit_ || bottom_limit_ <= row )
    if( left_limit_ < column && column < right_limit_ )
      return PointClassification::DoubleHorizontal;
    else
      return PointClassification::Single;
  else
    return PointClassification::DoubleVertical;
}

int BilinearInterpolation::interpolateSingle(const cv::Mat split_matrix, const Transformation& transformation, int original_intensity, int row, int column) const
{
  int region_id = split_matrix.at<int>(row, column);
  return transformation.getTransformation(region_id, original_intensity); 
}

int BilinearInterpolation::interpolateQuadruple(const cv::Mat split_matrix, const Transformation& transformation, int original_intensity, int row, int column) const
{
  int container_region_id = split_matrix.at<int>(row, column);
  const std::pair<int, int> container_region_center = region_point_mapping_.at(container_region_id);

  int horizontal_region_id, vertical_region_id, diagonal_region_id;
  if( column < container_region_center.second )
    horizontal_region_id = region_left_mapping_.at(container_region_id);
  else    
    horizontal_region_id = region_right_mapping_.at(container_region_id);
    
  if( row < container_region_center.first )
    vertical_region_id = region_up_mapping_.at(container_region_id);
  else
    vertical_region_id = region_down_mapping_.at(container_region_id);

  if( column < container_region_center.second )
    diagonal_region_id = region_left_mapping_.at(vertical_region_id);
  else
    diagonal_region_id = region_right_mapping_.at(vertical_region_id);

  const std::pair<int, int> horizontal_region_center = region_point_mapping_.at(horizontal_region_id);
  const std::pair<int, int> vertical_region_center = region_point_mapping_.at(vertical_region_id);
  const std::pair<int, int> diagonal_region_center = region_point_mapping_.at(diagonal_region_id);

  double total_area =
    std::abs(container_region_center.first-vertical_region_center.first)*
    std::abs(container_region_center.second-horizontal_region_center.second);
  
  double weight_diagonal =
    std::abs(row-container_region_center.first)*std::abs(column-container_region_center.second)/total_area;
  double weight_vertical =
    std::abs(row-horizontal_region_center.first)*std::abs(column-horizontal_region_center.second)/total_area;
  double weight_horizontal =
    std::abs(row-vertical_region_center.first)*std::abs(column-vertical_region_center.second)/total_area;
  double weight_container =
    std::abs(row-diagonal_region_center.first)*std::abs(column-diagonal_region_center.second)/total_area;

  int container_intensity = transformation.getTransformation(container_region_id, original_intensity);
  int horizontal_intensity = transformation.getTransformation(horizontal_region_id, original_intensity);
  int vertical_intensity = transformation.getTransformation(vertical_region_id, original_intensity);
  int diagonal_intensity = transformation.getTransformation(diagonal_region_id, original_intensity);

  int interpolated_intensity = (int) std::round(
					  weight_container*container_intensity +
					  weight_horizontal*horizontal_intensity +
					  weight_vertical*vertical_intensity +
					  weight_diagonal*diagonal_intensity
					  );
  
  return interpolated_intensity;  
}

int BilinearInterpolation::interpolateDoubleVertical(const cv::Mat split_matrix, const Transformation& transformation, int original_intensity, int row, int column) const
{
  int container_region_id = split_matrix.at<int>(row, column);
  const std::pair<int, int> container_region_center = region_point_mapping_.at(container_region_id);

  int neighbor_region_id;
  if( row < container_region_center.first )
    neighbor_region_id = region_up_mapping_.at(container_region_id);
  else
    neighbor_region_id = region_down_mapping_.at(container_region_id);

  const std::pair<int, int> neighbor_region_center = region_point_mapping_.at(neighbor_region_id);

  double total_length = std::abs(container_region_center.first-neighbor_region_center.first);

  double weight_container = std::abs(row-neighbor_region_center.first)/total_length;
  double weight_neighbor = std::abs(row-container_region_center.first)/total_length;

  int container_intensity = transformation.getTransformation(container_region_id, original_intensity);
  int neighbor_intensity = transformation.getTransformation(neighbor_region_id, original_intensity);
    
  int interpolated_intensity = (int)std::round(weight_container*container_intensity +
					       weight_neighbor*neighbor_intensity);
  return interpolated_intensity;
}

int BilinearInterpolation::interpolateDoubleHorizontal(const cv::Mat split_matrix, const Transformation& transformation, int original_intensity, int row, int column) const
{
  int container_region_id = split_matrix.at<int>(row, column);
  const std::pair<int, int> container_region_center = region_point_mapping_.at(container_region_id);

  int neighbor_region_id;
  if( column < container_region_center.second )
    neighbor_region_id = region_left_mapping_.at(container_region_id);
  else
    neighbor_region_id = region_right_mapping_.at(container_region_id);

  const std::pair<int, int> neighbor_region_center = region_point_mapping_.at(neighbor_region_id);

  double total_length = std::abs(container_region_center.second-neighbor_region_center.second);

  double weight_container = std::abs(column-neighbor_region_center.second)/total_length;
  double weight_neighbor = std::abs(column-container_region_center.second)/total_length;

  int container_intensity = transformation.getTransformation(container_region_id, original_intensity);
  int neighbor_intensity = transformation.getTransformation(neighbor_region_id, original_intensity);
    
  int interpolated_intensity = (int)std::round(weight_container*container_intensity +
					       weight_neighbor*neighbor_intensity);
  return interpolated_intensity;
}
