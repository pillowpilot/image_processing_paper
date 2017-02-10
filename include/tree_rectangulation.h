#ifndef TREE_RECTANGULATION_H
#define TREE_RECTANGULATION_H

#include <rectangulation.h>
#include <optimizable.h>
#include <random_number_generator.h>

#include <queue>
#include <cassert>
#include <cmath>

class Node: public Optimizable
{
 public:
  enum class Orientation {Horizontal, Vertical};
  
 public:
  Node* left_child_;
  Node* right_child_;
  Node* parent_;
  int pivot_row_, pivot_column_;
  int region_width_, region_height_;
  int split_line_;
  int height_;
  Orientation orientation_;

 public:
 Node():
  left_child_(nullptr), right_child_(nullptr), parent_(nullptr)//, orientation_(Orientation::Horizontal)
    { }

  ~Node()
    {
      delete left_child_;
      delete right_child_;
    }
  
  inline bool isLeaf() const
  {
    return left_child_ == nullptr && right_child_ == nullptr;
  }

  inline bool isRoot() const
  {
    return parent_ == nullptr;
  }

  inline int getRegionArea() const
  {
    return region_width_*region_height_;
  }

  inline int getSplitLineMaximumValue() const
  {
    if( orientation_ == Orientation::Horizontal )
      return region_height_ + 1;
    else
      return region_width_ + 1;
  }

  inline int doGetNumberOfParameters() const override
  {
    if( isLeaf() ) return 0;
    else return 1;
  }

  inline double doGetParameter(int index) const override
  {
    return split_line_;
  }

  inline void doSetParameter(int index, double value) override
  {
    split_line_ = std::round(value);      
  }
};

class TreeRectangulation: public Rectangulation
{
 protected:
  Node* root_;
  int split_matrix_rows_, split_matrix_columns_;
  int minimum_area_;
  
 public:
  TreeRectangulation(const cv::Mat original_image, double alpha=0.1);
  ~TreeRectangulation()
    {
      delete root_;
    }
  
 private:
  void buildRandomTree();
  cv::Mat doRandomSplitMatrix(const cv::Mat original_image) const override;
  cv::Mat doSplitMatrix() const override;
  int doGetNumberOfParameters() const override;
  
  // TODO Implement!
  double doGetParameter(int index) const override {}
  void doSetParameter(int index, double value) override {}
};

#endif
