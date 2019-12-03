# L-Solver

The L-Solver is an open and extensible project for solving linear and nonlinear optimization problems 
using decomposition techniques. Its goal is not to compete with any commercial or non-commercial solver
but rather, to offer a modular and generic interface to experiment with decomposition methods. 
Its name comes from the interesting structure for decomposing problems referred to as the "L-shaped structure".

**Dependencies**: L-Solver depends on external solvers for which adapters (i.e., interfaces) have been developped
(see below). For instance, CPLEX may be used for solving the restricted master problem and the sub-problems when executing
a column-generation algorithm.

**IMPORTANT: ONLY MINIMIZATION IS IMPLEMENTED FOR NOW (though it is possible to set an objective's type to "Maximize")**

## Documentation

A documentation is available at [https://hlefebvr.github.io/l-solver/](https://hlefebvr.github.io/l-solver/). Feel free to also check the following tutorials:
- [x] [Modeling and solving a MILP using its natural formulation](https://hlefebvr.github.io/posts/en_lsolver_modeling_milp.html)
- [x] [Dantzig-Wolfe reformulation](https://hlefebvr.github.io/posts/en_lsolver_dantzig_wolfe.html)
- [ ] Writting a custom column generation procedure

## Optimization techniques

- [x] MILP resolution via external solver
- [x] LP resolution via external solver (automatically relaxes integer variables)
- [x] Generic branch-and-bound algorithm (can be inherited to create a custom branch-and-bound, branching rule can be changed with a startegy)
- [x] MILP resolution via branch-and-bound and LP external solver (*todo* should be extended to NLP solvers) 
- [x] Generic column generation (to be used by providing a child class of ColumnIterator and overriding `Column get_next_column()` and `bool is_done()` which are virtual pure methods)
- [x] Generic Dantzig-Wolfe decomposition (just provide a `Decomposition` object with block indicators, the master problem is solved via an external LP solver while the pricing problem can be solved by any solver)
- [x] Generic Dantzig-Wolfe decomposition with branching (i.e., branch-and-price)
- [ ] MINLP resolution via external solver
- [ ] NLP resolution via external solver (automatically relaxes integer variables)
- [ ] Generic epigraph solver for semi-infinite programs
- [ ] Generic Benders decomposition for MILPs
- [ ] Generic Generalize Benders decomposition for MINLPs
- [ ] RLT convexification scheme
- [ ] Generic Benders decomposition with MILP subproblems using RLT convexification scheme
- [ ] Generic Lagrangian decomposition for separable problems  
- [ ] Generic Cutting planes
- [ ] Generic branch-and-cut
- [ ] Branch-and-cut for MILPs

## Optimization solvers adapters

- [x] IBM Cplex (MILP and MIQP solver)
- [ ] Gurobi
- [ ] SCIP
- [ ] Generic Simplex algorithm
