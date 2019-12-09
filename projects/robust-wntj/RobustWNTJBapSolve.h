//
// Created by hlefebvr on 08/12/19.
//

#ifndef LSOLVERPROJECT_ROBUSTWNTJBAPSOLVE_H
#define LSOLVERPROJECT_ROBUSTWNTJBAPSOLVE_H

#include <LSolver/environment/Environment.h>
#include <LSolver/modeling/vectors/VariableVector.h>
#include <LSolver/modeling/models/Model.h>
#include <LSolver/algorithms/direct-solver/DirectMILPSolver.h>
#include <LSolver/adapters/CplexAdapter.h>

void solve_by_bap(const RobustWNTJInstance& instance) {
    using namespace L;

    Environment env;
    VariableVector U(env, "U");
    VariableVector y(env, "y");
    VariableVector z(env, "z");
    VariableVector t(env, "t");
    VariableVector v(env, "v");
    Variable u(env, "u");
    ConstraintVector non_anticipativity(env, "non_anticipativity");
    ConstraintVector dualized_cost(env, "dualized_cost");
    ConstraintVector release_date(env, "release_date");
    ConstraintVector non_overlap(env, "non_overlap");
    ConstraintVector deadline(env, "deadline");
    ConstraintVector fix_if_scheduled(env, "fix_if_scheduled");
    ConstraintVector at_most_one(env, "at_most_one");
    Objective obj(env, "minimize_cost");

    U.type(Binary);
    y.type(Binary);
    z.type(Binary);

    // BaP algorithm should not branch on second-stage variables
    y.priority(0);
    z.priority(0);

    const float gamma = 1;
    obj.expression() += gamma * u;
    for (const Job* job : instance.jobs()) {
        const float outsourcing_cost = job->_deadline;
        obj.expression() += outsourcing_cost + (job->_weight - outsourcing_cost) * U(job->_id) + v(job->_id);

        at_most_one(job->_id).type(LessOrEqualTo);
        at_most_one(job->_id).expression() = -1;

        non_anticipativity(job->_id).type(LessOrEqualTo);
        non_anticipativity(job->_id).expression() += U(job->_id) - 1;

        dualized_cost(job->_id).expression() = u + v(job->_id);
    }

    unsigned int k = 0;
    for (const JobOccurence* occ : instance.occurences()) {
        // TODO adapt for robust
        const float tau = occ->_weight;
        const float max_degradation = occ->_weight;
        const float outsourcing_cost = occ->_deadline;

        obj.expression() += outsourcing_cost * y(k);

        release_date(k).expression() = t(k) - occ->_release;
        if (k > 1) non_overlap(k).expression() = t(k-1) - t(k) - occ->_processing_time * y(k) - tau * z(k); // try factorized !
        deadline(k).expression() = t(k) + occ->_processing_time * y(k) + tau * z(k) - instance.max_deadline() - instance.max_deadline() * y(k) - occ->_deadline;
        deadline(k).type(LessOrEqualTo);
        fix_if_scheduled(k).expression() = y(k) - z(k);
        at_most_one(occ->_parent_job_id).expression() += y(k);
        non_anticipativity(occ->_parent_job_id).expression() += y(k);
        dualized_cost(occ->_parent_job_id).expression() += max_degradation * z(k) - max_degradation * y(k);

        k += 1;
    }

    Model model;
    // add objective
    model.add(obj);
    // add constraints
    model.add(non_anticipativity);
    model.add(dualized_cost);
    model.add(release_date);
    model.add(non_overlap);
    model.add(deadline);
    model.add(fix_if_scheduled);
    model.add(at_most_one);
    // add variables
    model.add(U);
    model.add(y);
    model.add(z);
    model.add(t);
    model.add(v);
    model.add(u);

    std::cout << model << std::endl;

    // Static solver
    DirectMILPSolver<CplexAdapter> solver(model);
    solver.solve();

    std::cout << "CPLEX status: " << obj.status() << std::endl;
    std::cout << "Execution time: " << solver.last_execution_time() << std::endl;
    if (obj.status() == Optimal) {
        std::cout << "Objective: " << obj.value() << std::endl;
    }

    // BaP Solver
}

#endif //LSOLVERPROJECT_ROBUSTWNTJBAPSOLVE_H
