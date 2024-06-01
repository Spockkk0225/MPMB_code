#include "Model.h"

bool cmp(const pair<Butterfly, double> &e1, const pair<Butterfly, double> &e2) { return e1.second > e2.second; }

void Model::PrintResult(double coe) {
    vector<pair<Butterfly, double>> b;
    for (auto &b_p: result) {
        b.push_back(b_p);
    }
    cout << "S{MB} size = " << result.size() << endl;
    sort(b.begin(), b.end(), cmp);
    int i = 0;
    for (auto &b_p: b) {
        i++;
        if (i > 50) break;
        cout << "Butterfly("
             << b_p.first.u1
             << "," << b_p.first.u2
             << "," << b_p.first.v1
             << "," << b_p.first.v2
             << ")   weight=" << b_p.first.weight
             << "  times=" << b_p.second
             << "  percent=" << b_p.second / coe
             << "  butterfly pr=" << b_p.first.pr
             << "  butterfly combined weight=" << b_p.first.combined_weight << endl;
    }
}


// e1 = (u, v1)
// e2 = (u, v2)
// a  = (v1, u, v2)
Angle Model::EdgeToAngle(Edge e1, Edge e2) {
    Angle a{};
    a.weight = e1.weight + e2.weight;
    a.pr = e1.pr * e2.pr;
    a.combined_weight = (int) (e1.weight * e1.pr + e2.weight * e2.pr);

    if (e1.u == e2.u) {
        //
    } else if (e1.u == e2.v) {
        swap(e2.u, e2.v);

    } else if (e1.v == e2.u) {
        swap(e1.u, e1.v);

    } else if (e1.v == e2.v) {
        swap(e1.u, e1.v);
        swap(e2.u, e2.v);

    } else {
        assert(0);
    }
    if (e1.v > e2.v) swap(e1, e2);
    a.u = e1.v;
    a.v = e1.u;
    a.w = e2.v;
    a.pr_u_v = e1.pr;
    a.pr_v_w = e2.pr;

    return a;
}

// a1 = (u1, v1, u2)
// a2 = (u1, v2, u2)
// b  = (u1, u2, v1, v2)
// u1 < u2
Butterfly Model::AngleToButterfly(const Angle &a1, const Angle &a2) {
//    if (a1.u != a2.u) {
//        cout << a1.u << " " << a1.v << " " << a1.w << endl;
//        cout << a2.u << " " << a2.v << " " << a2.w << endl;
//    }
    assert(a1.u == a2.u);
    assert(a1.w == a2.w);
    Butterfly b{};
    b.weight = a1.weight + a2.weight;
    b.pr = a1.pr * a2.pr;
    b.combined_weight = a1.combined_weight + a2.combined_weight;

    if (a1.v < a2.v) {
        b.u1 = a1.u;
        b.u2 = a1.w;
        b.v1 = a1.v;
        b.v2 = a2.v;
        b.pr_u1_v1 = a1.pr_u_v;
        b.pr_u2_v1 = a1.pr_v_w;
        b.pr_u1_v2 = a2.pr_u_v;
        b.pr_u2_v2 = a2.pr_v_w;
    } else {
        b.u1 = a1.u;
        b.u2 = a1.w;
        b.v1 = a2.v;
        b.v2 = a1.v;
        b.pr_u1_v1 = a2.pr_u_v;
        b.pr_u2_v1 = a2.pr_v_w;
        b.pr_u1_v2 = a1.pr_u_v;
        b.pr_u2_v2 = a1.pr_v_w;
    }

    if (is_left[b.u1] == 0) {
        swap(b.u1, b.v1);
        swap(b.u2, b.v2);
        swap(b.pr_u1_v2, b.pr_u2_v1);
    }

    return b;
}

unordered_map<int, int> Model::is_left{};