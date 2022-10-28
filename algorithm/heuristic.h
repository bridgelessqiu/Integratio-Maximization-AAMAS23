#ifndef HEURISTIC_H_06191147
#define HEURISTIC_H_06191147 

#include<string>
#include<unordered_map>
#include<vector>
#include "graph.h"

// local_improvement - 1/2 approximation
int local_improvement(const std::vector<std::vector<int>>&, int k);

// Greedy
int greedy(const std::vector<std::vector<int>>&, int k);

// Random
int random_assignment(const std::vector<std::vector<int>>&, int k);

int myrandom (int i);

#endif
