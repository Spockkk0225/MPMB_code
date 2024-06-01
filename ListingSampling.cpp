#include "ListingSampling.hpp"

bool cmp(const Butterfly &b1, const Butterfly &b2) { return b1.weight > b2.weight; }

void ListingSampling(Model &model) {
    ComputePriority(model);
    vector<Butterfly> candidate_butterfly_set;

    clock_t start = clock();
    cout << "Start Listing\n";

    for (int &u: model.node) {
        unordered_map<int, vector<Angle>> angle;
        for (auto &edge1: model.neighbor[u]) {
            int v = edge1.v;
            if (model.priority[v] >= model.priority[u]) continue;
            for (auto &edge2: model.neighbor[v]) {
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
                    candidate_butterfly_set.push_back(butterfly);
                }
            }
        }
    }


    cout << "Finish Listing\n";
    cout << "size = " << candidate_butterfly_set.size() << "\n";

    sort(candidate_butterfly_set.begin(), candidate_butterfly_set.end(), cmp);

    cout << "finish sort\n";
    clock_t end = clock();
    cout << "Listing Time : " << (double) (end - start) / CLOCKS_PER_SEC << " s " << endl;


    start = clock();
//    ProbabilityEstimation(model, candidate_butterfly_set);
    ProbabilityEstimationKarpLuby(model, candidate_butterfly_set);
    end = clock();
    cout << "Listing Time : " << (double) (end - start) / CLOCKS_PER_SEC << " s " << endl;
}

bool EdgeExist(int u, int v, double pr, unordered_map<int, unordered_map<int, int>> &edge_exist) {
    if (edge_exist[u][v] == 0) {
        if (rand() % 100 <= pr * 100) {
            edge_exist[u][v] = 1;
        } else {
            edge_exist[u][v] = 2;
        }
    }
    if (edge_exist[u][v] == 1) {
        return true;
    } else {
        return false;
    }
}

void ProbabilityEstimation(Model &model, vector<Butterfly> &candidate_butterfly_set) {
//    unordered_map<Butterfly, double, Butterfly::HashFunction> result;
    for (int i = 0; i < model.monte_carlo_iteration; i++) {
        double w_max = 0;
        vector<Butterfly> max_butterfly_set;
        // 0: not sampled   1: exists    2: not exists
        unordered_map<int, unordered_map<int, int>> edge_exist;
        for (auto &butterfly: candidate_butterfly_set) {
            if (butterfly.weight < w_max) break;

            if (!EdgeExist(butterfly.u1, butterfly.v1, butterfly.pr_u1_v1, edge_exist)) continue;
            if (!EdgeExist(butterfly.u1, butterfly.v2, butterfly.pr_u1_v2, edge_exist)) continue;
            if (!EdgeExist(butterfly.u2, butterfly.v1, butterfly.pr_u2_v1, edge_exist)) continue;
            if (!EdgeExist(butterfly.u2, butterfly.v2, butterfly.pr_u2_v2, edge_exist)) continue;

            max_butterfly_set.push_back(butterfly);
            w_max = butterfly.weight;
        }

        for (auto &butterfly: max_butterfly_set) {
            model.result[butterfly] += 1;
//            result[butterfly] += 1;
        }

    }

    model.PrintResult(model.monte_carlo_iteration);
}

bool Check(int &u, int &v, Butterfly &b) {
    if ((u == b.u1 || u == b.u2) && (v == b.v1 || v == b.v2)) {
        return false;
    } else {
        return true;
    }
}

double Except(Butterfly &b1, Butterfly &b2) {
    double p = 1;
    if (Check(b1.u1, b1.v1, b2)) p *= b1.pr_u1_v1;
    if (Check(b1.u1, b1.v2, b2)) p *= b1.pr_u1_v2;
    if (Check(b1.u2, b1.v1, b2)) p *= b1.pr_u2_v1;
    if (Check(b1.u2, b1.v2, b2)) p *= b1.pr_u2_v2;
    return p;
}

void Add(Butterfly &b1, Butterfly &b2, unordered_map<int, unordered_map<int, int>> &edge_exist) {
    if (Check(b1.u1, b1.v1, b2)) edge_exist[b1.u1][b1.v1] = 1;
    if (Check(b1.u1, b1.v2, b2)) edge_exist[b1.u1][b1.v2] = 1;
    if (Check(b1.u2, b1.v1, b2)) edge_exist[b1.u2][b1.v1] = 1;
    if (Check(b1.u2, b1.v2, b2)) edge_exist[b1.u2][b1.v2] = 1;
}

