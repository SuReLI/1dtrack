#ifndef AGENT_HPP_
#define AGENT_HPP_

#include <node.hpp>

/** @brief Parameters of the policy */
struct policy_parameters {
    /**
     * @brief Attributes
     * @param {unsigned} h; tree horizon
     * @param {double} cst; UCT constant factor
     * @param {double} model_stddev; model noise standard deviation
     * @param {constexpr double} model_failure_probability; probability with chich the
     * oposite action effect is applied in the model (randomness of the transition function)
     * @param {bool} reuse; set to true if the policy is able to reuse the tree
     */
    unsigned h;
    double cst;
    double model_stddev;
    double model_failure_probability;
    bool reuse;

    policy_parameters(
        unsigned _h,
        double _cst,
        double _model_stddev,
        double _model_failure_probability,
        bool _reuse) :
        h(_h),
        cst(_cst),
        model_stddev(_model_stddev),
        model_failure_probability(_model_failure_probability),
        reuse(_reuse)
    {}
};

/** @brief Agent struct */
struct agent {
    /**
     * @brief Attributes
     * @param {double} s; current state: value on the track
     * @param {unsigned} a; current action in {-1,0,1}
     * @param {policy_parameters} p; policy parameters
     */
    double s;
    int a;
    policy_parameters p;

    /** @brief Constructor */
    agent(double _s, policy_parameters _p) : s(_s), p(_p) {
        a = 0;
    }

    /**
     * @brief Expand the node i.e. create a new node
     * @return Pointer to the new node
     */
    node * expand(node &v) {
        if(v.is_root) {

        } else {

        }
        //v.create_child(state,action);
        return nullptr; //TRM
    }

    /** @brief Return the best child according to the UCB */
    node& ucb_child(node &v) {
        //TODO
    }

    /** @brief Simulate a transition wrt the model parameters */
    double transition_model(const double &s, const int &a) {
        double noise = normal_d(0.,p.model_stddev);
        double action_effect = (double) a;
        if(is_less_than(uniform_d(0.,1.),p.model_failure_probability)) {
            action_effect *= (-1.);
        }
        return s + action_effect + noise;
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

    int best_action(root_node v) {
        //TODO
    }

    /** @brief UCT policy */
    int uct(double s) {
        root_node root(s);
        for(unsigned i=0; i<p.h; ++i) {
            //node *ptr = tree_policy(root);
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
