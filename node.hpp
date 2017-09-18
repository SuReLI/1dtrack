#ifndef NODE_HPP_
#define NODE_HPP_

/**
 * @brief Structure embedding the shared attributes and members between nodes and
 * root nodes
 */
struct base_node {
    /**
     * @brief Attributes
     * @param {bool} is_root; true if root node i.e. labeled by a particular state;
     * @param {std::vector<int>} actions; possible actions at this node (bandit arms)
     * @param {std::vector<node>} children; vector of the children
     */
    bool is_root;
    std::vector<int> actions;
    std::vector<base_node> children;

    base_node (bool _is_root) : is_root(_is_root) {
        actions = std::vector<int>{-1,0,1};
        std::random_shuffle(actions.begin(),actions.end());
    }

    /** @brief Get the number of children */
    unsigned get_nb_children() const {
        return children.size();
    }

    /** @brief Return true if the node is fully expanded */
    bool is_fully_expanded() {
        return (get_nb_children() == NB_ACTIONS);
    }

    /** @brief Display basic informations for debug */
    void display_a() {
        std::cout << "Root: " << is_root << " ";
        std::cout << "Actions: ";
        for(auto & elt : actions) {
            std::cout << elt << " ";
        }
        std::cout << "Children: " << get_nb_children() << "\n";
    }
};

/** @brief Node structure */
struct node : public base_node {
    /**
     * @brief Attributes
     * @param {double} value; value estimate
     * @param {node *} parent; pointer to the parent node
     * @param {unsigned} incoming_action; action of the parent node that leaded to this node
     * @param {std::vector<double>} states; sampled states
     */
    double value;
    node *parent;
    unsigned incoming_action;
    std::vector<double> states;

    /** @brief Constructor */
    node(
        node * _parent,
        unsigned _incoming_action) :
        base_node(false),
        parent(_parent),
        incoming_action(_incoming_action)
    {
        value = 0.;
    }

    /** @brief Display basic informations for debug */
    void display_b() {
        std::cout << "Adress: " << this << " ";
        std::cout << "Parent: " << parent << " ";
        std::cout << "Val: " << value << " ";
        std::cout << "IncAction: " << incoming_action << " ";
        for(auto & elt : states) {
            std::cout << elt << " ";
        }
        std::cout << "\n";
    }
};

/**
 * @brief Root node structure
 * @note Different from a standard node since labeled by a unique state
 * @deprecated Root node are nodes
 */
struct root_node : public base_node {
    /**
     * @brief Attributes
     * @param {double} state;
     */
    double state;

    /** @brief Constructor for new root node */
    root_node(double _state) :
        base_node(true),
        state(_state)
    { }

    /**
     * @brief Constructor from a specified node
     * @param {node *} ptr; pointer to the node
     * @param {double} state; state of the new root node
     */
    root_node(node *ptr, double _state) :
        base_node(true),
        state(_state)
    {
        actions = ptr->actions;
        children = ptr->children;
    }

    /** @brief Display basic informations for debug */
    void display_b() {
        std::cout << "Adress: " << this << " ";
        std::cout << "State: " << state << " ";
        std::cout << "\n";
    }
};

#endif
