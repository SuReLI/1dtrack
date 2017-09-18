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
     * @param {bool} reuse; set to true if the policy is able to reuse the tree
     */
    unsigned h;
    double cst;
    double model_stddev;
    bool reuse;

    policy_parameters(
        unsigned _h,
        double _cst,
        double _model_stddev,
        bool _reuse) :
        h(_h),
        cst(_cst),
        model_stddev(_model_stddev),
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
    agent(double _s, policy_parameters _p) : s(_s), p(_p) {}

    /**
     * @brief Expand the node i.e. create a new node
     * @return Pointer to the new node
     */
    node * expand(node &v) {
        (void) v; //TRM
        //TODO
        return nullptr; //TRM
    }

    /** @brief Return the best child according to the UCB */
    node& ucb_child(node &v) {
        //TODO
    }

    double transition_model(double s, unsigned a) {
        std::default_random_engine generator;
        std::normal_distribution<double> distribution(0.,p.model_stddev);
        return s + ((double) a) + distribution(generator);
    }

    /** @brief Sample a new state if not root */
    void sample_new_state(node &v) {
        if(!v.is_root) {
            //v.states.push_back(transition_model(v.parent->states.back(),v.incoming_action));
        }
    }

    /**
     * @brief Apply the tree policy and store the sampled leaf state into the parameters
     * @return A pointer to the created node
     * @note No terminal node case
     */
    node * tree_policy(node &v) {
        if(v.get_nb_children() < NB_ACTIONS) { // expand node
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
     * @note Action must be in {-1, 0, 1}
     */
    void take_action() {
        a = uct(s);
    }
};

#endif
