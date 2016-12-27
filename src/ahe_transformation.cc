#include <ahe_transformation.h>

AHETransformation::AHETransformation(const cv::Mat original_image, const cv::Mat split_matrix)
{
  // TODO Assert that original_image and split_matrix are valid
  // TODO Assert that split_matrix is valid for original_image

  buildHistograms(original_image, split_matrix);
  makeHistogramsCumulative();
  normalizeCumulativeHistograms();
}

void AHETransformation::buildHistograms(const cv::Mat original_image, const cv::Mat split_matrix)
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
	      transformations_.push_back( std::vector<int>(256) );
	    }

	  transformations_[histogram_id][intensity]++;
	}
    }
}

void AHETransformation::makeHistogramsCumulative()
{
  for(auto& histogram: transformations_)
    {
      for(int intensity = 1; intensity < histogram.size(); intensity++)
	{
	  histogram[intensity] += histogram[intensity-1];
	}
    }
}

void AHETransformation::normalizeCumulativeHistograms()
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

int AHETransformation::doGetTransformation(int region_id, int intensity) const
{
  // TODO Raise exception if region_id is not valid
  auto iterator = region_id_transformation_mapping_.find(region_id);
  int transformation_id = iterator->second;
  return transformations_[transformation_id][intensity];
}

bool AHETransformation::doCheckRegionId(int region_id) const
{
  auto iterator = region_id_transformation_mapping_.find(region_id);
  return iterator != end(region_id_transformation_mapping_);
}
