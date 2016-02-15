#ifndef MDPLIB_SOLVER_H
#define MDPLIB_SOLVER_H

#include <random>
#include <cassert>
#include <vector>
#include <mutex>

#include "../Heuristic.h"
#include "../Problem.h"
#include "../State.h"
#include "../util/general.h"

#define bb_cost first
#define bb_action second

namespace mlsolvers
{
/**
 * A mutex used by BellmanUpdate in order to avoid race conditions while
 * planning/executing concurrently.
 */
extern std::mutex bellman_mutex;

/**
 * Random number generator.
 */
extern std::random_device rand_dev;

/**
 * Mersenne Twister 19937 generator.
 */
extern std::mt19937 gen;

/**
 * Uniform distribution [0,1] generator.
 */
extern std::uniform_real_distribution<> dis;

/**
 * An interface describing planning algorithms.
 */
class Solver
{
public:

    /**
     * Solves the associated problem using this solver.
     *
     * @param s0 the state to start the search at.
     * @return the action recommended by this algorithm for state s0.
     */
    virtual mlcore::Action* solve(mlcore::State* s0) =0;
};

/**
 * Computes the Q-value of a state-action pair.
 * This method assumes that the given action is applicable on the state.
 *
 * @param problem the problem that contains the given state.
 * @param s the state for which the Q-value will be computed.
 * @param a the action for which the Q-value will be computed.
 * @return the Q-value of the state-action pair.
 */
double qvalue(mlcore::Problem* problem, mlcore::State* s, mlcore::Action* a);


/**
 * Computes the weighted-Q-value of a state-action pair.
 * This method assumes that the given action is applicable on the state.
 *
 * @param problem the problem that contains the given state.
 * @param s the state for which the Q-value will be computed.
 * @param a the action for which the Q-value will be computed.
 * @param weight the weight to use.
 * @return a pair of doubles representing the g-value and h-value of the
 *         state-action pair.
  *        The weighted-Q-value can be recovered as g + weight * h.
 */
std::pair<double, double>
weightedQvalue(mlcore::Problem* problem, mlcore::State* s, mlcore::Action* a);


/**
 * Performs a Bellman backup of a state.
 *
 * This backup uses fSSPUDE - see http://arxiv.org/pdf/1210.4875.pdf
 *
 * @param problem the problem that contains the given state.
 * @param s the state on which the Bellman backup will be performed.
 * @return a pair containing the estimated cost and estimated best action
 *         according to this Bellman backup.
 */
std::pair<double, mlcore::Action*>
bellmanBackup(mlcore::Problem* problem, mlcore::State* s);


/**
 * Performs a Bellman backup of a state, and the￼n updates the state with
 * the resulting expected cost and greedy action.
 *
 * @param problem the problem that contains the given state.
 * @param s the state on which the Bellman backup will be performed.
 * @return the residual of the state.
 */
double bellmanUpdate(mlcore::Problem* problem, mlcore::State* s);


/**
 * Performs a weighted-Bellman backup a state, and then updates the state with
 * the resulting expected cost and greedy action.
 *
 * This backup uses fSSPUDE - see http://arxiv.org/pdf/1210.4875.pdf
 *
 * @param problem the problem that contains the given state.
 * @param s the state on which the Bellman backup will be performed.
 * @param weight the weight to use.
 * @return the residual of the state.
 */
double bellmanUpdate(mlcore::Problem* problem, mlcore::State* s, double weight);


/**
 * Samples a successor state of a state and action using the probabilities
 * defined by the problem's transition function.
 *
 * If the given action is a null pointer or if the state is a dead-end
 * (i.e., the transition function returns empty list of successors) this method
 * will return the same state that is given.
 *
 * @param problem the problem that defines the transition function.
 * @param s the state for which the sucessor state will be sampled.
 * @param a the action that generates the successors.
 * @return a successor sampled from the transition function corresponding to the
 *        state and action pair.
 */
mlcore::State*
randomSuccessor(mlcore::Problem* problem, mlcore::State* s, mlcore::Action* a);


/**
 * Returns the action with minimum Q-value for a state.
 *
 * This method assumes that any action stored in state.bestAction() is
 * consistent with the latest expected costs in the problem. This is
 * guaranteed by any solver that performs backup operations through calls
 * to bellmanUpdate(problem, state).

 * When no action is stored in the state, then action is chosen greedily on the
 * states estimated costs as stored in state.cost().
 *
 * @param problem the problem that contains the given state.
 * @param s the state for which the action will be computed.
 * @return the action with minimum Q-value.
 */
mlcore::Action* greedyAction(mlcore::Problem* problem, mlcore::State* s);


/**
 * Computes the residual of a state.
 *
 * @param problem the problem that contains the given state.
 * @param s the state for which the residual will be computed.
 * @return the residual of the given state.
 */
double residual(mlcore::Problem* problem, mlcore::State* s);


/**
 * Returns the most likely outcome after executing the given action on the given
 * state.
 *
 * @param problem the problem that defines the transition function.
 * @param s the state for which the most likely outcome will be obtained.
 * @param a the action executed in the given state.
 * @return the most likely outcome of the state and action pair.
 */
mlcore::State* mostLikelyOutcome(mlcore::Problem* problem,
                                 mlcore::State* s,
                                 mlcore::Action* a);


/**
 * Samples a trial of the greedy policy implied by the current state
 * values and returns the accumulated discounted cost (using problem->gamma()).
 * The trial starts at the given state s.
 *
 * @param problem the problem that defines the transition function.
 * @param s the initial state for the trial.
 * @return the accumulated cost of this trial.
 */
double sampleTrial(mlcore::Problem* problem, mlcore::State* s);


/**
 * Gets all reachable states in problem up to the given horizon
 * and stores them in the set reachableStates.
 *
 * @param problem the problem describing the state space to traverse.
 * @param horizon the horizon limit for the search.
 * @param horizon a set to store the reachable states in.
 */
void getReachableStates(mlcore::Problem* problem,
                        int horizon,
                        mlcore::StateSet& reachableStates);

} // mlsolvers


#endif // MDPLIB_SOLVER_H
