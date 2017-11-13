#ifndef AGENT_HPP_
#define AGENT_HPP_

#include <node.hpp>
#include <test.hpp>
#include <exceptions.hpp>

/**
 * @brief Parameters of the policy
 *
 * This class is a parameters container.
 */
struct policy_parameters {
    unsigned policy_selector; ///< Policy selector (0: vanilla UCT; 1: plain OLUCT; default: epsilon-optimal policy)
    unsigned budget; ///< Algorithm budget (number of expanded nodes)
    unsigned horizon; ///< Algorithm horizon for the default policy
    unsigned expd_counter; ///< Counter of the number of expanded nodes
    double uct_cst; ///< UCT constant factor
    double discount_factor; ///< Discount factor for the MDP
    double epsilon; ///< Epsilon for the epsilon-optimal default policy
    std::vector<int> action_space; ///< Action space used by the policy
    std::vector<bool> decision_criteria_selector; ///< Decision criterion selector
    node root_node; ///< Root node of the tree

    /**
     * @brief Constructor
     *
     * Standard constructor. The root node constructor is called with the initial state
     * defined in the 'main.cpp' file (see parameters definition).
     * Edit: 22/09/2017
     * @deprecated
     */
    policy_parameters(
        unsigned _policy_selector,
        unsigned _budget,
        unsigned _horizon,
        double _uct_cst,
        double _discount_factor,
        double _epsilon,
        std::vector<int> _action_space,
        double initial_state) :
        policy_selector(_policy_selector),
        budget(_budget),
        horizon(_horizon),
        uct_cst(_uct_cst),
        discount_factor(_discount_factor),
        epsilon(_epsilon),
        action_space(_action_space),
        root_node(initial_state,action_space)
    {
        expd_counter = 0;
    }