bool Exist(Butterfly &b1, Butterfly &b2, unordered_map<int, unordered_map<int, int>> &edge_exist) {
//    if (Check(b1.u1, b1.v1, b2)) if (!edge_exist[b1.u1][b1.v1]) return false;
//    if (Check(b1.u1, b1.v2, b2)) if (!edge_exist[b1.u1][b1.v2]) return false;
//    if (Check(b1.u2, b1.v1, b2)) if (!edge_exist[b1.u2][b1.v1]) return false;
//    if (Check(b1.u2, b1.v2, b2)) if (!edge_exist[b1.u2][b1.v2]) return false;
    if (Check(b1.u1, b1.v1, b2)) if (!EdgeExist(b1.u1, b1.v1, b1.pr_u1_v1, edge_exist)) return false;
    if (Check(b1.u1, b1.v2, b2)) if (!EdgeExist(b1.u1, b1.v2, b1.pr_u1_v2, edge_exist)) return false;
    if (Check(b1.u2, b1.v1, b2)) if (!EdgeExist(b1.u2, b1.v1, b1.pr_u2_v1, edge_exist)) return false;
    if (Check(b1.u2, b1.v2, b2)) if (!EdgeExist(b1.u2, b1.v2, b1.pr_u2_v2, edge_exist)) return false;
    return true;
}

void ProbabilityEstimationKarpLuby(Model &model, vector<Butterfly> &candidate_butterfly_set) {
//    int check = false;
//    for (int i = 0;i < min(100, (int)candidate_butterfly_set.size()); i++) {
    for (int i = 0; i < candidate_butterfly_set.size(); i++) {
//        srand(i);
        int cnt = 0;
        int L = 0;
        for (int j = 0; j <= i; j++) {
            if (candidate_butterfly_set[j].weight == candidate_butterfly_set[i].weight) {
                L = j - 1;
                break;
            }
        }

        double S = 0;
        vector<double> pr;
        for (int j = 0; j <= L; j++) {
            double p = Except(candidate_butterfly_set[j], candidate_butterfly_set[i]);
            S += p;
            pr.push_back(p);
        }
        for (auto &p: pr) {
            p /= S;
        }

        double ratio = candidate_butterfly_set[i].pr * S * (candidate_butterfly_set[i].pr / 0.1 - 1);
        ratio = max(ratio, candidate_butterfly_set[i].pr * S);
        int r_limit = (int) (model.monte_carlo_iteration * ratio);
        r_limit = max(r_limit, 1);

        for (int r = 0; r < r_limit; r++) {
            if (pr.size() == 0) break;
            double p = 1.0 * (rand() % 100) / 100;
            int j = 0;
            while (p - pr[j] - 1e-9 > 0) {
                p -= pr[j];
                j++;
            }

            // 0: not sampled   1: exists    2: not exists
            unordered_map<int, unordered_map<int, int>> edge_exist;
            Add(candidate_butterfly_set[j], candidate_butterfly_set[i], edge_exist);
//            for (auto &neighbor : model.neighbor) {
//                for (auto &edge : neighbor) {
//                    EdgeExist(edge.u, edge.v, edge.pr, edge_exist);
//                }
//            }

            cnt++;
            for (int k = 0; k < j; k++) {
                if (Exist(candidate_butterfly_set[k], candidate_butterfly_set[i], edge_exist)) {
                    cnt--;
                    break;
                }
            }

            if (candidate_butterfly_set[i].u1 == 2 && candidate_butterfly_set[i].u2 == 3 &&
                candidate_butterfly_set[i].v1 == 22552 && candidate_butterfly_set[i].v2 == 22553) {
                if ((r + 1) % (r_limit / 100) == 0) {
                    double pp = (1 - 1.0 * cnt / (r + 1) * S) * candidate_butterfly_set[i].pr;
                    cout << r << " " << pp << endl;
                }
            }
        }
        double pp = (1 - 1.0 * cnt / (r_limit) * S) * candidate_butterfly_set[i].pr;
        model.result[candidate_butterfly_set[i]] = pp;
    }
    model.PrintResult(1);

}









