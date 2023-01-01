#ifndef SAT_H
#define SAT_H

#include <vector>
#include <map>

bool DPLL(std::vector<std::vector<int>> cnf, std::map<int, int> assignment); // return the SAT result of cnf by calling MiniSAT

#endif
