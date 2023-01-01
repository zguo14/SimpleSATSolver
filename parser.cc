#include "parser.h"
#include "treeNode.h"
#include "tokenizer.h"

FormulaParser::FormulaParser(std::string ln): tknzr {new Tokenizer{ln}} {}

TreeNode *FormulaParser::parseConjTerm() {
  // your code starts here
  TreeNode *node1 = parseTerm();
  
  tknzr->advanceToken();

  Token t = tknzr->getToken();
  if (t.content == "*") {
    TreeNode *root = new OperatorNode("*");
    tknzr->advanceToken();
    TreeNode *node2 = parseConjTerm();
    root->updateChildren(node1, node2);
    return root;
  } else {
    return node1;
  }

  return nullptr;
}

TreeNode *FormulaParser::parseTerm() {
  Token t = tknzr->getToken();

  if (t.type == "CONSTANT") {
    TreeNode *node = new ConstantNode(t.content);
    return node;
  } else if (t.type == "VARIABLE") {
    TreeNode *node = new VariableNode(t.content);
    return node;
  } else if (t.content == "-") {
    TreeNode *root = new OperatorNode("-");
    tknzr->advanceToken();
    TreeNode *node = parseTerm();
    root->updateLeftChild(node); // update node
    return root;
  } else if (t.content == "(") {
    tknzr->advanceToken();
    TreeNode *node = parseFormula();
    return node;
  } else {
    throw "Error: invalid input"; // *; +; **; ***;
  }
  
  return nullptr;
}

TreeNode *FormulaParser::parseFormula() {
  TreeNode *node1 = parseConjTerm();
  Token t = tknzr->getToken();
  if (t.content == "+") {
    TreeNode *root = new OperatorNode("+");
    tknzr->advanceToken();
    TreeNode *node2 = parseFormula();
    root->updateChildren(node1, node2);
    return root;
  } else {
    return node1;
  }
  return nullptr;
}

TreeNode * FormulaParser::getTreeRoot() {
  // your code starts here
  TreeNode *node = parseFormula();
  // a b + ; | a b ; ab:1
  if (tknzr->hasToken()) {
    throw "Error: invalid input";
  } 
  
  tknzr->deleteTokens();
  return node;
}

FormulaParser::~FormulaParser() {
  // your code starts here
  tknzr->deleteTokens();
  tknzr = nullptr;
}

AssignmentParser::AssignmentParser(std::string ln): tknzr {new Tokenizer{ln}} {}

std::map<std::string, bool> AssignmentParser::parseAssignment() {
  std::map<std::string, bool> results;
  // your code starts here
  std::string key = "";
  std::string value = "";
  Token prevToken;
  prevToken.content = "";
  prevToken.type = "";

  while (tknzr->hasToken()) {
    
    Token t = tknzr->getToken();
    
    if (t.type == "VARIABLE") {
      if (prevToken.type ==  "VARIABLE") {
        throw "Error: invalid input"; //a ; ab ab
      }
      key = t.content;
      prevToken.content = key;
      prevToken.type = "VARIABLE";
    }

    if (t.type == "COLON") {
      prevToken.content = ":";
    }
  
    if (t.type == "CONSTANT") {
      if (prevToken.type ==  "CONSTANT") {
        throw "Error: invalid input"; //a b ; ab:1
      }
      
      if (prevToken.content != "" && prevToken.content != ":") {
        throw "Error: invalid input";//1 * VAr + 0 ; VAr 1
      }
      value = t.content;
      prevToken.content = value;
      prevToken.type = "CONSTANT";
    }

    if (key != "" && value != "") {
      if (results.count(key)) {
        std::string tmp;
        if (results.at(key)) {
          tmp = "1";
        } else {
          tmp = "0";
        }
        if (tmp != value) {
          throw "Error: contradicting assignment";
        }
      }

      if (value == "0") {
        results[key] = false;
      }

      if (value == "1") {
        results[key] = true;
      }
      
      key = "";
      value = "";
    }
    tknzr->advanceToken();
  }
  return results;
}

AssignmentParser::~AssignmentParser() {
  // your code starts here
  tknzr = nullptr;
}
