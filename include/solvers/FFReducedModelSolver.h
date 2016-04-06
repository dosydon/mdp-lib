#ifndef MDPLIB_FFREDUCEDMODELSOLVER_H
#define MDPLIB_FFREDUCEDMODELSOLVER_H

#include <string>

#include "../ppddl/PPDDLState.h"

#include "../Action.h"
#include "../Problem.h"

#include "Solver.h"


namespace mlsolvers
{

/**
 * Implements a re-plannner that integrates FF-replan with the
 * reduced models framework.
 */
class FFReducedModelSolver : public Solver
{
private:
    /* The problem to solve. */
    mlcore::Problem* problem_;

    /* The file where FF is to be called at. */
    std::string ffExecFilename_;

    /* The file name where the determinized PPDDL domain is defined. */
    std::string determinizedDomainFilename_;

    /*
     * The file name with the template used to create the problems to be solved
     * by FF.
     */
    std::string templateProblemFilename_;

    /*
     * The file name of the updated problems solved by FF.
     */
    const std::string currentProblemFilename_ = "/tmp/fftmpfile";

public:
    FFReducedModelSolver(mlcore::Problem* problem,
                         std::string ffExecFilename,
                         std::string determinizedDomainFilename,
                         std::string templateProblemFilename) :
        problem_(problem),
        ffExecFilename_(ffExecFilename),
        determinizedDomainFilename_(determinizedDomainFilename),
        templateProblemFilename_(templateProblemFilename) { }

    virtual ~FFReducedModelSolver() {}

    virtual mlcore::Action* solve(mlcore::State* s0) {}

    /**
     * Returns a string with the predicates in the given state.
     */
    std::string extractStatePredicates(mlppddl::PPDDLState* state);

    /**
     * Replaces the initial state in the problem file with the current state
     * and creates a new file with the new state.
     */
    void replaceInitStateInProblemFile(std::string currentStatePredicates);

    /**
     * Calls the FF planner and gets an action for the current state.
     */
    std::string getActionNameFromFFPlanner();

    /**
     * Returns the action with the given name.
     */
    mlcore::Action* getActionFromName(std::string actionName);
};

}

#endif // MDPLIB_FFREDUCEDMODELSOLVER_H
