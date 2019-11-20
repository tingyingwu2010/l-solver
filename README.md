# Lambda Based Decomposition Solver

The LBD Solver is an open and extensible project for solving linear and nonlinear optimization problems 
using all kinds of decomposition techniques. Its goal is not to compete with any commercial or non-commercial solver
but rather, to present a modular and generic interface to experiment with decomposition methods. 

**Dependencies**: LBDS depends on external solvers for which adapters (i.e., interfaces) have been developped
(see below). For instance, CPLEX may be used for solving the restricted master problem and the sub-problems when executing
a column-generation algorithm.

## Optimization techniques

**Implemented**:
- DirectMILPSolver: solves a MILP using its natural formulation
- DirectLPSolver: solves an LP using its natural formulation
- BranchAndBoundSolver: generic implementation of branch-and-bound algorithm
- DirectBranchAndBoundSolver: branch-and-bound algorithm using any give direct solver (e.g., DirectLPSolver, DirectQPSolver, DirectNLPSolver)
- ColumnGeneration: column-generation algorithm for LPs and NLPs (i.e., inner-linear approximation)
- DantzigWolfeColumnGenerationSolver: generic solver for Dantzig-Wolfe decomposition for MILP sub-problems and LP master problem
- DantzigWolfeBranchAndPriceSolver: generic solver for Dantzig-Wolfe decomposition for MILP sub-problems and master problem

**Yet to be implemented**: (by priority order)
- DirectMINLPSolver: solves a MINLP solver using its natural formulation
- DirectNLPSolver: solves an NLP solver using its natural formulation
- EpigraphSolver: generic solver for epigraph formulation (i.e., $$\min_x\max_y f(x,y) : y\in\mathcal Y$$)
- BendersSolver: generic Benders decomposition solver for MILP (continuous sub-problem)
- GeneralizedBendersSolver: generic Generalized Benders decomposition solver for MINLP (under separability)
- RLTConvexificationScheme: implementation of the reformulation-linearization technique for MILPs
- IntegerBendersSolver: RLT-based Benders decomposition for MILPs with integral sub-problems
- LagrangianDecomposition: Lagrangian relaxation for MILPs
- CuttingPlanes: generic implementation of cutting planes algorithm
- BranchAndCut: generic implementation of branch-and-cut
- MILPBranchAndCut: sepecialized version of the above

## Optimization solvers adapters

**Implemented**:
- IBM Cplex (MILP and MIQP solver)

**Yet to be implemented**:
- Gurobi
- SCIP
- Generic Simplex algorithm
 
 ## Examples
 
 ### Direct MILP solver
 
 ```cpp
// Knapsack example
std::vector<pair<float, float>> items = { {1,4}, {2,1}, {4,3}, {5,8}, {2,2} };

// Create an environment
Environment env;

// Create variables and constraints
VariableVector x = VariableVector(env, "x");
x.type(VariableType::Binary);
Constraint kp_ctr = Constraint(env, "kp_ctr");
Objective objective = Objective(env, ObjectiveType::Maximize);

for (unsigned long int i = 0, n = items.size() ; i < n ; i += 1 ) {
    objective.expression() += item.first * x(i);
    kp_ctr.expression() += item.second * x(i); 
}

// Declare solver
DirectMILPSolver solver;
solver.add_all(env);
solver.solve();

// Output results
if (solver.status() = SolverStatus::Optimal) {
    cout << "Optimal solution found in " << solver.last_execution_time() << endl;
    cout << "Objective =" << objective.value() << endl;
    
    solver.save_primal_values();
    for (Variable x_i : x)
        cout << x_i.user_defined_name() << " = " << x_i.value() << endl;
}

```
 
## Column-Generation
