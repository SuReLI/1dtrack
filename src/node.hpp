#ifndef NODE_HPP_
#define NODE_HPP_

/**
 * @brief Node class
 *
 * General node class, for root and standard nodes.
 */
struct node {
private :
    bool root; ///< True if the node is root i.e. labeled by a unique state instead of a family of states
    std::vector<double> sampled_outcomes; ///< Value function estimate
    int incoming_action; ///< Action of the parent node that led to this node
    unsigned visits_count; ///< Number of visits during the tree expansion
    double state; ///<Unique labelling state for a root node
    std::vector<double> sampled_states; ///< Sampled states for a standard node
    std::vector<int> local_action_space; ///< Possible actions at this node (bandit arms)

public :
    node *parent; ///< Pointer to the parent node
    std::vector<node> children; ///< Vector of nodes children

    /**
     * @brief Root node constructor
     *
     * Usually the first node to be created. The provided action space is a vector containing
     * all the actions and is shuffled at the nodes creation.
     * @param {std::vector<int>} action_space; copied then shuffled in actions of the node
     * (bandit arms)
     */
    node(double _state, std::vector<int> _local_action_space) : state(_state) {
        root = true;
        local_action_space = _local_action_space;
        shuffle(local_action_space);
        visits_count = 0;
    }

    /**
     * @brief Standard node constructor
     *
     * Used during the expansion of the tree.
     * @param {std::vector<int>} action_space; copied then shuffled in actions of the node
     * (bandit arms)
     */
    node(
        node * _parent,
        int _incoming_action,
        double _new_state,
        std::vector<int> _local_action_space) :
        incoming_action(_incoming_action),
        parent(_parent)
    {
        root = false;
        visits_count = 0;
        sampled_states.push_back(_new_state);
        local_action_space = _local_action_space;
        shuffle(local_action_space);
    }

    /**
     * @brief Clear method
     *
     * Clear the sampled outcomes; the parent; the incoming action; the state/states; the visit count
     * and the children vector of the node. Do not change the value of 'root' attribute,
     * hence the status of the node. Do not clear actions vector, hence the available actions
     * still remain in the same organisation order.
     */
    void clear_node() {
        parent = nullptr;
        incoming_action = 0;
        state = 0.;
        visits_count = 0;
        sampled_outcomes.clear();
        sampled_states.clear();
        children.clear();
    }

    /** @brief Get the number of children */
    unsigned get_nb_children() const {
        return children.size();
    }

    /** @brief Get a pointer to the last created child */
    node * get_last_child() {
        return &children.back();
    }

    /** @brief Get a pointer to a child given its indice */
    node * get_child_at(unsigned indice) {
        return &children.at(indice);
    }

    /** @brief Get the value of the node */
    double get_value() const {
        double value = 0.;
        for(auto &r : sampled_outcomes) {
            value += r;
        }
        return value / ((double) visits_count);
    }

    /** @brief Get the state of the node (root node) */
    double get_state() const {
        assert(root);
        return state;
    }

    /** @brief Get the number of sampled states (non-root node) */
    unsigned get_nb_sampled_states() const {
        assert(!root);
        return sampled_states.size();
    }

    /** @brief Get a copy of the states vector of the node */
    std::vector<double> get_sampled_states() const {
        return sampled_states;
    }

    /** @brief Get a copy of the sampled outcomes of the node */
    std::vector<double> get_sampled_outcomes() const {
        return sampled_outcomes;
    }

    /** @brief Get a copy of the last sampled state among the states family (non-root node) */
    double get_last_sampled_state() const {
        assert(!root);
        return sampled_states.back();
    }

    /**
     * @brief Get state or last sampled state
     *
     * Get either the unique labelling state if node is root, or the last sampled state
     * if non-root.
     */
    double get_state_or_last() const {
        if(is_root()) {
            return get_state();
        } else {
            return get_last_sampled_state();
        }
    }

    /** @brief Get the incoming action of the node (non-root node) */
    int get_incoming_action() const {
        assert(!root);
        return incoming_action;
    }

    /** @brief Get the visits count of the node (non-root node) */
    unsigned get_visits_count() const {
        assert(!root);
        return visits_count;
    }

    /** @brief Get a copy of the actions vector */
    std::vector<int> get_actions() const {
        return local_action_space;
    }

    /** @brief Get one action of the node given its indice in the actions vector */
    int get_action_at(unsigned indice) const {return local_action_space.at(indice);}

    /** @brief Get the next expansion action among the available actions */
    int get_next_expansion_action() const {
        return local_action_space.at(children.size());
    }

    /** @brief Get the number of actions (arms of the bandit) */
    unsigned get_nb_of_actions() const {
        return local_action_space.size();
    }

    /** @brief Is fully expanded @return Return true if the node is fully expanded */
    bool is_fully_expanded() const {
        return (get_nb_children() == get_nb_of_actions());
    }

    /** @brief Is root @return Return true if the node is a root node */
    bool is_root() const {
        return root;
    }

    /**
     * @brief Create a child
     *
     * Create a child based on the incoming action.
     * @param {int} inc_ac; incoming action of the new child
     * @param {double} new_state; first sampled state of the new child
     */
    void create_child(int inc_ac, double new_state) {
        children.emplace_back(node(this,inc_ac,new_state,local_action_space));
    }

    /**
     * @brief Set state
     *
     * Copy the given state to the nodes state. Node should be root.
     * @param {double} s; copied state
     */
    void set_state(double s) {
        assert(root);
        state = s;
    }

    /**
     * @brief Add to state
     *
     * Add a new sampled state to the states. Node should not be root.
     * @param {double} s; added state
     */
    void add_to_sampled_states(double s) {
        assert(!root);
        sampled_states.push_back(s);
    }

    /**
     * @brief Visit count increment
     *
     * Increment the node's visits counter.
     */
    void increment_visits_count() {
        ++visits_count;
    }

    /**
     * @brief Add to value
     *
     * Add a sample to the sampled outcome.
     * Node should not be root.
     * @param {double} r; outcome sample value to be added
     */
    void add_to_value(double r) {
        assert(!root);
        sampled_outcomes.push_back(r);
    }

    /**
     * @brief Move to child
     *
     * The current root node takes the children, states and action vector of one of its
     * children and updates its state.
     * @param {unsigned} indice; indice of the moved child
     * @param {double} new_state; new labelling state
     */
    void move_to_child(unsigned indice, double new_state) {
        std::cout << "move to children with inc ac " << children[indice].get_incoming_action() << std::endl;//TRM
        assert(is_root());
        local_action_space = children[indice].get_actions();
        sampled_states = children[indice].get_sampled_states();
        visits_count = children[indice].get_visits_count();
        sampled_outcomes = children[indice].get_sampled_outcomes();
        auto tmp = std::move(children[indice].children); // Temporary variable to prevent from overwriting
        for(auto &elt : tmp) {
            elt.parent = this;
        }
        children = std::move(tmp);
        state = new_state;
    }
};

#endif // NODE_HPP_
