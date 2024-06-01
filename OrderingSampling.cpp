#include "OrderingSampling.h"
#include <algorithm>

bool cmp(const Edge &e1, const Edge &e2) { return e1.weight > e2.weight; }

void OrderingSampling(Model &model) {
    vector<Edge> ordering_edge;
    for (auto &u: model.node_l) {
        for (auto &edge: model.neighbor[u]) {
            ordering_edge.push_back(edge);
        }
    }
    sort(ordering_edge.begin(), ordering_edge.end(), cmp);

    cout << "Start...\n";
    clock_t start = clock();
    for (int i = 0; i < model.monte_carlo_iteration; i++) {
        OrderingMaximumButterfly(model, ordering_edge);
    }
    clock_t end = clock();
    cout << "sampling Time : " << (double) (end - start) / CLOCKS_PER_SEC << " s " << endl;

    model.PrintResult(model.monte_carlo_iteration);
}

void OrderingMaximumButterfly(Model &model, vector<Edge> &ordering_edge) {
    double max3 = ordering_edge[0].weight + ordering_edge[1].weight + ordering_edge[2].weight;
    double max2 = ordering_edge[0].weight + ordering_edge[1].weight;

    double max_butterfly_weight = 0;
    vector<Butterfly> max_butterfly_set;
    model.sampling_neighbor.clear();
    model.sampling_neighbor.resize(model.neighbor.size(), vector<Edge>{});
    unordered_map<pair<int, int>, vector<Angle>, hash_pair> angle1, angle2;

    for (auto &edge1: ordering_edge) {
        if (edge1.weight + max3 < max_butterfly_weight) break;
        if (!(rand() % 100 <= edge1.pr * 100)) continue;

        for (auto &edge2: model.sampling_neighbor[edge1.v]) {
            if (edge1.weight + edge2.weight + max2 < max_butterfly_weight) break;
            Angle angle = Model::EdgeToAngle(edge1, edge2);

            pair<int, int> endpoints = make_pair(min(edge1.u, edge2.u), max(edge1.u, edge2.u));

            if (angle1[endpoints].empty() || angle.weight > angle1[endpoints][0].weight) {
                angle2[endpoints] = angle1[endpoints];
                angle1[endpoints].clear();
                angle1[endpoints].push_back(angle);

            } else if (angle.weight == angle1[endpoints][0].weight) {
                angle1[endpoints].push_back(angle);

            } else if (angle2[endpoints].empty() || angle.weight > angle2[endpoints][0].weight) {
                angle2[endpoints].clear();
                angle2[endpoints].push_back(angle);

            } else if (angle.weight == angle2[endpoints][0].weight) {
                angle2[endpoints].push_back(angle);
            }

            if (angle1[endpoints].size() > 1) {
                max_butterfly_weight = max(angle1[endpoints][0].weight * 2, max_butterfly_weight);
            }
            if (!angle2[endpoints].empty()) {
                max_butterfly_weight = max(angle1[endpoints][0].weight + angle2[endpoints][0].weight,
                                           max_butterfly_weight);
            }
        }

        model.sampling_neighbor[edge1.v].push_back(edge1);
    }

    for (auto &kv: angle1) {
        auto endpoints = kv.first;
        if (angle1[endpoints].size() > 1 && angle1[endpoints][0].weight * 2 == max_butterfly_weight) {
            for (int i = 0; i < angle1[endpoints].size(); i++) {
                for (int j = i + 1; j < angle1[endpoints].size(); j++) {
                    Butterfly b = Model::AngleToButterfly(angle1[endpoints][i], angle1[endpoints][j]);
                    max_butterfly_set.push_back(b);
                }
            }
        }

        if (!angle2[endpoints].empty() &&
            angle1[endpoints][0].weight + angle2[endpoints][0].weight == max_butterfly_weight) {
            for (int i = 0; i < angle2[endpoints].size(); i++) {
                Butterfly b = Model::AngleToButterfly(angle1[endpoints][0], angle2[endpoints][i]);
                max_butterfly_set.push_back(b);
            }
        }
    }

    for (auto &butterfly: max_butterfly_set) {
        model.result[butterfly] += 1;
    }

}