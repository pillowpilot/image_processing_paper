#include <clahe_transformation.h>

CLAHETransformation::CLAHETransformation(const cv::Mat original_image, const cv::Mat split_matrix, double clip_limit)
{
  // TODO Exception for original_image and split_matrix are valid
  // TODO Exception for split_matrix is valid for original_image

  buildHistograms(original_image, split_matrix);
  applyClipLimit(clip_limit);
  makeHistogramsCumulative();
  normalizeCumulativeHistograms();
}

void CLAHETransformation::buildHistograms(const cv::Mat original_image, const cv::Mat split_matrix)
{
  const int rows = original_image.rows;
  const int columns = original_image.cols;  

  for(int row = 0; row < rows; row++)
    {
      for(int column = 0; column < columns; column++)
	{
	  // TODO Fast access
	  const int intensity = original_image.at<uchar>(row, column);
	  const int region_id = split_matrix.at<int>(row, column);
	  int histogram_id;

	  const auto iterator = region_id_transformation_mapping_.find(region_id);
	  if( iterator != end(region_id_transformation_mapping_) )
	    histogram_id = iterator->second;
	  else
	    {
	      histogram_id = transformations_.size();
	      region_id_transformation_mapping_.insert(std::make_pair(region_id, histogram_id));
	      transformations_.push_back( std::vector<int>(256) ); // TODO Magic number
	    }

	  transformations_[histogram_id][intensity]++;
	}
    }
}

void CLAHETransformation::applyClipLimit(double clip_limit)
{
  // TODO Refactor
  for(auto& histogram: transformations_)
    {
      int max_value = 0;
      for(int frecuency: histogram)
	if(max_value < frecuency)
	  max_value = frecuency;

      int cut_limit = (max_value+1) * clip_limit;
      if(cut_limit == 0)
	cut_limit = 1;

      int upper_area = 0;
      int lower_area = 0;
      for(int& frecuency: histogram){
	if(cut_limit < frecuency)
	  {
	  upper_area += frecuency - cut_limit;
	  frecuency = cut_limit;
	  }
	lower_area += frecuency;
      }

      distributeUpperArea(histogram, cut_limit, upper_area, lower_area);
    }
}

void CLAHETransformation::distributeUpperArea(std::vector<int>& histogram, int cut_limit, int upper_area, int lower_area)
{
  if(upper_area < histogram.size()*cut_limit - lower_area)
    {
      int intensity = 0;
      while(0 < upper_area)
	{
	  if(histogram[intensity] < cut_limit)
	    {
	      histogram[intensity]++;
	      upper_area--;
	    }
			  
	  intensity = (intensity+1) % histogram.size();
	}
    }
  else
    {
      for(int& frecuency: histogram)
	frecuency = cut_limit;
    } 
}

void CLAHETransformation::makeHistogramsCumulative()
{
  for(auto& histogram: transformations_)
    {
      for(int intensity = 1; intensity < histogram.size(); intensity++)
	{
	  histogram[intensity] += histogram[intensity-1];
	}
    }
}

void CLAHETransformation::normalizeCumulativeHistograms()
{
  for(auto& cumulative_histogram: transformations_)
    {
      const int max_intensity = cumulative_histogram[cumulative_histogram.size()-1];
      for(int intensity = 0; intensity < cumulative_histogram.size(); intensity++)
	{
	  cumulative_histogram[intensity] =
	    cumulative_histogram[intensity] * 255 / max_intensity; // TODO Magic number
	}
    }
}

int CLAHETransformation::doGetTransformation(int region_id, int intensity) const
{
  // TODO Raise exception if region_id is not valid
  auto iterator = region_id_transformation_mapping_.find(region_id);
  int transformation_id = iterator->second;
  return transformations_[transformation_id][intensity];
}

bool CLAHETransformation::doCheckRegionId(int region_id) const
{
  auto iterator = region_id_transformation_mapping_.find(region_id);
  return iterator != end(region_id_transformation_mapping_);
}