    /**
     * @brief Constructor
     *
     * Constructor via simulation parameters.
     */
    policy_parameters(
        parameters &sp) :
        policy_selector(sp.POLICY_SELECTOR),
        budget(sp.BUDGET),
        horizon(sp.HORIZON),
        uct_cst(sp.UCT_CST),
        discount_factor(sp.DISCOUNT_FACTOR),
        epsilon(sp.EPSILON),
        action_space(sp.ACTION_SPACE),
        root_node(sp.INIT_S,action_space)
    {
        expd_counter = 0;
        sp.parse_decision_criterion(decision_criteria_selector);
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
     * @param {double} s; state
     * @param {int} a; action
     * @return Return the resulting state
     */
    double transition_model(double s, int a) {
        nb_calls++;
        double noise = normal_double(0.,model_stddev);
        double action_effect = (double) a;
        if(is_less_than(uniform_double(0.,1.),model_failure_probability)) {
            action_effect *= (-1.);
        }
        return s + action_effect + noise;
    }

    /**
     * @brief Reward model
     *
     * Reward model of the transition (s,a,s_p)
     * @param {double} s; state
     * @param {int} a; action
     * @param {double} s_p; next state
     * @return Return the resulting reward
     */
    double reward_model(double s, int a, double s_p) {
        (void) a; (void) s_p; //default
        return is_less_than(std::abs(s),model_track_length) ? 0. : 1.;
    }

    /**
     * @brief Terminal state test
     *
     * Test if the state is terminal.
     * @param {double} s; tested state
     * @return Return 'true' if terminal
     */
    bool is_terminal(double s) {
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
            assert(p.expd_counter > 0);
            uct_scores.emplace_back(
                elt.get_value() + 2 * p.uct_cst *
                sqrt(log((double) p.expd_counter)/ ((double) elt.get_visits_count()))
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
        if(is_node_terminal(v)) { // terminal
            sample_new_state(&v);
            return &v;
        } else if(!v.is_fully_expanded()) { // expand node
            return expand(v);
        } else { // apply UCT tree policy
            node * v_p = uct_child(v);
            sample_new_state(v_p);
            return tree_policy(*v_p);
        }
    }

    /**
     * @brief Epsilon optimal policy
     *
     * Compute the optimal action at given state with probability (1-p.epsilon). Otherwise,
     * acte randomly without excluding the optimal action.
     * @param {double} s; input state
     * @return Return the epsilon-optimal action.
     */
    int epsilon_optimal_policy(double s) {
        if(is_less_than(uniform_double(0.,1.),p.epsilon)) { // random action
            return rand_element(p.action_space);
        } else { // optimal action
            int sgn = ((int)sign(s));
            if(!is_less_than(m.model_failure_probability,.5)) {
                sgn *= -1;
            }
            int mgn = (*std::max_element(p.action_space.begin(),p.action_space.end()));
            return sgn * mgn;
        }
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
            return m.reward_model(s,0,s);
        }
        double total_return = 0.;
        double s = ptr->get_last_sampled_state();
        int a = epsilon_optimal_policy(s);
        for(unsigned t=0; t<p.horizon; ++t) {
            double s_p = m.transition_model(s,a);
            double r = m.reward_model(s,a,s_p);
            total_return += pow(p.discount_factor,(double)t) * r;
            if(m.is_terminal(s)) { // Termination criterion
                break;
            }
            s = s_p;
            a = epsilon_optimal_policy(s);
        }
        return total_return;
    }

    /**
     * @brief Backup method
     *
     * Increment all the visited nodes visits counters and update their values w.r.t. the
     * given discounted return.
     * This method is recursive.
     * @param {double &} total_return; return to be backed up, iteratively discounted
     * @param {node *} ptr; pointer to the node, first the leaf node, then to the parents
     */
    void backup(double &total_return, node * ptr) {
        ptr->increment_visits_count();
        ptr->add_to_value(total_return);
        total_return *= p.discount_factor; // apply the discount for the parent node
        /* //TODO: test
        total_return += envt->reward_function( // add the reward of the transition
            ptr->parent->get_last_sampled_state(),
            ptr->get_incoming_action(),
            ptr->get_last_sampled_state()
        );
        */
        if(!ptr->parent->is_root()) {
            backup(total_return,ptr->parent);
        }
    }

    /**
     * @brief Argmax of the score
     *
     * Get the indice of the child achieving the best score.
     * @param {const node &} v; node of the tree
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
     * @return Return the action with the highest score (leading to the child with the higher
     * value).
     */
    int get_recommended_action(const node &v) {
        return v.get_action_at(argmax_score(v));
    }

    /**
     * @brief State multimodality test decision criterion
     *
     * Keep the sub-tree if there is only one state mode, up to a certain precision.
     * Also discard the tree if the current state does not lie in this mode.
     * The test is implemented for the discrete case.
     * @param {double} s; current state of the agent
     * @return Return 'true' if the tree is kept.
     */
    bool state_multimodality_test(double s) {
        std::vector<double> modes_values;
        std::vector<unsigned> modes_counters;
        for(auto &si : p.root_node.get_states()) {
            bool is_new_mode = true;
            for(auto &m : modes_values) {
                if(is_equal_to(si,m)) {
                    is_new_mode = false;
                }
            }
            if(is_new_mode) {
                modes_values.push_back(si);
                modes_counters.push_back(1);
            } else {
                for(unsigned j=0; j<modes_values.size(); ++j) {
                    if(is_equal_to(si,modes_values[j])) {
                        modes_counters.at(j)++;
                    }
                }
            }
        }
        if(modes_values.size() == 1) { // mono-modal
            if(s == modes_values[0]) { // state within the unique mode
                return true;
            } else { // state out of the unique mode
                return false;
            }
        } else { // multi-modal
            std::vector<double> modes_ratio;
            for(auto &elt: modes_counters) { // build modes ratio vector
                modes_ratio.push_back(((double) elt) / ((double)p.root_node.get_states().size()));
            }
            unsigned state_mode_indice = 0;
            for(unsigned j=0; j<modes_values.size(); ++j) {
                if(s == modes_values[j]) {
                    state_mode_indice = j;
                    break;
                }
            }
            double ratio_min = .8; // ratio under which we discard the tree
            if(is_less_than(modes_ratio.at(state_mode_indice),ratio_min)) {
                return false;
            } else {
                return true;
            }
        }
    }

    /**
     * @brief TODO
     */
    bool state_distribution_variance_test() {
        //
    }

    /**
     * @brief TODO
     */
    bool distance_to_state_distribution_mean_test(double s) {
        //
    }

    /**
     * @brief TODO
     */
    bool outcome_distribution_variance_test() {
        //
    }

    /**
     * @brief Build UCT tree
     *
     * Build a tree starting from the root attribute of the parameters using the
     * vanilla UCT algorithm. This is a 'void' method, the tree is kept in memory.
     * @param {double} s; current state of the agent
     */
    void build_uct_tree(double s) {
        p.root_node.clear_node();
        p.root_node.set_state(s);
        p.expd_counter = 0;
        for(unsigned i=0; i<p.budget; ++i) {
            node *ptr = tree_policy(p.root_node);
            double total_return = default_policy(ptr);
            backup(total_return,ptr);
            p.expd_counter += 1;
        }
    }

    /**
     * @brief Switch on decision criterion
     *
     * Select the decision criterion according to the chosen algorithm.
     * The tested tree is the current tree saved in the parameters.
     * @param {double} s; the current state of the agent
     * @return Return 'true' if the sub-tree is kept.
     */
    bool decision_criterion(double s) {
        bool keep_tree = true;
        if(p.decision_criteria_selector[1]) { // state multi-modality
            keep_tree *= state_multimodality_test(s);
        }
        if(p.decision_criteria_selector[2]) { // state distribution variance
            keep_tree *= state_distribution_variance_test();
        }
        if(p.decision_criteria_selector[3]) { // distance to state distribution mean
            keep_tree *= distance_to_state_distribution_mean_test(s);
        }
        if(p.decision_criteria_selector[4]) { // outcome distribution variance
            keep_tree *= outcome_distribution_variance_test();
        }
        return keep_tree;
    }

    /**
     * @brief Open Loop UCT
     *
     * Open Loop UCT policy which has the possibility to reuse a sub-tree.
     * @param {double} s; current state of the agent
     * @return Return the recommended action.
     */
    int oluct(double s) {
        /* deprecated
        int ra = 0;
        if(p.root_node.is_fully_expanded() // necessary condition
        && decision_criterion(s)) { // Open Loop control
            ra = get_recommended_action(p.root_node);
        } else { // Closed Loop control
            build_uct_tree(s);
            ra = get_recommended_action(p.root_node);
        }
        p.root_node.move_to_child(argmax_score(p.root_node),s);
        */
        if(!p.root_node.is_fully_expanded() || !decision_criterion(s)) {
            build_uct_tree(s);
        }
        int ra = get_recommended_action(p.root_node);
        p.root_node.move_to_child(argmax_score(p.root_node),s);
        return ra;
    }

    /**
     * @brief Vanilla UCT
     * @param {double} s; current state of the agent
     * @return Return the recommended action.
     */
    int vanilla_uct(double s) {
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
        switch(p.policy_selector) {
            case 0: { // vanilla UCT
                a = vanilla_uct(s);
                break;
            }
            case 1: { // plain OLUCT
                a = oluct(s);
                break;
            }
            case 2: { // another OLUCT
                a = oluct(s);
                break;
            }
            default : { // epsilon-optimal policy
                a = epsilon_optimal_policy(s);
                break;
            }
        }
    }
};

#endif // AGENT_HPP_
