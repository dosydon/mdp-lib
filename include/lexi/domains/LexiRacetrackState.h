#ifndef MDPLIB_LEXIRACETRACKSTATE_H
#define MDPLIB_LEXIRACETRACKSTATE_H

#include <functional>

#include "../../state.h"
#include "../../problem.h"
#include "../../../include/lexi/lexi_problem.h"
#include "../../../include/lexi/lexi_state.h"

#include "../../../include/domains/racetrack/RacetrackState.h"

namespace mllexi
{

class LexiRacetrackState : public LexiState
{
private:
    int x_;
    int y_;
    int vx_;
    int vy_;
    bool safe_;

    /* A cache of all successors (for all actions) of this state */
    std::vector<mlcore::SuccessorsList> allSuccessors_;

    virtual std::ostream& print(std::ostream& os) const;

public:
    /**
     * Creates a state for the given racetrack problem with the given (x,y) position
     * and (vx,vy) velocity, safety indicator.
     */
    LexiRacetrackState(int x, int y, int vx, int vy, bool safe, LexiProblem* problem);

    virtual ~LexiRacetrackState() {}

    int x() const { return x_; }

    int y() const { return y_; }

    int vx() const { return vx_; }

    int vy() const { return vy_; }

    bool safe() const { return safe_; }

    void safe(bool safeValue) { safe_ = safeValue; }

    /**
     * Returns a pointer to the successor cache of this state.
     */
    std::vector<mlcore::SuccessorsList>* allSuccessors() { return &allSuccessors_; }

    /**
     * Overrides method from State.
     */
    virtual mlcore::State& operator=(const mlcore::State& rhs);

    /**
     * Overrides method from State.
     */
    virtual bool operator==(const mlcore::State& rhs) const;

    /**
     * Overrides method from State.
     */
    virtual bool equals(mlcore::State* other) const;

    /**
     * Overrides method from State.
     */
    virtual int hashValue() const;
};

}

#endif // MDPLIB_LEXIRACETRACKSTATE_H
