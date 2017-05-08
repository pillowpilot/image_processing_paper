#ifndef TREE_RECTANGULATION_ZERO_H
#define TREE_RECTANGULATION_ZERO_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <cassert>

#include <opencv2/opencv.hpp>

#include <rectangulation.h>
#include <random_number_generator.h>

class TreeRectangulationZero: public Rectangulation
{
 protected:
  class Node
  {
  public:
    enum class Orientation {Horizontal, Vertical};

  public:
    Node *left_child_, *right_child_, *parent_;
    int height_;
    double split_line_;
    Orientation orientation_;

  public:
  Node():
    left_child_(nullptr), right_child_(nullptr), parent_(nullptr), height_(0)
      {
	Random& random = Random::getInstance();
	split_line_ = random.nextDouble();
	if( random.nextInt(0, 2) == 0 )
	  orientation_ = Orientation::Horizontal;
	else
	  orientation_ = Orientation::Vertical;
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
  }; // End of Node class

 protected:
  int rows_, columns_;
  Node* root_;
  std::vector<Node*> node_to_parameter_mapping_;
  
 public:
  TreeRectangulationZero(int rows, int columns, int maximum_height=4);
  // TODO Add TreeRectangulationZero( const TreeRectangulationZero& other);
  friend std::ostream& operator<<(std::ostream& os, const TreeRectangulationZero::Node& n);
  friend std::ostream& operator<<(std::ostream& os, const TreeRectangulationZero& t);

 protected:
  Node* buildRandomTree(int maximum_height) const;
  std::vector<Node*> getNodesToParametersMapping() const;
  void getSplitMatrixHelper(cv::Mat split_matrix, const Node* node, std::pair<int, int> pivot,
			    std::pair<int, int> dimentions, int* region_id) const;
  
  // TODO make static
  void dump(std::ostringstream& oss, const Node* n, std::string prefix="", bool isTail=true) const;

  // Optimizable methods
  int doGetNumberOfParameters() const override;
  double doGetParameter(int index) const override;
  void doSetParameter(int index, double value) override;

  // Rectangulation methods
  cv::Mat doRandomSplitMatrix(int rows, int columns) const override;
  cv::Mat doSplitMatrix() const override;

};
#endif
