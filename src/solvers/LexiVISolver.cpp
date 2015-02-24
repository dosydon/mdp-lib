#include <list>
#include <climits>
#include <cmath>

#include "../../include/lexi/domains/LexiRacetrackState.h"

#include "../../include/solvers/solver.h"
#include "../../include/solvers/LexiVISolver.h"
#include "../../include/lexi/lexi_problem.h"
#include "../../include/lexi/lexi_state.h"
#include "../../include/util/general.h"

namespace mlsolvers
{

LexiVISolver::LexiVISolver(mlcore::Problem* problem, int maxIter, double tol)
{
    problem_ = problem;
    maxIter_ = maxIter;
    tol_ = tol;
}

mlcore::Action* LexiVISolver::solve(mlcore::State* s0)
{
    for (int i = 0; i < maxIter_; i++) {
        double maxResidual = 0.0;
        dprint2(((mllexi::LexiState *) problem_->initialState())->lexiCost()[0],
                ((mllexi::LexiState *) problem_->initialState())->lexiCost()[1]);
        for (mlcore::State* s : problem_->states()) {
            double residual = lexiBellmanUpdate((mllexi::LexiProblem *) problem_,
                                                 (mllexi::LexiState *) s,
                                                 ((mllexi::LexiProblem *) problem_)->size());
            if (residual > maxResidual)
                maxResidual = residual;
        }
        if (maxResidual < tol_)
            return nullptr;
    }
}

}
