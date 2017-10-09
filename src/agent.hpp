#ifndef AGENT_HPP_
#define AGENT_HPP_

#include <node.hpp>
#include <test.hpp>

/**
 * @brief Parameters of the policy
 *
 * This class is a parameters container.
 */
struct policy_parameters {
    unsigned budget; ///< Algorithm budget (number of expanded nodes)
    unsigned horizon; ///< Algorithm horizon for the default policy
    unsigned trials_count; ///< Trial count for the expansion in {0,budget-1}
    double uct_cst; ///< UCT constant factor
    double discount_factor; ///< Discount factor for the MDP
    bool reuse; ///< Set to true if open loop
    std::vector<int> action_space; ///< Action space used by the policy
    node root_node; ///< Root node of the tree

    /**
     * @brief Constructor
     *
     * Standard constructor. The root node constructor is called with the initial state
     * defined in the 'main.cpp' file (see parameters definition).
     * Edit: 22/09/2017
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

/**
 * @brief Model of the environment
 *
 * Class including the methods able to simulate the states transitions of the environment
 * and the reward function. The accuracy of the model is set with the simulation parameters.
 */
struct model {
    double model_track_length; ///< Model length of the track (half of the length)
    double model_stddev; ///< Model noise standard deviation
    double model_failure_probability; ///< Probability with chich the oposite action effect is applied in the model (randomness of the transition function)
    unsigned nb_calls; ///< Tracked number of calls to the model

    /** @brief Constructor */
    model(
        double _model_track_length,
        double _model_stddev,
        double _model_failure_probability) :
        model_track_length(_model_track_length),
        model_stddev(_model_stddev),
        model_failure_probability(_model_failure_probability)
    {
        nb_calls = 0;
    }

    /**
     * @brief Transition model
     *
     * Simulate a state transition wrt the models parameters.
     * @param {const double &} s; state
     * @param {const int &} a; action
     * @return Return the resulting state
     */
    double transition_model(const double &s, const int &a) {
        nb_calls++;
        double noise = normal_d(0.,model_stddev);
        double action_effect = (double) a;
        if(is_less_than(uniform_d(0.,1.),model_failure_probability)) {
            action_effect *= (-1.);
        }
        return s + action_effect + noise;
    }

    /**
     * @brief Reward model
     *
     * Reward model of the transition (s,a,s_p)
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
     * @brief Terminal state test
     *
     * Test if the state is terminal.
     * @param {const double &} s; tested state
     * @return Return 'true' if terminal
     */
    bool is_terminal(const double &s) {
        return !is_less_than(std::abs(s),model_track_length);
    }
};

/**
 * @brief Agent struct
 *
 * Agent including its policy, model of the environment and parameters.
 */
struct agent {
    double s; ///< Current state: value on the track.
    int a; ///< Current action in the action space defined by the parameters.
    policy_parameters p; ///< Policy parameters
    model m; ///< Model of the environment

    /** @brief Constructor */
    agent(double _s, policy_parameters _p, model _m) : s(_s), p(_p), m(_m) {
        a = 0;
    }

    /** \brief Get the number of calls */
    unsigned get_nb_calls() {return m.nb_calls;}

    /**
     * @brief UCT child
     *
     * UCT selection method for the tree policy.
     * @param {node &} v; parent node
     * @return Return the selected child according to the UCT formula
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
     * @brief Expansion method
     *
     * Expand the node i.e. create a new leaf node.
     * @return Return a pointer to the created leaf node
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
     * @brief State sampling
     *
     * Sample a new state w.r.t. to the incoming action and the parents state and add it to
     * the node.
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
     * @brief Terminal node test
     *
     * A node is considered terminal if all of its states are terminal states, however,
     * due to the randomness of the transition function, another state will be sampled in
     * the tree policy method so that the decision criterion becomes more reliable.
     * If the node is root, only the labelling state is tested.
     * @param {node &} v; the tested node
     * @return Return 'true' if the node is considered terminal
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
     * @brief Tree policy
     *
     * Apply the tree policy. During the descent, store the sampled leaf states into the
     * nodes parameters. This is a recursive method.
     * @return Return a pointer to the created leaf node or to the current node if terminal
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
     * @brief Default policy
     *
     * Compute the total return by running an episode with the default policy which is
     * random. The simulation starts from the last sampled state of the
     * input node.This is specific to the current implementation where the reward only
     * depends on the state of the agent (edit 22/09/2017).
     * @param {node *} ptr; pointer to the input node
     */
    double default_policy(node * ptr) {
        if(is_node_terminal(*ptr)) {
            double s = ptr->get_last_sampled_state();
            return m.reward_model(s,0,0.);
        }
        double total_return = 0.;
        double s = ptr->get_last_sampled_state();
        //int a = rand_element(p.action_space);
        int a = ((int)sign(s));
        for(unsigned t=0; t<p.horizon; ++t) {
            double s_p = m.transition_model(s,a);
            double r = m.reward_model(s,a,s_p);
            total_return += pow(p.discount_factor,(double)t) * r;
            if(m.is_terminal(s)) { // Termination criterion
                break;
            }
            s = s_p;
            //a = rand_element(p.action_space);
            a = ((int)sign(s));
        }
        return total_return;
//        // Previous method without terminal case:
//        double total_return = 0.;
//        double s = ptr->get_last_sampled_state();
//        int a = rand_element(p.action_space);
//        for(unsigned t=0; t<p.horizon; ++t) {
//            double s_p = m.transition_model(s,a);
//            double r = m.reward_model(s,a,s_p);
//            total_return += pow(p.discount_factor,(double)t) * r;
//            s = s_p;
//            a = rand_element(p.action_space);
//        }
//        return total_return;
    }

