#ifndef CIRCLE_RECTANGULATION_H
#define CIRCLE_RECTANGULATION_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <rectangulation.h>
#include <random_number_generator.h>

class Circle: public Optimizable
{
 public:
  int row_, column_, radius_;
  
  int doGetNumberOfParameters() const override
  {
    return 3;
  }
  
  double doGetParameter(int index) const override
  {
    switch(index)
      {
      case 0:
	return row_;
      case 1:
	return column_;
      case 2:
	return radius_;
      }
  }
  
  void doSetParameter(int index, double value) override
  {
    switch(index)
      {
      case 0:
	row_ = (int) value;
	break;
      case 1:
	column_ = (int) value;
	break;
      case 2:
	radius_ = (int) value;
	break;
      }
  }

};

class CircleRectangulation: public Rectangulation
{
 private:
  int split_matrix_rows_, split_matrix_columns_;
  std::vector<Circle> circles_;
  
 public:
  CircleRectangulation(int rows, int columns, int number_of_circles);
  ~CircleRectangulation() { }
  
 private:
  int doGetNumberOfParameters() const override;
  double doGetParameter(int index) const override;
  void doSetParameter(int index, double value) override;
  cv::Mat doRandomSplitMatrix(int rows, int columns) const override;
  cv::Mat doSplitMatrix() const override;
  
};

#endif
