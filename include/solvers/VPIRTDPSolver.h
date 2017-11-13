#ifndef MDPLIB_VPIRTDP_H
#define MDPLIB_VPIRTDP_H

#include "Solver.h"

#include "../State.h"

namespace mlsolvers {

/**
 * An SSP solver using the VPI-RTDP algorithm.
 * https://www.aaai.org/ocs/index.php/IJCAI/IJCAI-09/paper/viewFile/552/879
 */
class VPIRTDPSolver : public Solver
{
private:

    mlcore::Problem* problem_;

    /* The maximum number of trials to perform. */
    int maxTrials_;

    /* Criterion for stopping the algorithm
      (the difference between the bounds). */
    double epsilon_;

    /* Criterion for considering a state as well-known. */
    double tau_;

    /* If the average successor bound gap is larger than |beta_|, then
     * VPI will not be used (not enough information). */
    double beta_;

    /* The probability that we return a successor based solely on the bounds
     * when VPI of all outcomes is 0. */
    double alpha_;

    /* A value for initializing the upper bounds of states. */
    double initialUpperBound_;

    /* Upper bounds on the state costs. */
    mlcore::StateDoubleMap upperBounds_;

    /* Stores a greedy policy on the lower bound. */
    mlcore::StateActionMap lowerBoundGreedyPolicy_;

    /*
     * If true, the algorithm samples according to the transition function.
     * This makes it equivalent to RTDP.
     */
    bool vanillaSample_;

                                                                                bool sampleVPIOld_;


    /* Performs a single BRTDP trial. */
    void trial(mlcore::State* s);

    /* Performs a Bellman update for the state, changing its estimated cost
     * and the values of the bounds. */
    double bellmanUpdate(mlcore::State* s);

    /* Initializes the upper bound for the given state. */
    void initializeUpperBound(mlcore::State* s);

    /*
     * Samples a state biased according to the difference of its bounds.
     * Returns a nullptr if all successor states have "well-known" value.
     */
    mlcore::State* sampleBiasedBounds(mlcore::State* s,
                                      mlcore::Action* sampledAction);

    /*
     * Samples a state according to a myopic VPI analysis.
     */
    mlcore::State* sampleVPI(mlcore::State* s, mlcore::Action* sampledAction);

    /*
     * Samples a state according to a myopic VPI analysis.
     */
    mlcore::State* sampleVPIOld(mlcore::State* s, mlcore::Action* sampledAction);

    /*
     * Computes the VPI given the expected Q-value parameters.
     */
    double computeVPI(double PrSuGivenAlpha, double qValueRemAlpha,
                      double PrSuGivenAction, double qValueRemAction,
                      double lowerBound, double upperBound);

public:
    /**
     * Creates a BRTDP solver for the given problem.
     *
     * @param problem The problem to be solved.
     * @param maxTrials The maximum number of trials to perform.
     * @param epsilon The error tolerance.
     */
    VPIRTDPSolver(mlcore::Problem* problem,
                      double epsilon,
                      int maxTrials = 1000000,
                      double alpha = 1.0,
                      double beta = 45,
                      double tau = 100,
                      double initialUpperBound = 50,
                      bool vanillaSample = false);

    /**
     * Solves the associated problem using the Labeled RTDP algorithm.
     *
     * @param s0 The state to start the search at.
     */
    virtual mlcore::Action* solve(mlcore::State* s0);
                                                                                void sampleVPIOld() { sampleVPIOld_ = true; }
};

}

#endif // MDPLIB_VPIRTDP_H
