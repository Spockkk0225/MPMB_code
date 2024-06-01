#include "BruteForce.h"

void BruteForce(Model &model) {
    double ans = 0;
    for (int gen = 0; gen < (1 << model.num_e); gen++) {
        int gen_seed = gen;

        double pr = 1.0;
        model.sampling_neighbor.clear();
        model.sampling_neighbor.resize(model.neighbor.size(), vector<Edge>{});
        for (auto &u: model.node_l) {
            for (auto edge: model.neighbor[u]) {
                if ((gen_seed & 1) == 0) {
                    pr *= edge.pr;
                    model.sampling_neighbor[edge.u].push_back(Edge{edge.u, edge.v, edge.weight, edge.pr});
                    model.sampling_neighbor[edge.v].push_back(Edge{edge.v, edge.u, edge.weight, edge.pr});
                } else {
                    pr *= 1 - edge.pr;
                }
                gen_seed >>= 1;
            }
        }

        ans += pr;
        MaximumButterfly(model, pr);
    }

    cout << "total " << ans << "**" << endl;
    model.fout << "total " << ans << "*" << endl;
    model.PrintResult();
}

void MaximumButterfly(Model &model, double pr) {
    vector<Butterfly> max_butterfly_set;
    double max_weight = 0;
    for (int &u: model.node) {
        unordered_map<int, vector<Angle>> angle;
        for (auto &edge1: model.sampling_neighbor[u]) {
            int v = edge1.v;
            if (model.priority[v] >= model.priority[u]) continue;
            for (auto &edge2: model.sampling_neighbor[v]) {
                int w = edge2.v;
                if (model.priority[w] >= model.priority[u]) continue;

                Angle new_angle = Model::EdgeToAngle(edge1, edge2);
                angle[w].push_back(new_angle);
            }
        }

        for (auto &[w, angles]: angle) {

            for (int i = 0; i < angles.size(); i++) {
                for (int j = i + 1; j < angles.size(); j++) {
                    Butterfly butterfly = Model::AngleToButterfly(angles[i], angles[j]);

                    if (butterfly.weight == max_weight) {
                        max_butterfly_set.push_back(butterfly);
                    } else if (butterfly.weight > max_weight) {
                        max_butterfly_set.clear();
                        max_butterfly_set.push_back(butterfly);
                        max_weight = butterfly.weight;
                    }
                }
            }
        }
    }

    for (auto &butterfly: max_butterfly_set) {
        model.result[butterfly] += pr;
    }
}


void ComputePriority(Model &model) {
    vector<pair<int, int>> value;
    for (int i = 1; i <= model.num_l + model.num_r; i++) {
        value.emplace_back(model.neighbor[i].size(), i);
    }
    sort(value.begin(), value.end());

    model.priority.resize(model.num_l + model.num_r + 1);
    for (int i = 1; i <= model.num_l + model.num_r; i++) {
        model.priority[value[i - 1].second] = i;
    }
}


void MaximumButterfly2(Model &model, double pr) {
    vector<Butterfly> max_butterfly_set;
    int max_weight = 0;
    for (int &u: model.node_l) {
        unordered_map<int, vector<Angle>> angle;
        for (auto &edge1: model.sampling_neighbor[u]) {
            int v = edge1.v;
            for (auto &edge2: model.sampling_neighbor[v]) {
                int w = edge2.v;

                Angle new_angle = Model::EdgeToAngle(edge1, edge2);
                angle[w].push_back(new_angle);
            }
        }

        for (auto &[w, angles]: angle) {

            for (int i = 0; i < angles.size(); i++) {
                for (int j = i + 1; j < angles.size(); j++) {
                    Butterfly butterfly = Model::AngleToButterfly(angles[i], angles[j]);

                    if (butterfly.weight == max_weight) {
                        max_butterfly_set.push_back(butterfly);
                    } else if (butterfly.weight > max_weight) {
                        max_butterfly_set.clear();
                        max_butterfly_set.push_back(butterfly);
                        max_weight = butterfly.weight;
                    }
                }
            }
        }
    }

    for (auto &butterfly: max_butterfly_set) {
        model.result[butterfly] += pr;
    }
}



