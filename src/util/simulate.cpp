#include <climits>

#include "solvers/HDPSolver.h"
#include "solvers/Solver.h"
#include "solvers/UCTSolver.h"
#include "solvers/SoftFLARESSolver.h"

#include "domains/ctp/CTPOptimisticHeuristic.h"
#include "domains/ctp/CTPProblem.h"
#include "domains/ctp/CTPState.h"

#include "util/simulate.h"
#include "util/flags.h"
using namespace mdplib;
using namespace mlcore;
using namespace mlsolvers;
using namespace std;

bool mustReplan(Solver* solver, string algorithm, State* s, int plausTrial) {
    if (flag_is_registered("online"))
      return true;
    if (algorithm == "flares") {
      return !s->checkBits(mdplib::SOLVED_FLARES);
    }
    if (algorithm == "lrtdp") {
    return !s->checkBits(mdplib::SOLVED);
    }
    if (algorithm == "soft-flares") {
      if (s->checkBits(mdplib::SOLVED))
          return false;
      SoftFLARESSolver* flares = static_cast<SoftFLARESSolver*>(solver);
      return !flares->labeledSolved(s);
    }
    if (algorithm == "hdp") {
      if (flag_is_registered("i")) {
          int j = INT_MAX;
          if (flag_is_registered_with_value("j")) {
              j = stoi(flag_value("j"));
          }
          if (plausTrial >= j) {
              static_cast<HDPSolver*>(solver)->clearLabels();
              return true;
          }
      }
    }
    if (algorithm == "ssipp") {
        return !s->checkBits(mdplib::SOLVED_SSiPP);
    }
    if (algorithm == "uct") {
        return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//                              SIMULATION CODE                              //
///////////////////////////////////////////////////////////////////////////////
void updateStatistics(double cost, int n, double& mean, double& M2)
{
    double delta = cost - mean;
    mean += delta / n;
    M2 += delta * (cost - mean);
}

vector<double> simulate(Solver* solver,
                        string algorithm,
						Problem* problem,
                        int numSims,
                        int maxTime = -1,
                        bool perReplan = false,
						int verbosity = 0,
						bool useUpperBound = false)
{
    double expectedCost = 0.0;
    double variance = 0.0;
    double totalTime = 0.0;
    double longestTime = 0.0;
    StateSet statesSeen;
    int cnt = 0;
    int numDecisions = 0;
    clock_t simulationsStartTime = clock();
    for (int i = 0; i < numSims; i++) {
        if (verbosity >= 100)
            cout << " ********* Simulation Starts ********* " << endl;
        clock_t startTime, endTime;
        if (i == 0 && !flag_is_registered("no-initial-plan")) {
            for (State* s : problem->states())
                s->reset();
            if (maxTime > 0) {
                solver->maxPlanningTime(maxTime);
            }
            startTime = clock();
            if (algorithm == "uct") {
                static_cast<UCTSolver*>(solver)->reset();
            } else if (algorithm != "greedy") {
                solver->solve(problem->initialState());
            }
            endTime = clock();
            double planTime = (double(endTime - startTime) / CLOCKS_PER_SEC);
            totalTime += planTime;
            longestTime = std::max(longestTime, planTime);
            numDecisions++;
        }
        if (verbosity >= 10) {
            cout << "Starting simulation " << i << endl;
        }
        State* tmp = problem->initialState();
        if (verbosity >= 100) {
            cout << "Estimated cost " <<
                problem->initialState()->cost() << endl;
        }
        double costTrial = 0.0;
        int plausTrial = 0;
        while (!problem->goal(tmp)) {
            statesSeen.insert(tmp);
            Action* a;
            if (mustReplan(solver, algorithm, tmp, plausTrial)) {
                startTime = clock();
                int simulationsElapsedTime =
                    std::ceil(1000 * (double(startTime - simulationsStartTime)
                                / CLOCKS_PER_SEC));
                if (maxTime > -1) {
                    int planningTime = perReplan ?
                        maxTime : std::max(0, maxTime - simulationsElapsedTime);
                    solver->maxPlanningTime(planningTime);
                }
                if (algorithm != "greedy")
                    solver->solve(tmp);
                endTime = clock();
                double planTime =
                    (double(endTime - startTime) / CLOCKS_PER_SEC);
                totalTime += planTime;
                longestTime = std::max(longestTime, planTime);
                numDecisions++;
                a = greedyAction(problem, tmp);
            } else {
                if (useUpperBound) {
                    // The algorithms that use upper bounds store the
                    // greedy action with respect to the upper bound
                    // in State::bestAction_
                    a = tmp->bestAction();
                }
                else {
                    a = greedyAction(problem, tmp);
                }
            }

            if (verbosity >= 1000) {
                cout << "State/Action: " << tmp << " " << a << " " << endl;
            }

            costTrial += problem->cost(tmp, a);
            costTrial = std::min(costTrial, mdplib::dead_end_cost);
            if (costTrial >= mdplib::dead_end_cost) {
                break;
            }
            double prob = 0.0;
            State* aux = randomSuccessor(problem, tmp, a, &prob);
            if (algorithm == "hdp") {
                double maxProb = 0.0;
                for (auto const & sccr : problem->transition(tmp, a))
                    maxProb = std::max(maxProb, sccr.su_prob);
                plausTrial +=
                    static_cast<HDPSolver*>(solver)->kappa(prob, maxProb);
            }
            tmp = aux;
        }
        if (flag_is_registered("ctp")) {
            CTPState* ctps = static_cast<CTPState*>(tmp);
            if (!ctps->badWeather()) {
                cnt++;
                updateStatistics(costTrial, cnt, expectedCost, variance);
            }
        } else {
            cnt++;
            updateStatistics(costTrial, cnt, expectedCost, variance);
            if (verbosity >= 10)
                cout << costTrial << endl;
        }
    }

    if (verbosity >= 1) {
        cout << "Estimated cost " << problem->initialState()->cost() << " ";
        cout << "Avg. Exec cost " << expectedCost << " ";
        cout << "Std. Dev. " << sqrt(variance / (cnt - 1)) << " ";
        cout << "Total time " << totalTime / cnt << " " << endl;
        cout << "States seen " << statesSeen.size() << endl;
        cout << "Avg. time per decision "
             << totalTime / numDecisions << endl
             << "Longest planning time " << longestTime << endl;
        cout << "Num. decisions " << numDecisions << endl;
    } else if (verbosity >= 0) {
        cout << problem->initialState()->cost() << " ";
        cout << expectedCost << " " << sqrt(variance / (cnt - 1)) << " " <<
            totalTime / cnt << " " << totalTime / numDecisions << endl;
    }

    double reportedTime = perReplan ?
        totalTime / numDecisions : totalTime;
    double results[] = {expectedCost,
                        variance / (cnt - 1),
                        reportedTime,
                        double(statesSeen.size())};
    return vector<double>(results, results + sizeof(results) / sizeof(double));
}

