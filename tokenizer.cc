#include "tokenizer.h"
#include <vector>
#include <map>
#include <string>
#include <stack>


bool isNumeric(std::string const &str) {
	char* p;
	strtol(str.c_str(), &p, 10);
	return *p == 0;
}

Tokenizer::Tokenizer(std::string ln) {
  // complete the constructor
		cur = 0;
		std::stack<char> parentheses;
		int i = 0;
	while (i < ln.size()) {
		char c = ln.at(i);
		if (!(isalpha(c) || isdigit(c) || c == ' '|| c == ','|| c == ':'|| 
		c == '+'|| c == '-'|| c == '*'|| c == '(' || c == ')')){
			throw "Error: invalid input";
		}

		if (ln.at(i) == ' ') {
			i++;
			continue;
		}

		if (c == '(' || c == ')') {
			if (c == '(') {
			parentheses.push(c);
			} else {
			if (parentheses.empty()) {
				throw "Error: invalid input";
			}
			if (parentheses.top() != '(') {
				throw "Error: invalid input";
			}
			parentheses.pop();
			}
		}

		if (isalpha(ln.at(i))) {
			int j = i;
			std::string tmp = "";
			while (isalpha(ln.at(j)) || isdigit(ln.at(j))) {
			tmp += ln.at(j);
			j++;
			if (j == ln.size()) {
				break;
			}
			}
			tokens.push_back(tmp);
			i = j;
		} else if (isdigit(ln.at(i))) {
			throw "Error: invalid input";
		} else {
			std::string tmp = "";
			tmp += ln.at(i);
			tokens.push_back(tmp);
			i++;
		}
  }

  std::stack<char>().swap(parentheses);
	
  for (int i = 0; i < tokens.size(); i++) {
		std::string token = tokens.at(i);
		if (token.size() > 10) {
			throw "Error: invalid input";
		}
	// 1 + (1 + 0) + (2 + 3);
		if (isNumeric(token)) {
			int num = std::stoi(token);
			if (!(num == 1 || num == 0)) {
			throw "Error: invalid input";
			}
		}
  }
}

bool Tokenizer::advanceToken() {
  cur += 1;
  return false;
}

bool Tokenizer::hasToken() const {
  // your code starts here
  if (cur < tokens.size()) {
	return true;
  }
  return false;
}

void Tokenizer::deleteTokens() {
  std::vector <std::string>().swap(tokens);
}

// only can be called when hasToken() is true
Token Tokenizer::getToken() {
  Token resultToken;
  if (!hasToken()) {
	return resultToken;
  }
  // std::string cur_str = tokens[cur];
  std::string cur_str = tokens.at(cur);

  if (cur_str == "+" || cur_str == "-" || cur_str == "*") {
	resultToken.type = "OPERATOR";
  } else if (cur_str == "0" || cur_str == "1") {
	resultToken.type = "CONSTANT";
  } else if (cur_str == "(" || cur_str == ")") {
	resultToken.type = "PARENTHESES";
  } else if (cur_str == ":") {
	resultToken.type = "COLON"; 
  } else if (cur_str == ",") {
	resultToken.type = "COMMA";
  } else {
	resultToken.type = "VARIABLE";
  }
  
  resultToken.content = cur_str;

  return resultToken;
}
