#include "../../include/solvers/BoundedRTDPSolver.h"

namespace mlsolvers
{

BoundedRTDPSolver::BoundedRTDPSolver(mlcore::Problem* problem,
                                     double epsilon,
                                     int maxTrials)
    : problem_(problem), epsilon_(epsilon), maxTrials_(maxTrials), tau_(100.0)
{ }


void BoundedRTDPSolver::trial(mlcore::State* s) {
    mlcore::State* tmp = s;
    std::list<mlcore::State*> visited;
    while (true) {
        if (problem_->goal(tmp))
            break;
		if (visited.size() > 1000)
			break;
        visited.push_front(tmp);
        this->bellmanUpdate(tmp);
        // Explore using the lower bound.
        mlcore::Action* a = lowerBoundGreedyPolicy_[tmp];
        if (tmp->deadEnd())
            break;
        tmp = sampleBiased(tmp, a);
        if (tmp == nullptr)
            break;
    }

    while (!visited.empty()) {
        tmp = visited.front();
        visited.pop_front();
        this->bellmanUpdate(tmp);
    }
}

void BoundedRTDPSolver::initializeUpperBound(mlcore::State* s) {
    upperBounds_[s] = problem_->goal(s) ? 0.0 : 30.0;
}

mlcore::State*
BoundedRTDPSolver::sampleBiased(mlcore::State* s, mlcore::Action* a) {
    double B = 0.0;
    std::vector< std::pair<mlcore::State*, double> > statesAndScores;
    for (const mlcore::Successor& su : problem_->transition(s, a)) {
        double score =
            su.su_prob * (upperBounds_[su.su_state] - su.su_state->cost());
        statesAndScores.push_back(std::make_pair(su.su_state, score));
        B += score;
    }
    if ((upperBounds_[s] - s->cost()) == 0
            || B < (upperBounds_[s] - s->cost()) / tau_)
        return nullptr;
    double pick = kUnif_0_1(kRNG);
    double acc = 0;
    for (auto stateAndScore : statesAndScores) {
        acc += stateAndScore.second / B;
        if (acc >= pick) {
            return stateAndScore.first;
        }
    }
    assert(false);
    return nullptr;
}

double BoundedRTDPSolver::bellmanUpdate(mlcore::State* s) {
    double bestLowerBound = problem_->goal(s) ? 0.0 : mdplib::dead_end_cost;
    double bestUpperBound = problem_->goal(s) ? 0.0 : mdplib::dead_end_cost;
    bool hasAction = false;
    mlcore::Action* bestActionLowerBound = nullptr;
    mlcore::Action* bestActionUpperBound = nullptr;
    for (mlcore::Action* a : problem_->actions()) {
        if (!problem_->applicable(s, a))
            continue;
        hasAction = true;
        double lowerBoundAction = 0.0;
        double upperBoundAction = 0.0;
        for (const mlcore::Successor& su : problem_->transition(s, a)) {
            // state->cost() stores the lower bound.
            lowerBoundAction += su.su_prob * su.su_state->cost();
            if (upperBounds_.count(su.su_state) == 0)
                initializeUpperBound(su.su_state);
            upperBoundAction += su.su_prob * upperBounds_[su.su_state];
        }
        lowerBoundAction =
            (lowerBoundAction * problem_->gamma()) + problem_->cost(s, a);
        lowerBoundAction = std::min(mdplib::dead_end_cost, lowerBoundAction);
        upperBoundAction =
            (upperBoundAction * problem_->gamma()) + problem_->cost(s, a);
        upperBoundAction = std::min(mdplib::dead_end_cost, upperBoundAction);
        if (upperBoundAction <= bestUpperBound) {
            bestUpperBound = upperBoundAction;
            bestActionUpperBound = a;
        }
        if (lowerBoundAction <= bestLowerBound) {
            bestLowerBound = lowerBoundAction;
            bestActionLowerBound = a;
        }
    }
    if (!hasAction && bestLowerBound >= mdplib::dead_end_cost)
        s->markDeadEnd();
    s->setCost(bestLowerBound);
    upperBounds_[s] = bestUpperBound;
    lowerBoundGreedyPolicy_[s] = bestActionLowerBound;
    s->setBestAction(bestActionUpperBound);
    return bestUpperBound - bestLowerBound;
}

mlcore::Action* BoundedRTDPSolver::solve(mlcore::State* s0) {
    int trials = 0;
    while (trials++ < maxTrials_) {
		std::cout << trials << std::endl;
        trial(s0);
        if (upperBounds_[s0] - s0->cost() < epsilon_)
            break;
    }
}

}

#ifdef TEST
#include "catch.hpp"
#include "Problem.h"
#include "util/simulate.h"
#include "domains/racetrack/RacetrackProblem.h"
#include "domains/racetrack/RTrackDetHeuristic.h"

#include <limits>
#include <math.h>

using namespace mlsolvers;
using namespace std;
using namespace mlcore;

TEST_CASE("run BRTDP on race track", "[BRTDP]")
{
    string trackName = "data/tracks/known/square-4-error.track";
    int mds = -1;
    double perror = 0.10;
    double pslip = 0.20;
    double tol = 1.0e-3;
	int trials = 1000;
    int numSims = 100;

	Heuristic* heuristic = new RTrackDetHeuristic(trackName.c_str());

    Problem* problem = new RacetrackProblem(trackName.c_str());
    ((RacetrackProblem*) problem)->pError(perror);
    ((RacetrackProblem*) problem)->pSlip(pslip);
    ((RacetrackProblem*) problem)->mds(mds);
	problem->generateAll();
    problem->setHeuristic(heuristic);

	Solver* solver = new BoundedRTDPSolver(problem, tol, trials);

	REQUIRE(400270 == problem->states().size());
	std::vector<double> results =
		simulate(solver, "lrtdp", problem, numSims, -1, false, 0, false, false);
	double err = 1e-1;
	REQUIRE(results[0] + err >=  11.66);
	REQUIRE(results[0] - err <=  11.66);
	delete problem;
	delete solver;
}
#endif
