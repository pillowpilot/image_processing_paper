#ifndef CLAHE_TRANSFORMATION_H
#define CLAHE_TRANSFORMATION_H

#include <vector>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include <transformation.h>
#include <rectangulation.h>

class CLAHETransformation: public Transformation
{
 protected:
  std::unordered_map<int, int> region_id_transformation_mapping_;
  std::vector< std::vector<int> > transformations_;
  
 public:
  CLAHETransformation(const cv::Mat original_image, const Rectangulation& rectangulation, double clip_limit=0.75);
  CLAHETransformation(const cv::Mat original_image, const cv::Mat split_matrix, double clip_limit=0.75);

 private:
  int doGetTransformation(int region_id, int intensity) const override;
  bool doCheckRegionId(int region_id) const override;
  void buildHistograms(const cv::Mat original_image, const cv::Mat split_matrix);
  void applyClipLimit(double clip_limit);
  void distributeUpperArea(std::vector<int>& histogram, int cut_limit, int upper_area, int lower_area);
  void makeHistogramsCumulative();
  void normalizeCumulativeHistograms();
  
};

#endif
