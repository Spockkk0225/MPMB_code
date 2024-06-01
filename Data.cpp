#include "Data.h"

void LoadData(Model &model) {
    model.fin.open(R"(..\Datasets\Brain.txt)");
//    model.fin.open(R"(..\Datasets\MovieLens.txt)");
//    model.fin.open(R"(..\Datasets\Joker.txt)");
//    model.fin.open(R"(..\Datasets\Mouse.txt)");

    model.fin >> model.num_l >> model.num_r >> model.num_e;
    for (int i = 1; i <= model.num_l; i++) {
        model.node_l.push_back(i);
        model.node.push_back(i);
        Model::is_left[i] = 1;
    }
    for (int i = 1; i <= model.num_r; i++) {
        model.node_r.push_back(model.num_l + i);
        model.node.push_back(model.num_l + i);
    }

    model.neighbor.resize(model.num_l + model.num_r + 1);

    for (int i = 1; i <= model.num_e; i++) {
        int u, v;
        double w, p;
        model.fin >> u >> v >> w >> p;
//        p = min(p * 1.2, 1.0);
        model.neighbor[u].push_back(Edge{u, v, w, p});
        model.neighbor[v].push_back(Edge{v, u, w, p});
    }

    cout << "read finish: L=" << model.num_l << " R=" << model.num_r << " E=" << model.num_e << "\n";
}

// this loading function is only used for the use case
void LoadDataBrain(Model &model) {

    cout << "Load Data" << endl;
    model.fout << "Load Data" << endl;

    model.num_l = 58;
    model.num_r = 58;
    for (int i = 1; i <= model.num_l; i++) {
        model.node_l.push_back(i * 2 - 1);
        model.node.push_back(i * 2 - 1);
        Model::is_left[i * 2 - 1] = 1;
    }
    for (int i = 1; i <= model.num_l; i++) {
        model.node_r.push_back(i * 2);
        model.node.push_back(i * 2);
    }

    /*
     * pr = 0.01 ~ 1.00
     */
    model.num_e = 0;
    model.neighbor.resize(model.num_l + model.num_r + 1);
    double max_prob = 0;

    vector<vector<double>> p(model.num_l + model.num_r + 1, vector<double>(model.num_l + model.num_r + 1, 0));
//    model.fin.open(R"(..\probability-50012.txt)");
//    model.fin.open(R"(..\probability-50033.txt)");
    for (int i = 1; i <= model.num_l + model.num_r; i++) {
        for (int j = 1; j <= model.num_l + model.num_r; j++) {
            model.fin >> p[i][j];
            p[i][j] = fabs(p[i][j]);
            max_prob = max(max_prob, p[i][j]);
        }
    }
    std::cout << "max_prob=" << max_prob << "\n";
    max_prob = 1;
    model.fin.close();
    vector<vector<double>> d(model.num_l + model.num_r + 1, vector<double>(model.num_l + model.num_r + 1, 0));
    model.fin.open(R"(..\Butterfly\distance.txt)");
    for (int i = 1; i <= model.num_l + model.num_r; i++) {
        for (int j = 1; j <= model.num_l + model.num_r; j++) {
            model.fin >> d[i][j];
        }
    }
    model.fin.close();

    for (auto &u: model.node_l) {
        for (auto &v: model.node_r) {
//            if (rand() % 10 <= 5) continue;
            model.num_e++;
//            model.neighbor[u].push_back(Edge{u, v, (int)d[u][v], p[u][v] / max_prob});
//            model.neighbor[v].push_back(Edge{v, u, (int)d[u][v], p[u][v] / max_prob});
        }
    }
    cout << "edge num = " << model.num_e << endl;

//    for (auto &u : model.node) {
//        for (auto &edge : model.neighbor[u]) {
////            printf("%d %d %d %.2f\n", u, edge.tar, edge.weight, edge.pr);
////            cout << edge.u << " " << edge.v << " " << edge.weight << " " << edge.pr << endl;
//            model.fout << edge.u << " " << edge.v << " " << edge.weight << " " << edge.pr << endl;
//        }
//    }



}