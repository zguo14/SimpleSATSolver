#include "treeNode.h"

TreeNode::TreeNode(std::string cntt): content {cntt} {}

std::string TreeNode::getContent() const {    
  // your code starts here
  return content;
}

TreeNode *TreeNode::getLeftChild() const {
  // your code starts here
  return leftChild;
}
TreeNode *TreeNode::getRightChild() const {
  // your code starts here
  return rightChild;
}

void TreeNode::updateLeftChild(TreeNode *lChild) {
  // your code starts here
  leftChild = lChild;
}

void TreeNode::updateChildren(TreeNode *lChild, TreeNode *rChild) {
  // your code starts here
  leftChild = lChild;
  rightChild = rChild;
}

OperatorNode::OperatorNode(std::string cntt): TreeNode{cntt} {}

bool OperatorNode::evaluate(const std::map<std::string, bool> &assignment) const {
  // your code starts here
  bool leftVal;
  bool rightVal;

  if (getLeftChild()) {
    leftVal = getLeftChild()->evaluate(assignment);
  }
  
  if (getRightChild()) {
    rightVal = getRightChild()->evaluate(assignment);
  }
  
  if (getContent() == "+") {
    return leftVal || rightVal;
  }

  if (getContent() == "*") {
    return leftVal && rightVal;
  }

  if (getContent() == "-") {
    return !leftVal;
  }

  return false;
}

std::string OperatorNode::getType() const {
return "OPERATOR";
}

ConstantNode::ConstantNode(std::string cntt): TreeNode{cntt} {}

bool ConstantNode::evaluate(const std::map<std::string, bool> &assignment) const {
  // your code starts here
  if (getContent() == "1") {
    return true;
  }

  if (getContent() == "0") {
    return false;
  }

  return false;
}

std::string ConstantNode::getType() const {
  return "CONSTANT";
}

VariableNode::VariableNode(std::string cntt): TreeNode{cntt} {}

bool VariableNode::evaluate(const std::map<std::string, bool> &assignment) const {
  // your code starts here
  if (assignment.count(getContent()) == 0) {
    throw "Error: incomplete assignment";
  }

  bool val = assignment.at(getContent());
  
  return val;
}

std::string VariableNode::getType() const {
  return "VARIABLE";
}

TreeNode::~TreeNode() {
  // your code starts here
  leftChild = nullptr;
  rightChild = nullptr;
}
