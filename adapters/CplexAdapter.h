//
// Created by hlefebvr on 12/11/19.
//

#ifndef ED_SOLVER_CPLEXADAPTER_H
#define ED_SOLVER_CPLEXADAPTER_H

#include <ilcplex/ilocplex.h>
#include "SolverAdapter.h"

namespace L {
    class CplexAdapter;
}

/**
 * \brief Adapter for the IBM Cplex solver, for MILP models
 */
class L::CplexAdapter : public SolverAdapter {

};


#endif //ED_SOLVER_CPLEXADAPTER_H
