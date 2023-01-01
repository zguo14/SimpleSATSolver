#include "satSolver.h"
#include <stdlib.h>
#include <memory>
#include <vector>
#include <set>
#include <map>

bool isPureLiteral(const std::vector<std::vector<int>> &cnf, int lit) {
	bool positive = false;
	bool negative = false;
    for (int i = 0; i < cnf.size(); i++) {
        for (int j = 0; j < cnf.at(i).size(); j++) {
            int cur = cnf.at(i).at(j);
			if (lit == cur) {
				positive = true;
				if (negative) {
					return false;
				}
			} 
			if (lit == -cur) {
				negative = true;
				if (positive) {
					return false;
				}
			}
        }
    }
	return false;
}

bool hasEmptyClause(const std::vector<std::vector<int>> &cnf) {
    for (int i = 0; i < cnf.size(); i++) {
        if (cnf.at(i).empty()) {
            return true;
        } 
    }
    return false;
}

std::vector<std::vector<int>> propagate(std::vector<std::vector<int>> cnf, int lit) {
    std::vector<int> clausesToDrop;

    for (int i = 0; i < cnf.size(); i++) {

        std::vector<int> litToRemove;

        for (int j = 0; j < cnf.at(i).size(); j++) {
            if (abs(lit) == abs(cnf.at(i).at(j))) {
				// should remove that literal in current clause
				if (cnf.at(i).at(j) * lit < 0) {
                    litToRemove.push_back(j);
                    break;
                } 
				// should drop current clause from CNF 
                if (cnf.at(i).at(j) * lit > 0) {
                    clausesToDrop.push_back(i);
                    break;
                } 
            }
        }

		// delete literals in current clause
        for (int k = 0; k < litToRemove.size(); k++) {
            cnf[i].erase(cnf.at(i - k).begin() + litToRemove[k]);
        }
    }

	// drop clauses
    for (int i = 0; i < clausesToDrop.size(); i++) {
        cnf.erase(cnf.begin() + clausesToDrop.at(i) - i);
    }

    return cnf;
}

bool BCP(std::vector<std::vector<int>> &cnf, std::map<int, int> &assignment) {
    bool foundUnitClause = false;

    while (true) {
        int unitClause = 0;

		for (int i = 0; i < cnf.size(); i++) {
			if (cnf.at(i).size() == 1) {
				unitClause = cnf.at(i).at(0);
			}
		}

        if (unitClause == 0) {
			return foundUnitClause;
		} else {
			foundUnitClause = true;
		}

        if (unitClause > 0) {
			assignment[unitClause] = 1;
		} 
		
		if (unitClause < 0){
			assignment[-unitClause] = -1;
		}

        cnf = propagate(cnf, unitClause);
    }

}

bool PLP(std::vector<std::vector<int>> &cnf, std::map<int, int> &assignment) {  
	bool foundPureLiteral = false;

    std::vector<int> pureLiterals;
	std::map<int, int>::iterator it;
    std::map<int, int>::iterator itEnd;
	it = assignment.begin();
	itEnd = assignment.end();

	while (it != itEnd) {
		// Only choose unassigned vars
		if (it->second == 0) {
			if (isPureLiteral(cnf, it->first)) {
				foundPureLiteral = true;
				pureLiterals.push_back(it->first);
				cnf = propagate(cnf, it->first);
			}
		}
		it++;
	}

	// Update assignment map
    for(int i = 0; i < pureLiterals.size(); i++) {
		int cur = pureLiterals.at(i);
        if (cur > 0) {
			assignment[cur] = 1;
		} 
		if (cur < 0) {
			assignment[-cur] = -1;
		}
    }

    return foundPureLiteral;
}

int chooseVar(const std::vector<std::vector<int>> &cnf, std::map<int, int> assignment) {
    int var = 0;
    int max = 0;
	std::map<int, int>::iterator it;
    std::map<int, int>::iterator itEnd;
	it = assignment.begin();
	itEnd = assignment.end();

	while (it != itEnd) {
		// Only choose unassigned vars
		if (it->second == 0) { 
			int count = 0;
			for (int i = 0; i < cnf.size(); i++) {
				for (int j = 0; j < cnf.at(i).size(); j++) {
					if(abs(cnf.at(i).at(j)) == it->first) {
						count++;
					}
				}
			}
			// Choose variables that appears most often in CNF
			if(count > max) {
				max = count;
				var = it->first;
			}
		}
		it++;
	}
    return var;
}



bool DPLL(std::vector<std::vector<int>> cnf, std::map<int, int> assignment) {
    while (true) {
        if(cnf.empty()) {
            return true;
        }

        if(hasEmptyClause(cnf)) {
            return false;
        }

		bool ifBCP = BCP(cnf, assignment);

		bool ifPLP = PLP(cnf, assignment);

		if (!(ifBCP || ifPLP)) {
			break;
		}
    }

    int lit = chooseVar(cnf, assignment);
	
	assignment[abs(lit)] = -2;// just remove it from unassigned status

	std::vector<std::vector<int>> newCNF;
	std::map<int, int> newAssignment = assignment;

	newCNF = propagate(cnf, lit);
	newAssignment[lit] = 1;
	if (DPLL(newCNF, newAssignment)) {
		return true;
	}

	newCNF = propagate(cnf, -lit);
	newAssignment[lit] = -1;
	if (DPLL(newCNF, newAssignment)) {
		return true;
	}

    return false;
}