#include "MonteCarloSampling.h"
#include "BruteForce.h"

void MonteCarloSampling(Model &model) {
    ComputePriority(model);

    double ans = 0;
    for (int i = 0; i < model.monte_carlo_iteration; i++) {
        if (i % 1 == 0) cout << "iteration: " << i << "/" << model.monte_carlo_iteration << endl;
        model.sampling_neighbor.clear();
        model.sampling_neighbor.resize(model.neighbor.size(), vector<Edge>{});
        for (auto &u: model.node_l) {
            for (auto edge: model.neighbor[u]) {
                if (rand() % 100 <= edge.pr * 100) {
                    model.sampling_neighbor[edge.u].push_back(Edge{edge.u, edge.v, edge.weight, edge.pr});
                    model.sampling_neighbor[edge.v].push_back(Edge{edge.v, edge.u, edge.weight, edge.pr});
                }
            }
        }
        MaximumButterfly(model, 1);
    }

    cout << "total " << ans << "**" << endl;
    model.fout << "total " << ans << "*" << endl;

    model.PrintResult(model.monte_carlo_iteration);
}