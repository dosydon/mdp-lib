#include "../../include/solvers/THTSSolver.h"

#include <limits>

using namespace std;

namespace mlsolvers
{
////////////////////////////
// ChanceNode
///////////////////////////
ChanceNode::ChanceNode(mlcore::Action* action,
                       int depth,
                       DecisionNode* parent) {
    assert(parent != nullptr);
    this->parent_ = parent;
    this->depth_ = depth;
    this->initialize();
    action_ = action;
    action_value_ = 0.0;
}

void ChanceNode::backup(THTSSolver* solver, double cumulative_value) {
    this->increaseBackupCounter();
    if (this->depth_ == solver->max_depth_) {
        action_value_ =
            static_cast<DecisionNode*>(this->parent_)->state_->cost();
    } else {
        // TODO: Add other backup functions
        // Monte-Carlo backup
        action_value_ += (cumulative_value - action_value_) / backup_counter_;
    }
}

void ChanceNode::updateSuccessorIndexMap(mlcore::State* state) {
    state_successor_index_map_[state] = (explicated_successors_.size() - 1);
                                                                                dprint2("      successor-added", state_successor_index_map_.count(state));
}

DecisionNode* ChanceNode::getDecisionNodeForState(mlcore::State* state) {

                                                                                dprint3("      successor-seen", state_successor_index_map_.count(state), state_successor_index_map_.size());
    if (state_successor_index_map_.count(state)) {
        // Successor state has already been explicated.
        int successor_node_index = state_successor_index_map_[state];
        assert(successor_node_index < explicated_successors_.size());
        return explicated_successors_[successor_node_index];
    } else {
        // Successor state has never been explicated, add a new node.
        explicated_successors_.push_back(
            new DecisionNode(state, this->depth_ + 1, this));
        updateSuccessorIndexMap(state);
        return explicated_successors_.back();
    }
}

double ChanceNode::visit(THTSSolver* solver, mlcore::Problem* problem) {
                                                                                dprint3(debug_pad(this->depth_), "visit-chance", this);
    this->increaseSelectionCounter();
    mlcore::State* s = static_cast<DecisionNode*>(this->parent_)->state_;
    double cumulative_value = problem->cost(s, action_);
                                                                                dprint3(debug_pad(this->depth_), "  c. value", cumulative_value);
    if (solver->continueTrial() && this->depth_ < solver->max_depth_) {
        mlcore::State* s = solver->selectOutcome(this);
                                                                                dprint3(debug_pad(this->depth_), "    outcome", s);
        DecisionNode* node = getDecisionNodeForState(s);
                                                                                dprint3(debug_pad(this->depth_), "    going-to-successor", node->state_);
        cumulative_value += node->visit(solver, problem);
                                                                                dprint2(debug_pad(this->depth_), "    backup");
        this->backup(solver, cumulative_value);
                                                                                dprint2(debug_pad(this->depth_), "    backup-done");
    }
    return cumulative_value;
}

////////////////////////////
// DecisionNode
///////////////////////////
DecisionNode::DecisionNode(mlcore::State* state,
                           int depth,
                           ChanceNode* parent) {
    if (parent == nullptr)
        assert(depth == 0);
    this->parent_ = parent;
    this->depth_ = depth;
    this->initialize();
    state_ = state;
    state_value_ = 0.0;
}

ChanceNode* DecisionNode::getChanceNodeForAction(mlcore::Action* action) {
    // For decision nodes, all successor chance nodes must be explicated.
    assert(action_chance_node_index_map_.count(action));
    int chance_node_index = action_chance_node_index_map_[action];
    assert(chance_node_index < successors_.size());
    return successors_[chance_node_index];
}

void DecisionNode::updateSuccessorIndexMap(mlcore::Action* action) {
    action_chance_node_index_map_[action] = (successors_.size() - 1);
}

void DecisionNode::backup(THTSSolver* solver, double cumulative_value) {
    this->increaseBackupCounter();
    state_value_ = std::numeric_limits<double>::max();
    for (ChanceNode* successor : successors_) {
        if (successor->action_value_ < state_value_) {
            state_value_ = successor->action_value_;
        }
    }
}

double DecisionNode::visit(THTSSolver* solver, mlcore::Problem* problem) {
                                                                                dprint4(debug_pad(this->depth_), "visit-dec", this, successors_.size());
    if (successors_.empty()) {   // not expanded yet
        for (mlcore::Action* action : problem->actions()) {
            if (!problem->applicable(this->state_, action))
                continue;
            successors_.push_back(new ChanceNode(action, this->depth_, this));
            updateSuccessorIndexMap(action);
            // TODO: initialize the decision node
        }
    }
    this->increaseSelectionCounter();
    mlcore::Action* a = solver->selectAction(this);
                                                                                dprint3(debug_pad(this->depth_), "  select-action", a);

    ChanceNode* chance_node = getChanceNodeForAction(a);
    double cumulative_value = chance_node->visit(solver, problem);
                                                                                dprint3(debug_pad(this->depth_), "  value", cumulative_value);
    this->backup(solver, cumulative_value);

                                                                                dprint2(debug_pad(this->depth_), "done-visit-dec");
    return cumulative_value;
}

////////////////////////////
// THTSSolver
///////////////////////////
// TODO: This needs to be adjusted so that old values can be reused during
// re-planning
mlcore::Action* THTSSolver::solve(mlcore::State* s0) {
                                                                                dprint2("solve", num_trials_);
    DecisionNode* root = new DecisionNode(s0, 0, nullptr);
    for (int i = 0; i < num_trials_; i++) {
        root->visit(this, problem_);
    }
    return selectAction(root);
                                                                                dprint1("done");
}

bool THTSSolver::continueTrial() {
    if (num_nodes_expanded_trial_ == max_nodes_expanded_per_trial_)
        return false;
    return true;
}

mlcore::Action* THTSSolver::selectAction(DecisionNode* node) {
                                                                                dprint1("  select-action");
    double q_min = std::numeric_limits<double>::max();
    double q_max = -q_min;
    for (ChanceNode* action_node : node->successors()) {
        q_max = std::max(action_node->action_value_ , q_max);
        q_min = std::min(action_node->action_value_, q_min);
    }
    double q_diff = q_max - q_min;
                                                                                dprint4("    q-values", q_max, q_min, q_diff);
    // UCB1 selection
    std::vector<ChanceNode*> best_action_nodes;
    double best_value = std::numeric_limits<double>::max();
    for (ChanceNode* action_node : node->successors()) {
        if (action_node->selection_counter_ == 0) {
            return action_node->action_;
        }
        double q_normalized = q_diff == 0 ?
            1 : (action_node->action_value_ - q_min) / (q_diff);
        double value_ucb1 = sqrt(2 * log(node->selection_counter_)
                                    / action_node->selection_counter_);
                                                                                dprint4("      action, q-normalized, value_ucb1:", action_node->action_, q_normalized, value_ucb1);
        double q_ucb1 = q_normalized - value_ucb1;
        if (q_ucb1 < best_value) {
            best_value = q_ucb1;
            best_action_nodes.clear();
        }
        if (q_ucb1 == best_value) {
            best_action_nodes.push_back(action_node);
        }
    }
    return best_action_nodes[rand() % best_action_nodes.size()]->action_;
}

mlcore::State* THTSSolver::selectOutcome(ChanceNode* node) {
    mlcore::State* state = static_cast<DecisionNode*>(node->parent_)->state_;
    return randomSuccessor(problem_, state, node->action_);
}

} // namespace mlsolvers

