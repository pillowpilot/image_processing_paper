#include <circle_interpolation.h>

CircleInterpolation::CircleInterpolation(double radius):
  radius_(radius)
{
  calculateOffsets();
}

cv::Mat CircleInterpolation::doInterpolation(const cv::Mat original_image, const cv::Mat split_matrix, const Transformation& transformation)
{
  const int rows = original_image.rows;
  const int columns = original_image.cols;
  
  cv::Mat interpolated_image(rows, columns, CV_8U);

  for(int row = 0; row < rows; row++)
    {
      calculateAreas(split_matrix, row, 0);
      // TODO Fast access
      int intensity = original_image.at<uchar>(row, 0);
      int interpolated_intensity = calculateInterpolation(intensity, transformation);
      interpolated_image.at<uchar>(row, 0) = interpolated_intensity;
  
      for(int column = 1; column < columns; column++)
	{
	  updateAreasGoRight(split_matrix, row, column-1);
	  // TODO Fast access
	  const int intensity = original_image.at<uchar>(row, column);
	  const int interpolated_intensity = calculateInterpolation(intensity, transformation);
	  // TODO Fast access
	  interpolated_image.at<uchar>(row, column) = interpolated_intensity;
	}
    }

  return interpolated_image;
}

void CircleInterpolation::calculateAreas(const cv::Mat split_matrix, const int center_row, const int center_column)
{
  const int rows = split_matrix.rows;
  const int columns = split_matrix.cols;
  
  region_area_mapping_.clear();
  
  for(auto& offsets: area_offsets_)
    {
      const int row_offset = offsets.first;
      const int column_offset = offsets.second;
      const int reflected_row = unlimitedFieldMapping(center_row+row_offset, rows-1);
      const int reflected_column = unlimitedFieldMapping(center_column+column_offset, columns-1);
	      
      // TODO Fast access
      const int region_id = split_matrix.at<int>(reflected_row, reflected_column);
      auto iterator = region_area_mapping_.find(region_id);
	      
      if(iterator != end(region_area_mapping_))		
	iterator->second++;
      else
	region_area_mapping_.insert(std::make_pair(region_id, 1));
    }
}

void CircleInterpolation::updateAreasGoRight(const cv::Mat split_matrix, const int center_row, const int center_column)
{
  const int rows = split_matrix.rows;
  const int columns = split_matrix.cols;
  
  for(auto& offset: right_offsets_)
    {
      int row_offset = offset.first;
      int column_offset = offset.second+1;
      int reflected_row = unlimitedFieldMapping(center_row+row_offset, rows-1);
      int reflected_column = unlimitedFieldMapping(center_column+column_offset, columns-1);
      
      // TODO Fast access
      int region_id = split_matrix.at<int>(reflected_row, reflected_column);
      auto iterator = region_area_mapping_.find(region_id);
      if(iterator != end(region_area_mapping_))
	iterator->second++;
      else
	region_area_mapping_.insert(std::make_pair(region_id, 1));

      column_offset = -offset.second;
      reflected_column = unlimitedFieldMapping(center_column+column_offset, columns-1);
      
      // TODO Fast access
      region_id = split_matrix.at<int>(reflected_row, reflected_column);
      iterator = region_area_mapping_.find(region_id);
      iterator->second--;
      if(iterator->second == 0)
	region_area_mapping_.erase(region_id);
    }
}

void CircleInterpolation::updateAreasGoDown(const cv::Mat split_matrix, const int center_row, const int center_column)
{
  const int rows = split_matrix.rows;
  const int columns = split_matrix.cols;
  for(auto& offset: bottom_offsets_)
    {
      int row_offset = offset.first+1;
      int column_offset = offset.second;
      int reflected_row = unlimitedFieldMapping(center_row+row_offset, rows-1);
      int reflected_column = unlimitedFieldMapping(center_column+column_offset, columns-1);
      
      // TODO Fast access
      int region_id = split_matrix.at<int>(reflected_row, reflected_column);
      auto iterator = region_area_mapping_.find(region_id);
      if(iterator != end(region_area_mapping_))
	iterator->second++;
      else
	region_area_mapping_.insert(std::make_pair(region_id, 1));

      row_offset = -offset.first;
      reflected_row = unlimitedFieldMapping(center_row+row_offset, rows-1);

      region_id = split_matrix.at<int>(reflected_row, reflected_column);
      iterator = region_area_mapping_.find(region_id);
      iterator->second--;
      if(iterator->second == 0)
	region_area_mapping_.erase(region_id);
    }
}

int CircleInterpolation::calculateInterpolation(int intensity, const Transformation& transformations) const
{  
  const double circle_area = area_offsets_.size();
  
  double interpolated_intensity = 0;
  for(auto& region_weight: region_area_mapping_)
    {
      const int region_id = region_weight.first;
      const double weight = (double)(region_weight.second) / circle_area;
      const double transformated_intensity = transformations.getTransformation(region_id, intensity);
      
      interpolated_intensity += weight*transformated_intensity;
    }
  
  interpolated_intensity = std::round(interpolated_intensity);
  if(interpolated_intensity < 0)
    interpolated_intensity = 0;
  if(255 < interpolated_intensity)
    interpolated_intensity = 255;

  return interpolated_intensity;
}

void CircleInterpolation::calculateOffsets()
{
  const double squaredRadius = std::pow(radius_, 2);
  
  area_offsets_.reserve(squaredRadius*M_PI+1);
  right_offsets_.reserve(2*radius_+1);
  bottom_offsets_.reserve(2*radius_+1);
  
  for(int row = -radius_; row <= radius_; row++)
    {
      for(int column = -radius_; column <= radius_; column++)
	{
	  const double squaredDistance = row*row + column*column;
	  if(squaredDistance <= squaredRadius)
	    {
	      area_offsets_.push_back(std::make_pair(row, column));
	    }
	}
      
      auto last_offset = area_offsets_[area_offsets_.size() - 1];
      right_offsets_.push_back(last_offset);
    }

  for(auto& offsets: right_offsets_)
    {
      const int row_offset = offsets.first;
      const int column_offset = offsets.second;
      auto bottom_offset = std::make_pair(column_offset, -row_offset);
      bottom_offsets_.push_back(bottom_offset);
    }

  sort(begin(bottom_offsets_), end(bottom_offsets_));
}

int CircleInterpolation::unlimitedFieldMapping(int x, int limit) const
{
  if( x < 0 )
    x = -x;
  if( 2*limit <= x )
    x %= 2*limit;
  if( limit <= x )
    x = limit - (x-limit);
  return x;
}
