#include "../../include/solvers/LRTDPSolver.h"

namespace mlsolvers
{

LRTDPSolver::LRTDPSolver(mlcore::Problem* problem,
                         int maxTrials,
                         double epsilon) :
    problem_(problem), maxTrials_(maxTrials), epsilon_(epsilon)
{ }


void LRTDPSolver::trial(mlcore::State* s, std::chrono::time_point<std::chrono::high_resolution_clock> start_time) {
    mlcore::State* tmp = s;
    std::list<mlcore::State*> visited;
    while (!tmp->checkBits(mdplib::SOLVED)) {
        if (problem_->goal(tmp))
            break;

        visited.push_front(tmp);

        bellmanUpdate(problem_, tmp);

        if (tmp->deadEnd())
            break;

                                                                                auto begin = std::chrono::high_resolution_clock::now();
        tmp = randomSuccessor(problem_, tmp, tmp->bestAction());
                                                                                auto end = std::chrono::high_resolution_clock::now();
                                                                                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
                                                                                cnt_samples_++;
                                                                                total_time_samples_ += duration;
    }

    while (!visited.empty()) {
        tmp = visited.front();
        visited.pop_front();
                                                                                auto begin = std::chrono::high_resolution_clock::now();
        bool solved = checkSolved(tmp, start_time);
                                                                                auto end = std::chrono::high_resolution_clock::now();
                                                                                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
                                                                                cnt_check_++;
                                                                                total_time_check_ += duration;
        if (!solved) break;
    }
}


bool LRTDPSolver::checkSolved(mlcore::State* s, std::chrono::time_point<std::chrono::high_resolution_clock> start_time)
{
    std::list<mlcore::State*> open, closed;

    mlcore::State* tmp = s;
    if (!tmp->checkBits(mdplib::SOLVED)) {
        open.push_front(s);
    }

    bool rv = true;
    while (!open.empty()) {
        tmp = open.front();
        open.pop_front();

        if (problem_->goal(tmp))
            continue;

        mlcore::Action* a = greedyAction(problem_, tmp);

        if (tmp->deadEnd())
            continue;

        closed.push_front(tmp);
        tmp->setBits(mdplib::CLOSED);

        if (residual(problem_, tmp) > epsilon_) {
            rv = false;
            // The original paper includes this line, but the algorithm
            // seems to work significantly faster without this
            //  continue;
        }

        for (mlcore::Successor su : problem_->transition(tmp, a)) {
            mlcore::State* next = su.su_state;
            if (!next->checkBits(mdplib::SOLVED) &&
                !next->checkBits(mdplib::CLOSED)) {
                open.push_front(next);
            }
        }
    }

    if (rv) {
        for (mlcore::State* sc : closed) {
            sc->setBits(mdplib::SOLVED);
            sc->clearBits(mdplib::CLOSED);
        }
    } else {
        while (!closed.empty()) {
            tmp = closed.front();
            closed.pop_front();
            tmp->clearBits(mdplib::CLOSED);
            bellmanUpdate(problem_, tmp);
        }
    }

    return rv;
}


mlcore::Action* LRTDPSolver::solve(mlcore::State* s0)
{
    int trials = 0;
    auto begin = std::chrono::high_resolution_clock::now();
    while (!s0->checkBits(mdplib::SOLVED) && trials++ < maxTrials_) {
        trial(s0, begin);
    }
// 	std::cout << trials << std::endl;
                                                                                dprint(cnt_samples_, double(total_time_samples_) / cnt_samples_);
                                                                                dprint(cnt_check_, double(total_time_check_) / cnt_check_);
                                                                                dprint(trials);
    return s0->bestAction();
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

TEST_CASE("run LRTDP on race track", "[LRTDP]")
{
    string trackName = "data/tracks/known/square-4-error.track";
    int mds = -1;
    double perror = 0.10;
    double pslip = 0.20;
    double tol = 1.0e-3;
	int trials = 1000000;
    int numSims = 100;

	Heuristic* heuristic = new RTrackDetHeuristic(trackName.c_str());

    Problem* problem = new RacetrackProblem(trackName.c_str());
    ((RacetrackProblem*) problem)->pError(perror);
    ((RacetrackProblem*) problem)->pSlip(pslip);
    ((RacetrackProblem*) problem)->mds(mds);
	problem->generateAll();
    problem->setHeuristic(heuristic);

	Solver* solver = new LRTDPSolver(problem, trials, tol);

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
