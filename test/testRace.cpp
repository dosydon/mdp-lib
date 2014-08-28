#include <iostream>
#include <ctime>
#include <fstream>
#include <sstream>
#include <unistd.h>

#include "../include/solvers/solver.h"
#include "../include/solvers/VISolver.h"
#include "../include/solvers/LRTDPSolver.h"
#include "../include/solvers/UCTSolver.h"
#include "../include/solvers/LAOStarSolver.h"
#include "../include/solvers/WeightedLAOStarSolver.h"
#include "../include/solvers/DeterministicSolver.h"

#include "../include/util/general.h"
#include "../include/util/graph.h"

#include "../include/domains/racetrack/RacetrackProblem.h"
#include "../include/domains/racetrack/RacetrackState.h"
#include "../include/domains/racetrack/RacetrackAction.h"
#include "../include/domains/racetrack/RTrackDetHeuristic.h"

using namespace mlcore;
using namespace mlsolvers;
using namespace std;

int main(int argc, char* args[])
{
    Problem* problem = new RacetrackProblem(args[1]);
    ((RacetrackProblem*) problem)->setPError(0.10);
    ((RacetrackProblem*) problem)->setPSlip(0.20);
    ((RacetrackProblem*) problem)->setMDS(-1);
    problem->generateAll();

    Heuristic* heuristic = new RTrackDetHeuristic(args[1]);
    problem->setHeuristic(heuristic);

    cerr << problem->states().size() << " states" << endl;

    DeterministicSolver det(problem);
    clock_t startTime = clock();
    double tol = 0.001;
    if (strcmp(args[2], "wlao") == 0) {
        LAOStarSolver wlao(problem, tol, 1000000, atof(args[4]));
        wlao.solve(problem->initialState());
    } else if (strcmp(args[2], "lao") == 0) {
        LAOStarSolver lao(problem, tol, 1000000);
        lao.solve(problem->initialState());
    } else if (strcmp(args[2], "lrtdp") == 0) {
        LRTDPSolver lrtdp(problem, 1000000000, tol);
        lrtdp.solve(problem->initialState());
    } else if (strcmp(args[2], "det") != 0) {
        cerr << "Unknown algorithm: " << args[2] << endl;
        return -1;
    }

    cerr << "Estimated cost " << problem->initialState()->cost() << endl;
    cerr << problem->initialState()->gValue() << endl;
    cerr << problem->initialState()->hValue() << endl;
    clock_t endTime = clock();
    cerr << "Time: " << (double(endTime - startTime) / CLOCKS_PER_SEC) << endl;

    int nsims = atoi(args[3]);
    int verbosity = 1;
    double expectedCost = 0.0;
    for (int i = 0; i < nsims; i++) {
        State* tmp = problem->initialState();
        if (verbosity > 100) {
            cerr << " ********* Simulation Starts ********* " << endl;
            cerr << tmp << " ";
        }
        while (!problem->goal(tmp)) {
            Action* a = (strcmp(args[2], "det") == 0) ? det.solve(tmp) : tmp->bestAction();
            expectedCost += problem->cost(tmp, a);
            tmp = randomSuccessor(problem, tmp, a);
            if (verbosity > 100) {
                cerr << a << " " << endl;
                cerr << tmp << " ";
            }
        }
        if (verbosity > 100)
            cerr << endl;
    }

    cerr << "Avg. cost " << expectedCost / nsims << endl;

    cerr << bbcount << endl;

    delete problem;
    delete ((RTrackDetHeuristic*) heuristic);
}
