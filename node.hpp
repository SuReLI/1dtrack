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
     */
    bool is_root;
    std::vector<int> actions;

    /**
     * @brief Constructor
     * @note The actions sampled during node expansion are ordered here
     */
    base_node (bool _is_root) : is_root(_is_root) {
        actions = std::vector<int>{-1,0,1};
        std::random_shuffle(actions.begin(),actions.end());
    }

    /** @brief Display basic informations for debug */
    void display_a() {
        std::cout << "Root: " << is_root << " ";
        std::cout << "Actions: ";
        for(auto & elt : actions) {
            std::cout << elt << " ";
        }
        std::cout << std::endl;
    }
};

/** @brief Node structure */
struct node : public base_node {
    /**
     * @brief Attributes
     * @param {double} value; value estimate
     * @param {base_node *} parent; pointer to the parent node
     * @param {int} incoming_action; action of the parent node that leaded to this node
     * @param {std::vector<double>} states; sampled states
     * @param {std::vector<node>} children; vector of children
     */
    double value;
    base_node *parent;
    int incoming_action;
    std::vector<double> states;
    std::vector<node> children;

    /** @brief Constructor */
    node(
        base_node * _parent,
        int _incoming_action) :
        base_node(false),
        parent(_parent),
        incoming_action(_incoming_action)
    {
        value = 0.;
    }

    /** @brief Get the number of children */
    unsigned get_nb_children() const {
        return children.size();
    }

    /** @brief Return true if the node is fully expanded */
    bool is_fully_expanded() {
        return (get_nb_children() == NB_ACTIONS);
    }

    /** @brief Create a child based on incoming action */
    void create_child(double incoming_action) {
        children.emplace_back(node(this,incoming_action));
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
        std::cout << "Children: " << get_nb_children() << "\n";
    }

    void display() {
        display_a();
        display_b();
        std::cout << std::endl;
    }
};

/**
 * @brief Root node structure
 * @note Different from a standard node since labeled by a unique state
 */
struct root_node : public base_node {
    /**
     * @brief Attributes
     * @param {double} state;
     * @param {std::vector<node>} children; vector of children
     */
    double state;
    std::vector<node> children;

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
        base_node(true), state(_state)
    {
        actions = ptr->actions;
        children = ptr->children;
        for(auto &elt : children) {
            elt.parent = this;
        }
    }

    /** @brief Get the number of children */
    unsigned get_nb_children() const {
        return children.size();
    }

    /** @brief Return true if the node is fully expanded */
    bool is_fully_expanded() {
        return (get_nb_children() == NB_ACTIONS);
    }

    /** @brief Create a child based on incoming action */
    void create_child(double incoming_action) {
        children.emplace_back(node(this,incoming_action));
    }

    /** @brief Display basic informations for debug */
    void display_b() {
        std::cout << "Adress: " << this << " ";
        std::cout << "State: " << state << " ";
        std::cout << "Children: " << get_nb_children() << "\n";
    }

    void display() {
        display_a();
        display_b();
        std::cout << std::endl;
    }
};

#endif
