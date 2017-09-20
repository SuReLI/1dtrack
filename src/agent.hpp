#ifndef AGENT_HPP_
#define AGENT_HPP_

#include <node.hpp>

/** @brief Parameters of the policy */
struct policy_parameters {
    /**
     * @brief Attributes
     * @param {unsigned} horizon; tree horizon
     * @param {unsigned} trials_count; trial count for the expansion in {0,horizon-1}
     * @param {double} cst; UCT constant factor
     * @param {node} root; root node of the tree
     * @param {bool} reuse; set to true if the policy is able to reuse the tree
     */
    unsigned horizon;
    unsigned trials_count;
    double cst;
    bool reuse;
    std::vector<int> action_space;
    node root;

    /**
     * @brief Constructor
     * @note The root node constructor is called with the initial state defined in the
     * 'main.cpp' file (see parameters definition on top)
     */
    policy_parameters(
        unsigned _horizon,
        double _cst,
        bool _reuse,
        std::vector<int> _action_space,
        double initial_state) :
        horizon(_horizon),
        cst(_cst),
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
     * @param {double} model_stddev; model noise standard deviation
     * @param {constexpr double} model_failure_probability; probability with chich the
     * oposite action effect is applied in the model (randomness of the transition function)
     */
    double model_stddev;
    double model_failure_probability;

    model(
        double _model_stddev,
        double _model_failure_probability) :
        model_stddev(_model_stddev),
        model_failure_probability(_model_failure_probability)
    {}

    /**
     * @brief Simulate a transition wrt the model parameters
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
                elt.value + 2 * p.cst *
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

    /** @brief Compute the return from running an episode with the default policy */
    double default_policy(node * ptr) {
        double s = ptr->get_last_sampled_state();
        //TODO
    }

    /**
     * @brief Backup function
     * @note Increment all the visited nodes visits counters and update their values
     * @note Recursive function
     */
    void backup(double total_return, node * ptr) {
        // increment visit counters
        // update the values
        //TODO
    }

    int best_action(node v) {
        // check root
        //TODO
    }

    /** @brief UCT policy */
    int uct(const double &s) {
        p.root.set_state(s);
        p.trials_count = 0;
        for(unsigned i=0; i<p.horizon; ++i) {
            node *ptr = tree_policy(p.root);
            double total_return = default_policy(ptr);
            //backup(total_return,ptr);
            p.trials_count += 1;
        }
        return 1; //TODO
    }

    /**
     * @brief Take an action based on the current state s and set the action attribute
     * @note Action must be an integer within {-1, 0, 1}
     */
    void take_action() {
        a = uct(s);
        //a = 1;
    }
};

#endif
