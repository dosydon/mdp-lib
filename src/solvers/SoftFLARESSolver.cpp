#include "../../include/solvers/SoftFLARESSolver.h"

#include <cmath>
                                                                                #include <chrono>

#include "../../include/MDPLib.h"

using namespace mlcore;
using namespace std;

namespace mlsolvers
{

SoftFLARESSolver::SoftFLARESSolver(Problem* problem,
                                   int maxTrials,
                                   double epsilon,
                                   double horizon,
                                   TransitionModifierFunction modifierFunction,
                                   DistanceFunction distanceFunction,
                                   double alpha,
                                   bool useProbsForDepth,
                                   bool optimal,
                                   int maxTime) :
        problem_(problem),
        maxTrials_(maxTrials),
        epsilon_(epsilon),
        horizon_(horizon),
        useProbsForDepth_(useProbsForDepth),
        modifierFunction_(modifierFunction),
        distanceFunction_(distanceFunction),
        alpha_(alpha),
        optimal_(optimal),
        maxTime_(maxTime),
        useCache_(true) {
                                                                                dprint("SOFT-FLARES",
                                                                                       "horizon", horizon_,
                                                                                       "alpha", alpha_);
    beta_ = 1 - alpha_;
    if (modifierFunction_ == kLogistic) {
        tau_ = -log((beta_ * beta_) / (alpha_ * alpha_)) / horizon_;
    } else if (modifierFunction_ == kExponential) {
        tau_ = log(beta_ / alpha_) / horizon_;
    } else if (modifierFunction_ == kLinear) {
        tau_ = (beta_ - alpha_) / horizon_;
    }

    modifierCache_.resize(horizon_ + 1);
    for (int i = 0; i <= horizon_; i++) {
        modifierCache_[i] = computeProbUnlabeled(i);
                                                                                dprint(i, modifierCache_[i]);
    }
}


void SoftFLARESSolver::trial(State* s) {
    State* currentState = s;
    list<State*> visited;
    double accumulated_cost = 0.0;
//                                                                                dprint("****************");

    while (!labeledSolved(currentState)) {
        if (problem_->goal(currentState))
            break;

        visited.push_front(currentState);
                                                                                double res = residual(problem_, currentState);
        bellmanUpdate(problem_, currentState);
                                                                                if (res < epsilon_ && residual(problem_, currentState) > epsilon_) {
                                                                                    cerr << "ooops!" << residual(problem_, currentState) << endl;
                                                                                }

        if (currentState->deadEnd()
                || accumulated_cost >= mdplib::dead_end_cost)
            break;

        mlcore::Action* greedy_action = greedyAction(problem_, currentState);
//                                                                                dprint(currentState,
//                                                                                       currentState->residualDistance(),
//                                                                                       greedy_action);
        accumulated_cost += problem_->cost(currentState, greedy_action);
                                                                                auto begin = std::chrono::high_resolution_clock::now();
        currentState = sampleSuccessor(currentState, greedy_action);
                                                                                auto end = std::chrono::high_resolution_clock::now();
                                                                                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
                                                                                cnt_samples_++;
                                                                                total_time_samples_ += duration;

    }
//                                                                                dprint("********");

    while (!visited.empty()) {
        currentState = visited.front();
        visited.pop_front();

                                                                                auto begin = std::chrono::high_resolution_clock::now();
        computeResidualDistances(currentState);
                                                                                auto end = std::chrono::high_resolution_clock::now();
                                                                                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
                                                                                cnt_check_++;
                                                                                total_time_check_ += duration;
        if (!labeledSolved(currentState))
            break;
    }
}

double SoftFLARESSolver::computeProbUnlabeled(mlcore::State* s) {
    double distance = s->residualDistance();
    if (distance < 0)
        return 1.0;
    if (useCache_) {
        return modifierCache_[int(distance)];
    } else {
        return computeProbUnlabeled(distance);
    }
}

double SoftFLARESSolver::computeProbUnlabeled(double distance) {
    assert(distance >= 0);
    if (horizon_ == 0) {
        return beta_;
    }
    if (modifierFunction_ == kStep) {
        return alpha_;
    }
    if (modifierFunction_ == kLogistic) {
        double C = beta_ / alpha_;
        return 1.0 / (1 + C * exp(tau_ * distance));
    }
    if (modifierFunction_ == kExponential) {
        return alpha_ * exp(tau_ * distance);
    }
    if (modifierFunction_ == kLinear) {
        return tau_ *  distance + alpha_;
    }
    assert(false);
}

double SoftFLARESSolver::computeNewDepth(Successor& su, double depth) {
    if (distanceFunction_ == kTrajProb) {
        return depth - log2(su.su_prob);
    } else if (distanceFunction_ == kStepDist) {
        return depth + 1;
    }
}

mlcore::State* SoftFLARESSolver::sampleSuccessor(mlcore::State* s,
                                                 mlcore::Action* a) {
    if (a == nullptr)
        return s;

    double totalScore = 0.0;
    vector<double> modTransitionF;
    for (mlcore::Successor sccr : problem_->transition(s, a)) {
        double p = computeProbUnlabeled(sccr.su_state) * sccr.su_prob;
        modTransitionF.push_back(p);
        totalScore += p;
    }

    double pick = kUnif_0_1(kRNG);
    double acc = 0.0;
    int index = 0;
    for (mlcore::Successor sccr : problem_->transition(s, a)) {
        double p = modTransitionF[index++] / totalScore;
        acc += p;
        if (acc >= pick) {
            return sccr.su_state;
        }
    }
    assert(false);
}

bool SoftFLARESSolver::labeledSolved(State* s) {
    if (s->checkBits(mdplib::SOLVED))
        return true;
    bool labeled = (kUnif_0_1(kRNG) > computeProbUnlabeled(s));
    return labeled;
}

void SoftFLARESSolver::computeResidualDistances(State* s) {
    list<State*> open, closed;

    State* currentState = s;
    if (!currentState->checkBits(mdplib::SOLVED)) {
        open.push_front(currentState);
        currentState->depth(0.0);
    }

    bool rv = true;
    bool subgraphWithinSearchHorizon = true;
    while (!open.empty()) {
        State* currentState = open.front();
        open.pop_front();
        double depth = currentState->depth();
        if (depth > 2 * horizon_) {
            subgraphWithinSearchHorizon = false;
            continue;
        }

        if (problem_->goal(currentState))
            continue;

        closed.push_front(currentState);
        currentState->setBits(mdplib::CLOSED);

        Action* a = greedyAction(problem_, currentState);

        if (currentState->deadEnd())
            continue;

        if (residual(problem_, currentState) > epsilon_) {
            rv = false;
        }

        for (Successor su : problem_->transition(currentState, a)) {
            State* next = su.su_state;
            if (!labeledSolved(next) && !next->checkBits(mdplib::CLOSED)) {
                open.push_front(next);
                next->depth(computeNewDepth(su, depth));
            } else if (!(next->checkBits(mdplib::SOLVED)
                            || next->checkBits(mdplib::CLOSED))) {
                // If this happens, the state was skipped only due to
                // soft-labeling. Thus, there must be parts of the subgraph
                // that are outside of the horizon
                subgraphWithinSearchHorizon = false;
            }
        }
    }
//                                                                                dprint("  closed", closed.size());

    if (rv) {
        for (mlcore::State* state : closed) {
            state->clearBits(mdplib::CLOSED);
            if (subgraphWithinSearchHorizon) {
//                                                                                dprint("  --soft-flares SOLVED", state);
                state->setBits(mdplib::SOLVED);
            } else {
                double depth = state->depth();
                if (depth <= horizon_) {
                    state->residualDistance(horizon_ - depth);
//                                                                                dprint("  --soft-flares resdis",
//                                                                                       state,
//                                                                                       state->residualDistance(),
//                                                                                       "solved",
//                                                                                       labeledSolved(state));
                }
            }
        }
    } else {
        while (!closed.empty()) {
            State* state = closed.front();
            closed.pop_front();
            state->clearBits(mdplib::CLOSED);
            bellmanUpdate(problem_, state);
        }
    }
}


Action* SoftFLARESSolver::solve(State* s0) {
    int trials = 0;
    auto begin = std::chrono::high_resolution_clock::now();
    while (!labeledSolved(s0) && trials < maxTrials_) {
        trial(s0);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::
            duration_cast<std::chrono::milliseconds>(end-begin).count();
        if (maxTime_ > -1 && duration > maxTime_)
            break;
        if (!optimal_)
            trials++;
    }
                                                                                auto end = std::chrono::high_resolution_clock::now();
                                                                                auto duration = std::chrono::
                                                                                    duration_cast<std::chrono::milliseconds>(end-begin).count();
//                                                                                dprint("duration", duration, "trials", trials);
    return s0->bestAction();
}

}   // namespace mlsolvers

