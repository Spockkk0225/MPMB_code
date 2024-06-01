#include "Model.h"
#include "Data.h"
#include "BruteForce.h"
#include "MonteCarloSampling.h"
#include "OrderingSampling.h"
#include "ListingSampling.hpp"
#include "OrderingListingSampling.hpp"

#include <ctime>

int main() {
    clock_t start, end;
    Model model;
//    model.fin.open(R"(..\1.txt)");
    LoadData(model);

//    cout << " BruteForce " << endl;
//    BruteForce(model);

    start = clock();
    cout << endl;
    cout << " MonteCarlo " << endl;
    model.result.clear();
    MonteCarloSampling(model);
    end = clock();
    cout << "MonteCarlo Time : " << (double) (end - start) / CLOCKS_PER_SEC << " s " << endl;

    start = clock();
    cout << endl;
    cout << " Ordering " << endl;
    model.result.clear();
    OrderingSampling(model);
    end = clock();
    cout << "Ordering Time : " << (double) (end - start) / CLOCKS_PER_SEC << " s " << endl;

    start = clock();
    cout << endl;
    cout << " ListingSampling " << endl;
    model.result.clear();
    ListingSampling(model);
    end = clock();
    cout << "MonteCarlo Time : " << (double) (end - start) / CLOCKS_PER_SEC << " s " << endl;

    start = clock();
    cout << endl;
    cout << " OrderingListingSampling " << endl;
    model.result.clear();
    OrderingListingSampling(model);
    end = clock();
    cout << "MonteCarlo Time : " << (double) (end - start) / CLOCKS_PER_SEC << " s " << endl;
}

