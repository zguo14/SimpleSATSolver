#include "tseitinTransformer.h"

TseitinTransformer::TseitinTransformer(TreeNode *root): formulaRoot{root} {}

int TseitinTransformer::transSubformula(TreeNode* subRoot) {
  // your code starts here
  // if subRoot is an operator node
  if (subRoot->getType() == "OPERATOR") {
    // create new variable(string) for clause, map variable(string) to number(int)
    int curID = varIdCounter;
    varIdCounter++;
    std::string opname = "op" + std::to_string(curID);
    varIdTable[opname] = curID;

    if (subRoot->getContent() == "*") {
      int leftID = transSubformula(subRoot->getLeftChild());
      int rightID = transSubformula(subRoot->getRightChild());
      addAndEq(curID, leftID, rightID);
    }

    if (subRoot->getContent() == "+") {
      int leftID = transSubformula(subRoot->getLeftChild());
      int rightID = transSubformula(subRoot->getRightChild());
      addOrEq(curID, leftID, rightID);
    }

    if (subRoot->getContent() == "-") {
      int childID = transSubformula(subRoot->getLeftChild());
      addNegEq(curID, childID);
    }

    return curID;
  }

  // if subRoot is an variable node 
  if (subRoot->getType() == "VARIABLE") {
    // check if variable exists in map
    if (!varIdTable.count(subRoot->getContent())) {
        int curID = varIdCounter;
        varIdCounter++;
        varIdTable[subRoot->getContent()] = curID;
        return curID;
    } else {
      return varIdTable.at(subRoot->getContent());
    }
  }

  return 0;
}

void TseitinTransformer::addNegEq(int curID, int childID) {
  // your code starts here
  std::vector<int> clause;
  // curID <-> ~childID
  std::vector<int> clause1;
  std::vector<int> clause2;

  clause1.push_back(curID * (-1));
  clause1.push_back(childID * (-1));

  clause2.push_back(childID);
  clause2.push_back(curID); 

  // push vector into cnf
  cnf.push_back(clause1);
  cnf.push_back(clause2);
}

void TseitinTransformer::addOrEq(int curID, int leftID, int rightID) {
  // your code starts here
  // curID <-> leftID v rightID
  std::vector<int> clause1;
  std::vector<int> clause2;
  std::vector<int> clause3;

  clause1.push_back(curID * (-1));
  clause1.push_back(leftID);
  clause1.push_back(rightID);

  clause2.push_back(curID);
  clause2.push_back(leftID * (-1));

  clause3.push_back(curID);
  clause3.push_back(rightID * (-1));

  // push vector into cnf
  cnf.push_back(clause1);
  cnf.push_back(clause2);
  cnf.push_back(clause3);
}

void TseitinTransformer::addAndEq(int curID, int leftID, int rightID) {
  // your code starts here
  // curID <-> leftID ^ rightID
  std::vector<int> clause1;
  std::vector<int> clause2;
  std::vector<int> clause3;

  clause1.push_back(curID * (-1));
  clause1.push_back(leftID);

  clause2.push_back(curID * (-1));
  clause2.push_back(rightID);

  clause3.push_back(curID);
  clause3.push_back(leftID * (-1));
  clause3.push_back(rightID * (-1));

  // push vector into cnf
  cnf.push_back(clause1);
  cnf.push_back(clause2);
  cnf.push_back(clause3);
}

std::vector<std::vector<int>> TseitinTransformer::transform() {
  // your code starts here
  int rootNum = transSubformula(formulaRoot);
  std::vector<int> rootVec;
  rootVec.push_back(rootNum);
  cnf.push_back(rootVec);
  return cnf;
}

std::string TseitinTransformer::cnfString() const {
  std::string result = "";
  // your code starts here
  for (int i = 0; i < cnf.size(); i++) {
    result += "[";
    for (int j = 0; j < cnf.at(i).size(); j++) {
      result += std::to_string(cnf.at(i).at(j));
      if (j != cnf.at(i).size() - 1) {
        result += ",";
      }
    }
    result += "]";
  }
  return result;
}

unsigned int TseitinTransformer::getVarNum() const {
  // your code starts here
  return varIdCounter - 1;
}
