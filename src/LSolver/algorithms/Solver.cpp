//
// Created by hlefebvr on 14/11/19.
//

#include "Solver.h"

void L::Solver::solve() {
    clock_t begin_clock = clock();
    actually_solve();
    _last_execution_time = float(clock() - begin_clock) / CLOCKS_PER_SEC;
    save_results();
}

float L::Solver::last_execution_time() {
    return _last_execution_time;
}

