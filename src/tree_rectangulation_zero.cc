#include <tree_rectangulation_zero.h>

TreeRectangulationZero::TreeRectangulationZero(const cv::Mat original_image, int maximum_height):
  rows_(original_image.rows), columns_(original_image.cols)
{
  root_ = buildRandomTree(maximum_height);
  node_to_parameter_mapping_ = getNodesToParametersMapping();
}

TreeRectangulationZero::Node* TreeRectangulationZero::buildRandomTree(int maximum_height) const
{
  Random& random = Random::getInstance();
  std::queue<Node*> tasks;

  Node* root = new Node;

  tasks.push(root);
  while( !tasks.empty() )
    {
      Node* node = tasks.front();
      tasks.pop();

      if( node->height_ <= maximum_height )
	{
	  Node* left_child = new Node;
	  Node* right_child = new Node;

	  left_child->parent_ = right_child->parent_ = node;
	  left_child->height_ = right_child->height_ = node->height_+1;
	  node->left_child_ = left_child;
	  node->right_child_ = right_child;

	  tasks.push( left_child );
	  tasks.push( right_child );
	}
    }

  return root;
}

void TreeRectangulationZero::getSplitMatrixHelper(cv::Mat split_matrix, const Node* node, std::pair<int, int> pivot, std::pair<int, int> dimentions, int* region_id) const
{
  const int region_area = dimentions.first*dimentions.second;
  if( !node->isLeaf() && region_area != 0 )
    {
      std::pair<int, int> pivot_left, pivot_right;
      std::pair<int, int> dimentions_left, dimentions_right;
      int cut_line;
      
      pivot_left = pivot;
      if( node->orientation_ == Node::Orientation::Horizontal )
	{
	  cut_line = std::round(node->split_line_*dimentions.first);
	  
	  pivot_right.first = pivot.first + cut_line;
	  pivot_right.second = pivot.second;

	  dimentions_left.first = cut_line;
	  dimentions_left.second = dimentions.second;
	  
	  dimentions_right.first = dimentions.first - cut_line;
	  dimentions_right.second = dimentions.second;
	}
      else
	{
	  cut_line = std::round(node->split_line_*dimentions.second);

	  pivot_right.first = pivot.first;
	  pivot_right.second = pivot.second + cut_line;

	  dimentions_left.first = dimentions.first;
	  dimentions_left.second = cut_line;

	  dimentions_right.first = dimentions.first;
	  dimentions_right.second = dimentions.second - cut_line;
	}

      getSplitMatrixHelper(split_matrix, node->left_child_, pivot_left, dimentions_left, region_id);
      getSplitMatrixHelper(split_matrix, node->right_child_, pivot_right, dimentions_right, region_id);
    }
  else if( node->isLeaf() && region_area != 0 )
    {

      //std::cerr << "[" << node << "] (" << pivot.first << ", " << pivot.second << ") " << "[" << dimentions.first << ", " << dimentions.second << "]"
      //<< " Area: " << dimentions.first*dimentions.second << std::endl;
      
      for(int row_offset = 0; row_offset < dimentions.first; row_offset++)
	{
	  for(int column_offset = 0; column_offset < dimentions.second; column_offset++)
	    {
	      const int row = pivot.first + row_offset;
	      const int column = pivot.second + column_offset;

	      // TODO Remove invariants check
	      assert(0 <= row && row < split_matrix.rows);
	      assert(0 <= column && column < split_matrix.cols);
	      split_matrix.at<int>(row, column) = *region_id;
	    }
	}
      *region_id += 1;
    }
}

std::vector<TreeRectangulationZero::Node*> TreeRectangulationZero::getNodesToParametersMapping() const
{
  std::vector<Node*> mapping;
  
  std::queue<Node*> tasks;
  tasks.push(root_);

  while( !tasks.empty() )
    {
      Node* node = tasks.front();
      tasks.pop();

      if( !node->isLeaf() )
	{
	  mapping.push_back(node);
	  tasks.push(node->left_child_);
	  tasks.push(node->right_child_);
	}
    }

  return mapping;
}

int TreeRectangulationZero::doGetNumberOfParameters() const
{
  return node_to_parameter_mapping_.size();
}

double TreeRectangulationZero::doGetParameter(int index) const
{
  const Node* node = node_to_parameter_mapping_[index];
  return node->split_line_;
}

void TreeRectangulationZero::doSetParameter(int index, double value)
{
  assert(0.0 <= value && value <= 1.0);
  Node* node = node_to_parameter_mapping_[index];
  node->split_line_ = value;
}

cv::Mat TreeRectangulationZero::doRandomSplitMatrix(const cv::Mat original_image) const
{
  Random& random = Random::getInstance();
  
  const int tree_height = random.nextInt(1, 8); // TODO Magic numbers
  TreeRectangulationZero rectangulation(tree_height);

  cv::Mat split_matrix = rectangulation.getSplitMatrix();
  return split_matrix;
}

cv::Mat TreeRectangulationZero::doSplitMatrix() const
{
  cv::Mat split_matrix = cv::Mat::zeros(rows_, columns_, CV_32S);
  int region_id = 1;
  
  getSplitMatrixHelper(split_matrix, root_, {0, 0}, {rows_, columns_}, &region_id);

  // TODO Remove invariants check
  for(int row = 0; row < rows_; row++)
    for(int column = 0; column < columns_; column++)
      assert( split_matrix.at<int>(row, column) != 0 );

  return split_matrix; 
}

void TreeRectangulationZero::dump(std::ostringstream& oss, const Node* n, std::string prefix, bool isTail) const
{
  oss << prefix;
  if( isTail ) oss << "└── ";
  else oss << "├── ";

  oss << "[" << n << "]  ";

  if( !n->isLeaf() )
    {
      oss << *n << std::endl;
      dump(oss, n->left_child_, prefix + (isTail ? "    " : "│   "), false);
      dump(oss, n->right_child_, prefix + (isTail ? "    " : "│   "), true);      
    }
  else
    {
      oss << std::endl;
    }
}

std::ostream& operator<<(std::ostream& os, const TreeRectangulationZero::Node& n)
{
  //os << "[" << &n << "] ";
  os << " Split Line: " << std::fixed << std::setprecision(7) << n.split_line_ << " ";
  os << " Orientation: ";
  if( n.orientation_ == TreeRectangulationZero::Node::Orientation::Horizontal )
    os << "Horizontal";
  else
    os << "Vertical";
  return os;
}

std::ostream& operator<<(std::ostream& os, const TreeRectangulationZero& t)
{
  std::ostringstream oss;
  t.dump(oss, t.root_);
  os << oss.str();
  return os;
}
  




