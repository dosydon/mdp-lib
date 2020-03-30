#ifndef MDPLIB_SIMULATE_H
#define MDPLIB_SIMULATE_H
#include "solvers/Solver.h"

using namespace mdplib;
using namespace mlcore;
using namespace mlsolvers;
using namespace std;
void updateStatistics(double cost, int n, double& mean, double& M2);

// Runs [numSims] of the given solver and and returns the results
// (i.e., expectedCost, variance, totalTime, statesSeen).
// Argument [algorithm] is the name of the algorithm implemented by [solver].
// Argument [maxTime], if set to > 0, specifies the maximum time allowed to
// the algorithm to complete all simulations (in milliseconds).
// If [perReplan] is passed, then [maxTime] is used as the maximum time allowed
// per re-planning event.
vector<double> simulate(Solver* solver,
                        string algorithm,
						Problem* problem,
                        int numSims,
                        int maxTime,
                        bool perReplan,
						int verbosity,
						bool useUpperBound);
#endif // MDPLIB_SIMULATE_H
