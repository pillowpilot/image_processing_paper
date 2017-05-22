#ifndef GRID_RECTANGULATION_H
#define GRID_RECTANGULATION_H

#include <rectangulation.h>
#include <random_number_generator.h>

class GridRectangulation: public Rectangulation
{
 private:
  int image_rows_, image_columns_;
  int grid_rows_, grid_columns_;
  
 public:
  GridRectangulation(int image_rows, int image_columns, int grid_rows, int grid_columns);

 private:
  cv::Mat doRandomSplitMatrix(int image_rows, int image_columns) const override;
  cv::Mat doSplitMatrix() const override;

  int doGetNumberOfParameters() const override;
  double doGetParameter(int index) const override;
  void doSetParameter(int index, double value) override;
};

#endif