    /**
     * @brief Backup method
     *
     * Increment all the visited nodes visits counters and update their values w.r.t. the
     * given discounted return. This method is recursive.
     * @param {double &} total_return; return to be backed up, iteratively discounted
     * @param {node *} ptr; pointer to the node, first the leaf node, then to the parents
     * (recursive method)
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
     * @brief Argmax of the score
     *
     * Get the indice of the child achieving the best score.
     * @param {const node &} v; root node of the tree
     * @return Return the indice of the child achieving the best score.
     */
    unsigned argmax_score(const node &v) {
        std::vector<double> values;
        for(auto &ch: v.children) {
            values.push_back(ch.get_value());
        }
        return argmax(values);
    }

    /**
     * @brief Get the recommended action at a certain node
     *
     * This is the policy decision after the tree construction (recommended action). It gets
     * the greedy action wrt the values of the subsequent nodes.
     * @param {const node &} v; root node of the tree
     * @return Return the action with the best score (leading to the child with the higher
     * value).
     */
    int get_recommended_action(node &v) {
        //return v.get_child_at(argmax_score(v))->get_incoming_action();
        return v.get_action_at(argmax_score(v));
    }

    /**
     * @brief Plain keeping criterion
     *
     * The decision criterion of keeping the tree or not. 'Keeping' the tree means moving the
     * root to its 'best' child (edit 22/09/2017).
     * @param {const double &} s; the current state of the agent
     * @return Return 'true' if the sub-tree is kept.
     */
    bool plain_keeping_criterion(const double &s) {
        (void) s;
        if(!p.root_node.is_fully_expanded()) {
            return false;
        }
        node * ptr = p.root_node.get_child_at(argmax_score(p.root_node));
        return ptr->is_fully_expanded(); // naive implementation: keep the tree if it is built
    }

    /**
     * @brief Build UCT tree
     *
     * Build a tree starting from the root attribute of the parameters using the
     * vanilla UCT algorithm. This is a 'void' method, the tree is kept in memory.
     * @param {const double &} s; current state of the agent
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
     * @brief Open Loop UCT
     *
     * Open Loop UCT policy which has the possibility to reuse a sub-tree.
     * @param {const double &} s; current state of the agent
     * @return Return the recommended action.
     */
    int oluct(const double &s) {
        if(plain_keeping_criterion(s)) { // keep the subtree and use it
            std::cout << "Move to child nb " << argmax_score(p.root_node); //TRM
            std::cout << " v=" << p.root_node.get_child_at(argmax_score(p.root_node))->get_value();
            p.root_node.move_to_child(argmax_score(p.root_node),s);
            std::cout << " print new children:\n"; //TRM
            for(auto &ch : p.root_node.children) { //TRM
                print_node_bis(ch);
            }
        } else { // build or rebuild the subtree
            build_uct_tree(s);
            std::cout << "### Build plan: "; //TRM
            print_best_plan(p.root_node); //TRM
            std::cout << std::endl; //TRM
            print_three_layers(p.root_node); //TRM
        }
        return get_recommended_action(p.root_node);
    }

    /**
     * @brief Vanilla UCT
     * @param {const double &} s; current state of the agent
     * @return Return the recommended action.
     */
    int vanilla_uct(const double &s) {
        build_uct_tree(s);
        return get_recommended_action(p.root_node);
    }

    /**
     * @brief Print best plan
     *
     * Print the best plan as a standard output wrt the current tree.
     */
    void print_best_plan(node v) {
        if(v.is_fully_expanded()) {
            std::cout << get_recommended_action(v) << " ";
            print_best_plan(v.children.at(argmax_score(v)));
        }
    }

    /**
     * @brief Take an action
     *
     * Take an action based on the current state s and set the action attribute. This is the
     * main method of the agent.The Action must be within 'p.action_space'. The current state
     * and the recommended action are attributes of the agent class; hence stored in the
     * memory.
     */
    void take_action() {
        if(p.reuse) {
            a = oluct(s);
        } else {
            a = vanilla_uct(s);
        }
    }
};

#endif // AGENT_HPP_
