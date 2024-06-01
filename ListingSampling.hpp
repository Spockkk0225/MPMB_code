#ifndef BUTTERFLY_LISTINGSAMPLING_HPP
#define BUTTERFLY_LISTINGSAMPLING_HPP

#include "Model.h"
#include "BruteForce.h"
#include <algorithm>

void ListingSampling(Model &model);

bool EdgeExist(int u, int v, double pr, unordered_map<int, unordered_map<int, int>> &edge_exist);

void ProbabilityEstimation(Model &model, vector<Butterfly> &candidate_butterfly_set);

bool Check(int &u, int &v, Butterfly &b);

double Except(Butterfly &b1, Butterfly &b2);

void Add(Butterfly &b1, Butterfly &b2, unordered_map<int, unordered_map<int, int>> &edge_exist);

void ProbabilityEstimationKarpLuby(Model &model, vector<Butterfly> &candidate_butterfly_set);

#endif //BUTTERFLY_LISTINGSAMPLING_HPP
