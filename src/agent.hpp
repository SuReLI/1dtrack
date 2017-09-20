#ifndef AGENT_HPP_
#define AGENT_HPP_

#include <node.hpp>
#include <test.hpp> // only for testing

/** @brief Parameters of the policy */
struct policy_parameters {
    /**
     * @brief Attributes
     * @param {unsigned} budget; algorithm budget (number of expanded nodes)
     * @param {unsigned} horizon; algorithm horizon for the default policy
     * @param {unsigned} trials_count; trial count for the expansion in {0,budget-1}
     * @param {double} uct_cst; UCT constant factor
     * @param {double} discount_factor; discount factor for the MDP
     * @param {node} root; root node of the tree
     * @param {bool} reuse; set to true if the policy is able to reuse the tree
     */
    unsigned budget;
    unsigned horizon;
    unsigned trials_count;
    double uct_cst;
    double discount_factor;
    bool reuse;
    std::vector<int> action_space;
    node root;

    /**
     * @brief Constructor
     * @note The root node constructor is called with the initial state defined in the
     * 'main.cpp' file (see parameters definition on top)
     */
    policy_parameters(
        unsigned _budget,
        unsigned _horizon,
        double _uct_cst,
        double _discount_factor,
        bool _reuse,
        std::vector<int> _action_space,
        double initial_state) :
        budget(_budget),
        horizon(_horizon),
        uct_cst(_uct_cst),
        discount_factor(_discount_factor),
        reuse(_reuse),
        action_space(_action_space),
        root(initial_state,action_space)
    {
        trials_count = 0;
    }
};

/** @brief Model of the environment */
struct model {
    /**
     * @brief Attributes
     * @param {double} model_track_length; model length of the track (half of the length)
     * @param {double} model_stddev; model noise standard deviation
     * @param {constexpr double} model_failure_probability; probability with chich the
     * oposite action effect is applied in the model (randomness of the transition function)
     */
    double model_track_length;
    double model_stddev;
    double model_failure_probability;

    model(
        double _model_track_length,
        double _model_stddev,
        double _model_failure_probability) :
        model_track_length(_model_track_length),
        model_stddev(_model_stddev),
        model_failure_probability(_model_failure_probability)
    {}

    /**
     * @brief Simulate a state transition wrt the model parameters
     * @param {const double &} s; state
     * @param {const int &} a; action
     * @return The resulting state
     */
    double transition_model(const double &s, const int &a) {
        double noise = normal_d(0.,model_stddev);
        double action_effect = (double) a;
        if(is_less_than(uniform_d(0.,1.),model_failure_probability)) {
            action_effect *= (-1.);
        }
        return s + action_effect + noise;
    }

    /**
     * @brief Reward model of the transition (s,a,s_p)
     * @param {const double &} s; state
     * @param {const int &} a; action
     * @param {const double &} s_p; next state
     * @return The resulting reward
     */
    double reward_model(const double &s, const int &a, const double &s_p) {
        (void) a; (void) s_p; //default
        return is_less_than(std::abs(s),model_track_length) ? 0. : 1.;
    }
};

/** @brief Agent struct */
struct agent {
    /**
     * @brief Attributes
     * @param {double} s; current state: value on the track
     * @param {unsigned} a; current action in {-1,0,1}
     * @param {policy_parameters} p; policy parameters
     * @param {model} m; model of the environment
     */
    double s;
    int a;
    policy_parameters p;
    model m;

    /** @brief Constructor */
    agent(double _s, policy_parameters _p, model _m) : s(_s), p(_p), m(_m) {
        a = 0;
    }

    /**
     * @brief UCB selection for the tree policy
     * @param {node &} v; parent node
     * @return The selected child according to the UCT formula
     */
    node * uct_child(node &v) {
        std::vector<double> uct_scores;
        for(auto &elt : v.children) {
            assert(elt.visits_count != 0);
            assert(p.trials_count > 0);
            uct_scores.emplace_back(
                elt.value + 2 * p.uct_cst *
                sqrt(log((double) p.trials_count)/ ((double) elt.visits_count))
            );
        }
        unsigned ind = argmax(uct_scores);
        return &v.children.at(ind);
    }

