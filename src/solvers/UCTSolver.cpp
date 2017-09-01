#include <vector>
#include <cmath>

#include "../../include/solvers/Solver.h"
#include "../../include/solvers/UCTSolver.h"

namespace mlsolvers
{

mlcore::Action* UCTSolver::pickAction(UCTNode* node, double C)
{
    double best = mdplib::dead_end_cost + 1;
    mlcore::Action* bestAction = nullptr;
    std::vector<mlcore::Action*> unexplored_actions;
                                                                                dprint2("  pick action", node);
    auto actions_qvalue_map = action_qvalues_[node];
    int num_actions = actions_qvalue_map.size();
                                                                                dprint2("  num actions", num_actions);
    for (auto action_and_qvalue : actions_qvalue_map) {
        mlcore::Action* a = action_and_qvalue.first;
        if (!problem_->applicable(node->state_, a))
            continue;
        if (counters_node_action_[node][a] == delta_) {  // unexplored action
            unexplored_actions.push_back(a);
            continue;
        }
                                                                                dprint2("    cost for", a);
        if (use_qvalues_for_c_) {
            C = action_and_qvalue.second;
            C /= sqrt(log(num_actions) / (delta_ + 1) + 1);
        }
        double ucb1 = ucb1Cost(node, a, C);
                                                                                dprint2("    ucb1cost =", ucb1);
        if (ucb1 < best) {
            bestAction = a;
            best = ucb1;
        }
    }
    if (unexplored_actions.size() > 0) {
        int idx = rand() % unexplored_actions.size();
                                                                                dprint2("  unexplored", unexplored_actions[idx]);
        return unexplored_actions[idx];
    }
                                                                                dprint2("  best", bestAction);
    return bestAction;
}

double UCTSolver::ucb1Cost(UCTNode* node, mlcore::Action* a, double C)
{
                                                                                dprint4("      counters (node, action) / qvalue =",
                                                                                        counters_node_[node],
                                                                                        counters_node_action_[node][a],
                                                                                        action_qvalues_[node][a]);
    double cost = action_qvalues_[node][a] - C
        * std::sqrt(std::log(counters_node_[node])
                    / counters_node_action_[node][a]);
    return std::min(cost, mdplib::dead_end_cost);
}

mlcore::Action* UCTSolver::solve(mlcore::State* s0)
{
    UCTNode* root = new UCTNode(s0, 0);
    for (int r = 0; r < max_rollouts_; r++) {
        UCTNode* tmp_node = root;
        std::vector<int> cumCost(cutoff_ + 1);
        std::vector<UCTNode*> nodes_in_rollout(cutoff_ + 1);
        std::vector<mlcore::Action*> actions_in_rollout(cutoff_ + 1);
        int maxSteps = 0;
        for (int i = 1; i <= cutoff_; i++) {
            bool first_time_seen = visited_.insert(tmp_node).second;
            if (first_time_seen) {
                counters_node_[tmp_node] = 0;
                for (mlcore::Action* a : problem_->actions()) {
                    if (!problem_->applicable(tmp_node->state_, a))
                        continue;
                    counters_node_action_[tmp_node][a] = delta_;
                    counters_node_[tmp_node] += delta_; // not sure about this
                    action_qvalues_[tmp_node][a] =
                        qvalue(problem_, tmp_node->state_, a);
                }
            }
            if (problem_->goal(tmp_node->state_))
                break;
            maxSteps = i;
            mlcore::Action* a = pickAction(tmp_node, C_);
            mlcore::State* next =
                randomSuccessor(problem_, tmp_node->state_, a);
            cumCost[i] = cumCost[i - 1] + problem_->cost(tmp_node->state_, a);
            nodes_in_rollout[i] = tmp_node;
            actions_in_rollout[i] = a;
            tmp_node = new UCTNode(next, tmp_node->depth_ + 1);
        }

        for (int i = 1; i <= maxSteps; i++) {
            UCTNode* node = nodes_in_rollout[i];
            mlcore::Action* a = actions_in_rollout[i];
            double newq = counters_node_action_[node][a]
                * action_qvalues_[node][a]
                + cumCost[maxSteps] - cumCost[i - 1];
            newq /= (counters_node_action_[node][a] + 1);
                                                                                dprint5("update-qvalue", node, a,
                                                                                        newq,
                                                                                        cumCost[maxSteps] - cumCost[i - 1]);
            action_qvalues_[node][a] = newq;
            counters_node_[node]++;
            counters_node_action_[node][a]++;
        }
                                                                                dprint1("**************************************");
    }
    return pickAction(root, 0.0);
}

}

