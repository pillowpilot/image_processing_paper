#include <tree_rectangulation.h>

void Node::fixInvariants(){
  const Node* parent = parent_;

  double proportion;
  if( orientation_ == Orientation::Horizontal )
    proportion = (double)split_line_/region_height_;
  else
    proportion = (double)split_line_/region_width_;

  if( !isLeaf() ) assert(0 <= proportion && proportion <= 1);
  const bool is_left_child = parent->left_child_ == this;
  if( is_left_child ){    
    // Fix pivot point
    pivot_row_ = parent->pivot_row_;
    pivot_column_ = parent->pivot_column_;

    // Fix region dim
    if( parent->orientation_ == Orientation::Horizontal )
      {
	region_width_ = parent->region_width_;
	region_height_ = parent->region_height_ + parent->split_line_;
      }
    else
      {
	region_width_ = parent->region_width_ + parent->split_line_;
	region_height_ = parent->region_height_;
      }
  }else{
    // Pivot point
    if( parent->orientation_ == Orientation::Horizontal )
      {
	pivot_row_ = parent->pivot_row_ + parent->split_line_;
	pivot_column_ = parent->pivot_column_;
      }
    else
      {
	pivot_row_ = parent->pivot_row_;
	pivot_column_ = parent->pivot_column_ + parent->split_line_;
      }
    
    // Region Dims
    if( parent->orientation_ == Orientation::Horizontal )
      {
	region_width_ = parent->region_width_;
	region_height_ = parent->region_height_-parent->split_line_;
      }
    else
      {
	region_width_ = parent->region_width_-parent->split_line_;
	region_height_ = parent->region_height_;
      }
  }

  if( orientation_ == Orientation::Horizontal )
    split_line_ = (int)(proportion*region_height_);
  else
    split_line_ = (int)(proportion*region_width_);

  if( !isLeaf() ) assert(0 <= split_line_ && split_line_ <= getSplitLineMaximumValue());
  if( !isLeaf() ){
    left_child_->fixInvariants();
    right_child_->fixInvariants();
  }
}

void Node::doSetParameter(int index, double value)
{
  assert(0 <= value && value <= 1);
  const int new_split_line = (int)std::round(value*getSplitLineMaximumValue());
  assert(0 <= new_split_line && new_split_line <= getSplitLineMaximumValue());
  split_line_ = new_split_line;
  left_child_->fixInvariants();
  right_child_->fixInvariants();
}

TreeRectangulation::TreeRectangulation(int rows, int columns, double alpha):
  split_matrix_rows_(rows), split_matrix_columns_(columns), minimum_area_(alpha*rows*columns), root_(nullptr)
{
  root_ = buildRandomTree(rows, columns, alpha);

  std::ostringstream oss;
  printTree(oss, root_);
  std::cout << oss.str() << std::endl;

  parameter_node_mapping_ = buildParameterToNodeMapping(root_);
  std::cout << root_ << std::endl;
  checkTreeInvariants();
}

TreeRectangulation::TreeRectangulation(const TreeRectangulation& other):
  root_(other.root_), split_matrix_rows_(other.split_matrix_rows_),
  split_matrix_columns_(other.split_matrix_columns_), minimum_area_(other.minimum_area_),
  parameter_node_mapping_(buildParameterToNodeMapping(root_))
{
}

cv::Mat TreeRectangulation::doSplitMatrix() const
{
  cv::Mat split_matrix(split_matrix_rows_, split_matrix_columns_, CV_32S);
  split_matrix = cv::Mat::zeros(split_matrix_rows_, split_matrix_columns_, CV_32S);
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
	      assert(split_matrix.at<int>(row, column) == 0);
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

  for(int row = 0; row < split_matrix_rows_; row++)
    for(int column = 0; column < split_matrix_columns_; column++)
      assert(split_matrix.at<int>(row, column) != 0);

  root_->printToCout();
      
  return split_matrix;
}

