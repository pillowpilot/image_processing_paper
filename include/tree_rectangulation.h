#ifndef TREE_RECTANGULATION_H
#define TREE_RECTANGULATION_H

#include <queue>
#include <vector>
#include <cassert>
#include <string> // TODO Remove
#include <sstream> // TODO Remove
#include <iostream> // TODO Remove
#include <cmath>
#include <opencv2/opencv.hpp>
#include <rectangulation.h>
#include <optimizable.h>
#include <random_number_generator.h>

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
  left_child_(nullptr), right_child_(nullptr), parent_(nullptr)
    {
    }
  
 Node(const Node& other):
  left_child_(other.left_child_), right_child_(other.right_child_),
    pivot_row_(other.pivot_row_), pivot_column_(other.pivot_column_),
    region_width_(other.region_width_), region_height_(other.region_height_),
    split_line_(other.split_line_), height_(other.height_), orientation_(other.orientation_)
    {
    }

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
      return region_height_;
    else
      return region_width_;
  }

  inline int doGetNumberOfParameters() const override
  {
    if( isLeaf() ) return 0;
    else return 1;
  }

  inline double doGetParameter(int index) const override
  {
    if( orientation_ == Orientation::Horizontal )
      return split_line_ / region_height_;
    else
      return split_line_ / region_width_;
  }

  void doSetParameter(int index, double value) override;
  void fixInvariants();

  void printToCout() const;
};

class TreeRectangulation: public Rectangulation
{
 protected:
  std::vector<Node*> parameter_node_mapping_;
  Node* root_;
  int split_matrix_rows_, split_matrix_columns_;
  int minimum_area_;
  
 public:
  TreeRectangulation(const cv::Mat original_image, double alpha=0.1);
  TreeRectangulation(const TreeRectangulation& other);
  ~TreeRectangulation()
    {
      delete root_;
    }

  void checkTreeInvariants() const;
  
 private:
  Node* buildRandomTree(const cv::Mat original_image, const double alpha) const;  
  std::vector<Node*> buildParameterToNodeMapping(Node* root) const;
  cv::Mat doRandomSplitMatrix(const cv::Mat original_image) const override;
  cv::Mat doSplitMatrix() const override;
  int doGetNumberOfParameters() const override;
  double doGetParameter(int index) const override;
  void doSetParameter(int index, double value) override;
};

void printTree(std::ostringstream& oss, const Node* n, std::string prefix="", bool isTail=true)
{
  using namespace std;
  
  oss << prefix;
  if( isTail ) oss << "└── ";
  else oss << "├── ";

  oss << "(" << n->pivot_row_ << ", " << n->pivot_column_ << ")  ";
  oss << "[" << n->region_width_ << ", " << n->region_height_ << "]  ";
  oss << n->split_line_;
  
  oss << std::endl;
  
  if( !n->isLeaf() )
    {
      printTree(oss, n->left_child_, prefix + (isTail ? "    " : "│   "), false);
      printTree(oss, n->right_child_, prefix + (isTail ? "    " : "│   "), true);      
    }
}


std::ostream& operator<<(std::ostream& os, const Node* n)
{
  std::ostringstream oss;
  printTree(oss, n);
  os << oss.str();
  return os;
}

#endif