    /**
     * @brief Expand the node i.e. create a new leaf node
     * @return A pointer to the created leaf node
     */
    node * expand(node &v) {
        int nodes_action = v.get_next_expansion_action();
        double nodes_state = 0.;
        if(v.is_root) {
            nodes_state = v.state;
        } else {
            nodes_state = v.get_last_sampled_state();
        }
        double new_state = m.transition_model(nodes_state,nodes_action);
        v.create_child(nodes_action,new_state);
        return v.get_last_child();
    }

    /**
     * @brief Sample a new state
     * @param {node *} v; pointer to the node
     */
    void sample_new_state(node * v) {
        double s = 0.;
        if((v->parent)->is_root) {
            s = (v->parent)->state;
        } else {
            s = (v->parent)->get_last_sampled_state();
        }
        int a = v->incoming_action;
        double s_p = m.transition_model(s,a);
        v->add_to_states(s_p);
    }

    /**
     * @brief Apply the tree policy
     * @note During the descent, store the sampled leaf states into the nodes parameters
     * @note No terminal node case
     * @note Recursive function
     * @return A pointer to the created leaf node
     */
    node * tree_policy(node &v) {
        if(!v.is_fully_expanded()) { // expand node
            return expand(v);
        } else { // apply tree policy on 'best UCB child'
            node * v_p = uct_child(v);
            sample_new_state(v_p);
            return tree_policy(*v_p);
        }
    }

    /**
     * @brief Compute the total return from running an episode with the default policy
     * @note The simulation starts from the last sampled state of the input node
     * @param {node *} ptr; pointer to the input node
     */
    double default_policy(node * ptr) {
        double total_return = 0.;
        double s = ptr->get_last_sampled_state();
        int a = rand_element(p.action_space);
        for(unsigned t=0; t<p.horizon; ++t) {
            double s_p = m.transition_model(s,a);
            double r = m.reward_model(s,a,s_p);
            total_return += pow(p.discount_factor,(double)t) * r;
            s = s_p;
            a = rand_element(p.action_space);
        }
        return total_return;
    }

    /**
     * @brief Backup function
     * @param {double &} total_return; return to be backed up, iteratively discounted
     * @param {node *} ptr; pointer to the node, first the leaf node, then to the parents
     * @note Increment all the visited nodes visits counters and update their values
     * @note Recursive function
     */
    void backup(double &total_return, node * ptr) {
        ptr->increment_visits_count();
        ptr->add_to_value(total_return);
        total_return *= p.discount_factor; // apply the discount for the parent node
        if(!ptr->parent->is_root) {
            backup(total_return,ptr->parent);
        }
    }

    /**
     * @brief Take the policy decision after the tree construction
     * @param {const node &} v; root node of the tree
     * @return The action with the best score (leading to the child with the higher value)
     */
    int best_action(const node &v) {
        assert(v.is_root);
        std::vector<double> values;
        for(auto &elt: v.children) {
            values.push_back(elt.value);
        }
        unsigned ind = argmax(values);
        return v.actions.at(ind);
    }

    /** @brief UCT policy */
    int uct(const double &s) {
        p.root.clear_node();
        p.root.set_state(s);
        p.trials_count = 0;
        for(unsigned i=0; i<p.budget; ++i) {
            node *ptr = tree_policy(p.root);
            double total_return = default_policy(ptr);
            total_return = 1.;
            backup(total_return,ptr);
            p.trials_count += 1;
        }
        return best_action(p.root);
    }

    /**
     * @brief Take an action based on the current state s and set the action attribute
     * @note Action must be within p.action_space
     * @note The current state and the recommended action are attributes of the agent class
     */
    void take_action() {
        a = uct(s);
        print_node_and_children(p.root);
    }
};

#endif // AGENT_HPP_
