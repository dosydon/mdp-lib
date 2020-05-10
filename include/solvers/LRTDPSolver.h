#ifndef MDPLIB_LRTDPSOLVER_H
#define MDPLIB_LRTDPSOLVER_H

#include "../Problem.h"
#include "../Heuristic.h"

#include "Solver.h"

namespace mlsolvers
{

/**
 * An SSP solver using the Labeled RTDP algorithm.
 *
 * See http://www.aaai.org/Papers/ICAPS/2003/ICAPS03-002.pdf
 */
class LRTDPSolver : public Solver
{
private:

    mlcore::Problem* problem_;
    int maxTrials_;
    int maxTime_;
    double epsilon_;

    /* Performs a single LRTDP trial */
    void trial(mlcore::State* s, std::chrono::time_point<std::chrono::high_resolution_clock> start_time);

    /* Checks if the state has been solved. */
    bool checkSolved(mlcore::State* s, std::chrono::time_point<std::chrono::high_resolution_clock> start_time);

                                                                                int cnt_samples_ = 0;
                                                                                long int total_time_samples_ = 0;
                                                                                int cnt_check_ = 0;
                                                                                long int total_time_check_ = 0;

public:
    /**
     * Creates a LRTDP solver for the given problem.
     *
     * @param problem The problem to be solved.
     * @param maxTrials The maximum number of trials to perform.
     * @param epsilon The error tolerance.
     */
    LRTDPSolver(mlcore::Problem* problem, int maxTrials, double epsilon);

    /**
     * Solves the associated problem using the Labeled RTDP algorithm.
     *
     * @param s0 The state to start the search at.
     */
    virtual mlcore::Action* solve(mlcore::State* s0);

    /**
     * Sets the maximum planning time allowed to the algorithm (milliseconds).
     */
    virtual void maxPlanningTime(time_t theTime) { maxTime_ = theTime; }
};

}

#endif // MDPLIB_LRTDPSOLVER_H
