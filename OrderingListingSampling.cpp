#include "OrderingListingSampling.hpp"

void OrderingListingSampling(Model &model) {
    clock_t start = clock();

    vector<Edge> ordering_edge;
    for (auto &u: model.node_l) {
        for (auto &edge: model.neighbor[u]) {
            ordering_edge.push_back(edge);
        }
    }
    sort(ordering_edge.begin(), ordering_edge.end(),
         [&](const Edge &e1, const Edge &e2) {
             return e1.weight > e2.weight;
         });

    model.DEBUG = 0;
    for (int i = 0; i < 200; i++) {
//        cout << i << " / " << 100 <<"\n";
        OrderingMaximumButterfly(model, ordering_edge);
        model.DEBUG++;
    }

    vector<Butterfly> candidate_butterfly_set;
    for (auto &[butterfly, times]: model.result) {
        candidate_butterfly_set.push_back(butterfly);
    }
    model.result.clear();
    sort(candidate_butterfly_set.begin(), candidate_butterfly_set.end(),
         [&](const Butterfly &b1, const Butterfly &b2) {
             return b1.weight > b2.weight;
         });
    cout << "size = " << candidate_butterfly_set.size() << endl;
    clock_t end = clock();
    cout << "Ordering Time : " << (double) (end - start) / CLOCKS_PER_SEC << " s " << endl;

    start = clock();
    ProbabilityEstimation(model, candidate_butterfly_set);
//    ProbabilityEstimationKarpLuby(model, candidate_butterfly_set);
    end = clock();
    cout << "Sampling Time : " << (double) (end - start) / CLOCKS_PER_SEC << " s " << endl;
}
