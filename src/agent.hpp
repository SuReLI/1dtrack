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
     * @param {node} root_node; root node of the tree
     * @param {bool} reuse; set to true if the policy is able to reuse the tree
     */
    unsigned budget;
    unsigned horizon;
    unsigned trials_count;
    double uct_cst;
    double discount_factor;
    bool reuse;
    std::vector<int> action_space;
    node root_node;

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
        root_node(initial_state,action_space)
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
     * @return Return the resulting state
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
     * @return Return the resulting reward
     */
    double reward_model(const double &s, const int &a, const double &s_p) {
        (void) a; (void) s_p; //default
        return is_less_than(std::abs(s),model_track_length) ? 0. : 1.;
    }

    /**
     * @brief Test if the state is terminal
     * @param {const double &} s; tested state
     * @return Return 'true' if terminal
     */
    bool is_terminal(const double &s) {
        return !is_less_than(std::abs(s),model_track_length);
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
            assert(elt.get_visits_count() != 0);
            assert(p.trials_count > 0);
            uct_scores.emplace_back(
                elt.get_value() + 2 * p.uct_cst *
                sqrt(log((double) p.trials_count)/ ((double) elt.get_visits_count()))
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
        if(v.is_root()) {
            nodes_state = v.get_state();
        } else {
            nodes_state = v.get_last_sampled_state();
        }
        double new_state = m.transition_model(nodes_state,nodes_action);
        v.create_child(nodes_action,new_state);
        return v.get_last_child();
    }

    /**
     * @brief Sample a new state and add it to the given node
     * @param {node *} v; pointer to the node
     */
    void sample_new_state(node * v) {
        assert(!v->is_root());
        int a = v->get_incoming_action();
        double s = 0.;
        if((v->parent)->is_root()) {
            s = (v->parent)->get_state();
        } else {
            s = (v->parent)->get_last_sampled_state();
        }
        v->add_to_states(m.transition_model(s,a));
    }

    /**
     * @brief Check if a node is terminal.
     *
     * A node is considered terminal if all of its states are terminal states, however,
     * due to the randomness of the transition function, another state will be sampled in
     * the tree policy method so that the decision criterion becomes more reliable.
     * If the node is root, only the labelling state is tested.
     * @param {node &} v; the tested node
     * @return true if the node is considered terminal
     */
    bool is_node_terminal(node &v) {
        if(v.is_root()) {
            return m.is_terminal(v.get_state());
        } else {
            for(auto &elt: v.get_states()) {
                if(!m.is_terminal(elt)) {
                    return false;
                }
            }
            return true;
        }
    }

    /**
     * @brief Apply the tree policy
     *
     * During the descent, store the sampled leaf states into the nodes parameters.
     * Recursive function.
     * @return A pointer to the created leaf node or to the current node if terminal
     */
    node * tree_policy(node &v) {
        if(is_node_terminal(v)) {
            sample_new_state(&v);
            return &v;
        } else if(!v.is_fully_expanded()) { // expand node
            return expand(v);
        } else { // apply tree policy on 'best UCB child'
            node * v_p = uct_child(v);
            sample_new_state(v_p);
            return tree_policy(*v_p);
        }
//        // Previous method without terminal case:
//        if(!v.is_fully_expanded()) { // expand node
//            return expand(v);
//        } else { // apply tree policy on 'best UCB child'
//            node * v_p = uct_child(v);
//            sample_new_state(v_p);
//            return tree_policy(*v_p);
//        }
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
        if(!ptr->parent->is_root()) {
            backup(total_return,ptr->parent);
        }
    }

    /**
     * @brief See 'max_score' method
     * @param {const node &} v; root node of the tree
     * @return The indice of the child achieving the best score
     */
    unsigned argmax_score(node &v) {
        assert(v.is_root());
        std::vector<double> values;
        for(auto &elt: v.children) {
            values.push_back(elt.get_value());
        }
        return argmax(values);
    }

    /**
     * @brief Take the policy decision after the tree construction
     * @param {const node &} v; root node of the tree
     * @return The action with the best score (leading to the child with the higher value)
     */
    int max_score(node &v) {
        return v.get_action_at(argmax_score(v));
    }

    /**
     * @brief The decision criterion of keeping the tree or not
     * @note 'Keeping' the tree means moving the root to its 'best' child (edit 22/09/2017)
     * @param {const double &} s; the current state of the agent
     * @return 'true' if tree is kept
     */
    bool keeping_criterion(const double &s) {
        (void) s;
        if(!p.root_node.is_fully_expanded()) {
            return false;
        }
        node * ptr = p.root_node.get_child_at(argmax_score(p.root_node));
        return ptr->is_fully_expanded(); // naive implementation: keep the tree if it is built
    }

    /**
     * @brief Build a tree starting from the root attribute of the parameters using the
     * UCT algorithm
     * @param {const double &} s; current state of the agent
     * @note 'void' method, the tree is kept in memory
     */
    void build_uct_tree(const double &s) {
        p.root_node.clear_node();
        p.root_node.set_state(s);
        p.trials_count = 0;
        for(unsigned i=0; i<p.budget; ++i) {
            node *ptr = tree_policy(p.root_node);
            double total_return = default_policy(ptr);
            backup(total_return,ptr);
            p.trials_count += 1;
        }
    }

    /**
     * @brief Experimental UCT policy
     * @param {const double &} s; current state of the agent
     */
    int experimental_uct(const double &s) {
        if(keeping_criterion(s)) { // keep the subtree and use it
            p.root_node.move_to_child(argmax_score(p.root_node),s);
        } else { // build or rebuild the subtree
            print("build/rebuild");
            build_uct_tree(s);
        }
        return max_score(p.root_node);
    }

    /**
     * @brief Vanilla UCT policy
     * @param {const double &} s; current state of the agent
     */
    int vanilla_uct(const double &s) {
        build_uct_tree(s);
        return max_score(p.root_node);
    }

    /**
     * @brief Take an action based on the current state s and set the action attribute
     * @note Action must be within p.action_space
     * @note The current state and the recommended action are attributes of the agent class
     */
    void take_action() {
        if(p.reuse) {
            a = experimental_uct(s);
        } else {
            a = vanilla_uct(s);
        }
    }
};

#endif // AGENT_HPP_
