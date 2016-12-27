#ifndef AHE_TRANSFORMATION_H
#define AHE_TRANSFORMATION_H

#include <vector>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include <transformation.h>

class AHETransformation: public Transformation
{
 private:
  std::unordered_map<int, int> region_id_transformation_mapping_;
  std::vector< std::vector<int> > transformations_;
  
 public:
  AHETransformation(const cv::Mat original_image, const cv::Mat split_matrix);

 private:
  int doGetTransformation(int region_id, int intensity) const override;
  bool doCheckRegionId(int region_id) const override;
  void buildHistograms(const cv::Mat original_image, const cv::Mat split_matrix);
  void makeHistogramsCumulative();
  void normalizeCumulativeHistograms();
  
};

#endif
