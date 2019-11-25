//
// Created by hlefebvr on 14/11/19.
//

#include "Solver.h"

void L::Solver::solve() {
    clock_t begin_clock = clock();
    before_actually_solving_hook();
    actually_solve_hook();
    _last_execution_time = float(clock() - begin_clock) / CLOCKS_PER_SEC;
    before_saving_results_hook();
    save_results_hook();
}

float L::Solver::last_execution_time() {
    return _last_execution_time;
}

