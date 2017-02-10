#include <tree_rectangulation.h>
#include <iostream>

TreeRectangulation::TreeRectangulation(const cv::Mat original_image, double alpha):
  split_matrix_rows_(original_image.rows), split_matrix_columns_(original_image.cols), minimum_area_(alpha*original_image.rows*original_image.cols), root_(nullptr)
{
  
}

cv::Mat TreeRectangulation::doSplitMatrix() const
{
  cv::Mat split_matrix(split_matrix_rows_, split_matrix_columns_, CV_32S);
  std::queue<Node*> nodes;
  nodes.push(root_);

  int region_id = 1;
  while( !nodes.empty() ){
    Node* node = nodes.front();
    nodes.pop();

    if( node->isLeaf() )
      {
	for(int row_offset = 0; row_offset < node->region_height_; row_offset++)
	  for(int column_offset = 0; column_offset < node->region_width_; column_offset++)
	    {
	      const int row = node->pivot_row_+row_offset;
	      const int column = node->pivot_column_+column_offset;
	      split_matrix.at<int>(row, column) = region_id;
	    }
	region_id++;
      }
    else
      {
	nodes.push(node->left_child_);
	nodes.push(node->right_child_);
      }
  }

  return split_matrix;
}

cv::Mat TreeRectangulation::doRandomSplitMatrix(const cv::Mat original_image) const
{
  const double alpha = 0.1; // TODO Magic number
  const int minimum_area = alpha*original_image.rows*original_image.cols;
  
  Random& random = Random::getInstance();
  std::queue<Node*> tasks;

  Node* root = new Node;
  root->pivot_row_ = root->pivot_column_ = 0;
  root->region_height_ = original_image.rows;
  root->region_width_ = original_image.cols;
  root->height_ = 0;

  TreeRectangulation tree_rectangulation(original_image, alpha);
  tree_rectangulation.root_ = root;

  tasks.push(root);

  int region_id = 0;
  while( !tasks.empty() ){
    Node* node = tasks.front();
    tasks.pop();

    if( minimum_area < node->getRegionArea() )
      {
	if( random.nextInt(0, 2) == 0 )
	  node->orientation_ = Node::Orientation::Horizontal;
	else
	  node->orientation_ = Node::Orientation::Vertical;
	  
	const int split_line = random.nextInt(1, node->getSplitLineMaximumValue()-1);
	node->split_line_ = split_line;

	Node* left_child = new Node;
	Node* right_child = new Node;
	left_child->parent_ = right_child->parent_ = node;
	left_child->height_ = right_child->height_ = node->height_+1;
	node->left_child_ = left_child;
	node->right_child_ = right_child;	
	
	if( node->orientation_ == Node::Orientation::Horizontal )
	  {
	    left_child->pivot_row_ = node->pivot_row_;
	    left_child->pivot_column_ = node->pivot_column_;
	    left_child->region_width_ = node->region_width_;
	    left_child->region_height_ = node->split_line_;

	    right_child->pivot_row_ = node->pivot_row_+node->split_line_;
	    right_child->pivot_column_ = node->pivot_column_;
	    right_child->region_width_ = node->region_width_;
	    right_child->region_height_ = node->region_height_-node->split_line_;
	  }
	else	  
	  {
	    left_child->pivot_row_ = node->pivot_row_;
	    left_child->pivot_column_ = node->pivot_column_;
	    left_child->region_width_ = node->split_line_;
	    left_child->region_height_ = node->region_height_;

	    right_child->pivot_row_ = node->pivot_row_;
	    right_child->pivot_column_ = node->pivot_column_+node->split_line_;
	    right_child->region_width_ = node->region_width_-node->split_line_;
	    right_child->region_height_ = node->region_height_;
	  }
	
	tasks.push(left_child);
	tasks.push(right_child);	
      }
  }

  return tree_rectangulation.getSplitMatrix();
}

int TreeRectangulation::doGetNumberOfParameters() const
{
  int number_of_parameters = 0;
  
  std::queue<Node*> nodes;
  nodes.push(root_);
  while( !nodes.empty() )
    {
      Node* node = nodes.front();
      nodes.pop();

      number_of_parameters+= node->getNumberOfParameters();

      if( node->left_child_ != nullptr ) nodes.push(node->left_child_);
      if( node->right_child_ != nullptr ) nodes.push(node->right_child_);
    }

  return number_of_parameters;
}


