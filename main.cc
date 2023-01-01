#include "tseitinTransformer.h"
#include "satSolver.h"
#include "parser.h"
#include "treeNode.h"
#include <queue>
#include <set>
#include <vector>
#include <iostream>

// The program shall continuously ask for new inputs from standard input and output to the standard output
// The program should terminate gracefully (and quietly) once it sees EOF
int main() {
	while (true) // continuously asking for new inputs from standard input
	{
		std::string line; // store each input line
		// your code starts here
		getline(std::cin, line);

		if (line.length() == 0) {
			std::cout << "Error: invalid input"<< std::endl;
			continue;
		}

        bool has_invalid_char;
		for (char c : line) {
			if (isdigit(c) || isalpha(c) ||
            c == '(' || c == ')' || c == '+'|| c == '-'|| c == '*'|| c == ' ') {
                has_invalid_char = false;
            } else {
                has_invalid_char = true;
                break;
            }
		}
        if (has_invalid_char) {
            std::cout << "Error: invalid input"<< std::endl;
			continue;
        }

        FormulaParser *fp;
        try {
            fp = new FormulaParser(line);
        } catch (const char* msg) {
            std::cout << msg << std::endl;
            continue;
        }

        TreeNode *root;
		try {
            root = fp->getTreeRoot();
        } catch (const char* msg) {
            std::cout << msg << std::endl;
            continue;
        }
		
		TseitinTransformer *tt = new TseitinTransformer(root);
		std::vector<std::vector<int>> vec = tt->transform();

        // In assignment, 1 = true, -1 = false, 0 = unassigned
        std::map<int, int> assignment; 
        for (int i = 1; i <= tt->getVarNum(); i++) {
            assignment.insert(std::pair<int, int>(i, 0));
        }

        if(DPLL(vec, assignment)) {
            std::cout << "sat"<< std::endl;
        } else {
            std::cout << "unsat"<< std::endl;
        };
	}
}
