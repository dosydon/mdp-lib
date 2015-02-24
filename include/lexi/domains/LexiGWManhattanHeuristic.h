#ifndef MDPLIB_LEXIGWSECONDHEUR_H
#define MDPLIB_LEXIGWSECONDHEUR_H

#include "../../util/general.h"
#include "../../heuristic.h"
#include "../../state.h"

#include "LexiGridWorldProblem.h"
#include "LexiGridWorldState.h"

namespace mllexi
{

class LexiGWManhattanHeuristic : public mlcore::Heuristic
{
private:
    LexiGridWorldProblem* problem_;
    double costDown_;

public:
    LexiGWManhattanHeuristic(LexiGridWorldProblem* problem, double costDown)
    {
        problem_ = problem;
        costDown_ = costDown;
    }

    virtual double cost(const mlcore::State* s) const
    {
        LexiGridWorldState* gws = (LexiGridWorldState*) s;
        double cost_ = mdplib::dead_end_cost;
        if (gws->x() == -1) // absorbing dummy state
            return 0;
        for (PairDoubleMap::iterator it = problem_->goals()[0].begin();
                                        it != problem_->goals()[0].end(); it++) {
            std::pair<int,int> goal = it->first;
            double value = it->second;
            if (gws->x() == goal.first && gws->y() == goal.second)
                return value;
            double distX = abs(gws->x() - goal.first);
            double distY = abs(gws->y() - goal.second);
            double mult = (gws->y() > goal.second) ? costDown_ : 1.0;
            double goalCost = problem_->actionCost() * (distX + mult * distY) + value;
            if (goalCost < cost_)
                cost_ = goalCost;
        }
        return cost_;
    }
};

}

#endif // MDPLIB_LEXIGWSECONDHEUR_H

