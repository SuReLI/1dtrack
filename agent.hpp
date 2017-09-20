#ifndef AGENT_HPP_
#define AGENT_HPP_

#include <node.hpp>

/** @brief Parameters of the policy */
struct policy_parameters {
    /**
     * @brief Attributes
     * @param {unsigned} horizon; tree horizon
     * @param {double} cst; UCT constant factor
     * @param {node} root; root node of the tree
     * @param {bool} reuse; set to true if the policy is able to reuse the tree
     */
    unsigned horizon;
    double cst;
    bool reuse;
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
        double initial_state) :
        horizon(_horizon),
        cst(_cst),
        reuse(_reuse),
        node(initial_state)
    {}
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
     * @brief Expand the node i.e. create a new node
     * @return Pointer to the new node
     */
    node * expand(node &v) {
        int inc_ac = v.get_next_expansion_action();
        double new_state = 0.;
        if(v.is_root) {
            new_state = transition_model(v.state,inc_ac);
        } else {
            new_state = transition_model(v.get_last_sampled_state(),inc_ac);
        }
        v.create_child(new_state,inc_ac);
        return nullptr;
    }

    /** @brief Return the best child according to the UCB */
    node& ucb_child(node &v) {
        //TODO
    }

    /** @brief Sample a new state if not root */
    void sample_new_state(node &v) {
        if(!v.is_root) {
            //v.states.push_back(transition_model(v.parent->states.back(),v.incoming_action));
        }
    }

    /**
     * @brief Apply the tree policy and store the sampled leaf states into the parameters
     * @return A pointer to the created node
     * @note No terminal node case
     */
    node * tree_policy(node &v) {
        if(!v.is_fully_expanded()) { // expand node
            return expand(v);
        } else { // apply tree policy on 'best UCB child'
            node& v_p = ucb_child(v);
            sample_new_state(v_p);
            return tree_policy(v_p);
        }
    }

    /** @brief Compute the return from running an episode with the default policy */
    double default_policy(node * ptr) {
        double state = ptr->states.back();
    }

    /** @brief Backup function */
    void backup(double total_return, node * ptr) {
        //TODO
    }

    int best_action(node v) {
        // check root
        //TODO
    }

    /** @brief UCT policy */
    int uct(double s) {
        node root(s);
        for(unsigned i=0; i<p.horizon; ++i) {
            node *ptr = tree_policy(root);
            //double total_return = default_policy(ptr);
            //backup(total_return,ptr);
        }
        return best_action(root);
    }

    /**
     * @brief Take an action based on the current state s and set the action attribute
     * @note Action must be an integer within {-1, 0, 1}
     */
    void take_action() {
        //a = uct(s); //TRM
        a = 1;
    }
};

#endif