Node* TreeRectangulation::buildRandomTree(int rows, int columns, double alpha) const
{
  const int minimum_area = alpha*rows*columns;
  std::cout << minimum_area << std::endl;
  
  Random& random = Random::getInstance();
  std::queue<Node*> tasks;

  Node* root = new Node;
  root->pivot_row_ = root->pivot_column_ = 0;
  root->region_height_ = rows;
  root->region_width_ = columns;
  root->height_ = 0;

  tasks.push(root);

  while( !tasks.empty() ){
    Node* node = tasks.front();
    tasks.pop();

    if( minimum_area <= node->getRegionArea() && 1 < node->getRegionArea() )
      {
	if( node->region_width_ == 1 )
	  node->orientation_ = Node::Orientation::Horizontal;
	else if( node->region_height_ == 1 )
	  node->orientation_ = Node::Orientation::Vertical;
	else
	  {
	    if( random.nextInt(0, 2) == 0 )
	      node->orientation_ = Node::Orientation::Horizontal;
	    else
	      node->orientation_ = Node::Orientation::Vertical;
	  }
	  
	const int split_line = random.nextInt(1, node->getSplitLineMaximumValue()+1); // From [1, maxV]
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

  return root;
}

std::vector<Node*> TreeRectangulation::buildParameterToNodeMapping(Node* root) const
{
  std::vector<Node*> parameter_node_mapping;
  
  std::queue<Node*> nodes;
  nodes.push(root);
  while( !nodes.empty() )
    {
      Node* node = nodes.front();
      nodes.pop();

      if( node->getNumberOfParameters() == 1 )
	parameter_node_mapping.push_back(node);

      if( node->left_child_ != nullptr ) nodes.push(node->left_child_);
      if( node->right_child_ != nullptr ) nodes.push(node->right_child_);
    }

  return parameter_node_mapping;
}

cv::Mat TreeRectangulation::doRandomSplitMatrix(int rows, int columns) const
{
  Random& random = Random::getInstance();
  const double alpha = random.nextDouble();
  TreeRectangulation rectangulation(rows, columns, alpha);

  return rectangulation.getSplitMatrix();
}

int TreeRectangulation::doGetNumberOfParameters() const
{
  return parameter_node_mapping_.size();
}

double TreeRectangulation::doGetParameter(int index) const
{
  assert(0 <= index);
  assert(index < parameter_node_mapping_.size());
  
  Node* node = parameter_node_mapping_[index];
  return node->getParameter(0);
}

void TreeRectangulation::doSetParameter(int index, double value)
{
  assert(0 <= index);
  assert(index < parameter_node_mapping_.size());
  
  assert( 0 <= value );
  assert( value <= 1 );

  Node* node = parameter_node_mapping_[index];
  node->setParameter(0, value);
}

void TreeRectangulation::checkTreeInvariants() const
{
  using namespace std;

  const int rows = split_matrix_rows_;
  const int cols = split_matrix_columns_;
  
  queue<Node*> q;
  q.push(root_);
  while( !q.empty() ){
    Node* n = q.front(); q.pop();

    assert( 0 <= n->pivot_row_ && n->pivot_row_ < rows );
    assert( 0 <= n->pivot_column_ && n->pivot_column_ < cols );
    
    assert( 0 <= n->pivot_row_ + n->region_height_ && n->pivot_row_ + n->region_height_ <= rows );
    assert( 0 <= n->pivot_column_ + n->region_width_ && n->pivot_column_ + n->region_width_ <= cols );

    if( !n->isLeaf() ){
      assert( 0 <= n->split_line_ && n->split_line_ <= n->getSplitLineMaximumValue() );
    }

    if( !n->isLeaf() ){
      q.push(n->left_child_);
      q.push(n->right_child_);
    }
  }
}

void printTree(std::ostringstream& oss, const Node* n, std::string prefix, bool isTail)
{
  using namespace std;
  
  oss << prefix;
  if( isTail ) oss << "└── ";
  else oss << "├── ";

  oss << "[" << n << "]  ";
  oss << "pivot=" << "(" << n->pivot_row_ << ", " << n->pivot_column_ << ")  ";
  oss << "(width, height)=" << "(" << n->region_width_ << ", " << n->region_height_ << ")  ";

  if( !n->isLeaf() )
    {
      oss << "split line=" << n->split_line_ << " in [0, " << n->getSplitLineMaximumValue() <<"]  ";
      oss << "orientation=";
      if( n->orientation_ == Node::Orientation::Horizontal )
	oss << "Horizontal";
      else
	oss << "Vertical";
  
      oss << std::endl;

      printTree(oss, n->left_child_, prefix + (isTail ? "    " : "│   "), false);
      printTree(oss, n->right_child_, prefix + (isTail ? "    " : "│   "), true);      
    }
  else
    {
      oss << std::endl;
    }
}

void Node::printToCout() const
{
  std::cout << this << std::endl;
}
