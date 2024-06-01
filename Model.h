#ifndef BUTTERFLY_MODEL_H
#define BUTTERFLY_MODEL_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <cmath>
#include <cassert>
#include <algorithm>

using namespace std;

struct Edge {
    int u, v;
    double weight;
    double pr;
};

struct Butterfly {
    int u1, u2, v1, v2;
    double weight;
    double pr_u1_v1, pr_u1_v2, pr_u2_v1, pr_u2_v2;
    double pr;
    int combined_weight;

    bool operator==(const Butterfly &b) const {
        return  //weight == b.weight &&
                u1 == b.u1 && u2 == b.u2 &&
                v1 == b.v1 && v2 == b.v2;
    }

    struct HashFunction {
        size_t operator()(const Butterfly &b) const {
//            size_t h1 = hash<double>()(b.weight);
            size_t h2 = hash<int>()(b.u1) << 1;
            size_t h3 = hash<int>()(b.u2) << 3;
            size_t h4 = hash<int>()(b.v1) << 5;
            size_t h5 = hash<int>()(b.v2) << 9;
            return h2 ^ h3 ^ h4 ^ h5;
        }
    };
};

struct Angle {
    int u, v, w;
    double weight;
    double pr_u_v, pr_v_w;
    double pr;
    int combined_weight;
};

struct hash_pair {
    template<class T1, class T2>
    size_t operator()(const pair<T1, T2> &p) const {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

class Model {
public:
    int num_l, num_r;
    int num_e;
    vector<int> node_l, node_r, node;
    vector<vector<Edge>> neighbor;
    vector<vector<Edge>> sampling_neighbor;
    static unordered_map<int, int> is_left;

    vector<int> priority;

    unordered_map<Butterfly, double, Butterfly::HashFunction> result;

    void PrintResult(double coe = 1.0);

    int monte_carlo_iteration = 20000;
    int DEBUG = 0;

    ifstream fin;
    ofstream fout;

    static Angle EdgeToAngle(Edge e1, Edge e2);

    static Butterfly AngleToButterfly(const Angle &a1, const Angle &a2);
};


#endif //BUTTERFLY_MODEL_H
