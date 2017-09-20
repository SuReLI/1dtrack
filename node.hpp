#ifndef NODE_HPP_
#define NODE_HPP_

/** @brief General node structure, for root and standard nodes */
struct node {
    /**
     * @brief Attributes
     * @param {bool} is_root; true if root node i.e. labeled by a particular state;
     * @param {double} value; value estimate
     * @param {node *} parent; pointer to the parent node
     * @param {int} incoming_action; action of the parent node that leaded to this node
     * @param {double} state; labelling state for a root node
     * @param {std::vector<double>} states; sampled states for a standard node
     * @param {std::vector<int>} actions; possible actions at this node (bandit arms)
     * @param {std::vector<node>} children; vector of nodes children
     */
    bool is_root;
    double value;
    node *parent;
    int incoming_action;
    double state;
    std::vector<double> states;
    std::vector<int> actions;
    std::vector<node> children;

    /** @brief Node constructor, used during the expansion of the tree */
    node(
        node * _parent,
        int _incoming_action,
        double _new_state) :
        parent(_parent),
        incoming_action(_incoming_action)
    {
        is_root = false;
        value = 0.;
        states.push_back(_new_state);
        actions = std::vector<int>{-1,0,1};
        shuffle(actions);
    }

    /** @brief Root node constructor */
    node(double _state) : state(_state) {
        is_root = true;
        actions = std::vector<int>{-1,0,1};
        shuffle(actions);
    }

    // MEMBERS ///////////////////////////////////////////////////////////////////////////////

    /**
     * @brief Set the nodes attributes as root nodes attributes
     * @param {double} new_state; new labelling state
     */
    void set_as_root(double new_state) {
        is_root = true;
        state = new_state;
        states.clear();
        value = 0.;
        parent = nullptr;
        incoming_action = 0;
    }

    /**
     * @brief Move the content of a child to the current node and reroute children parents
     * @param {unsigned} indice; indice of the moved child
     * @param {double} new_state; new labelling state
     */
    void absorb_child(unsigned indice, double new_state) {
        *this = std::move(children.at(indice));
        set_as_root(new_state);
        for(auto &elt : children) {
            elt.parent = this;
        }
    }

    /** @brief Get the number of children */
    unsigned get_nb_children() const {
        return children.size();
    }

    /** @brief Return true if the node is fully expanded */
    bool is_fully_expanded() const {
        return (get_nb_children() == NB_ACTIONS);
    }

    /**
     * @brief Create a child based on incoming action
     * @param {int} inc_ac; incoming action of the new child
     * @param {double} new_state; first sampled state of the new child
     */
    void create_child(int inc_ac, double new_state) {
        children.emplace_back(node(this,inc_ac,new_state));
    }

    /** @brief Return a pointer to the last created child */
    node * get_last_child() {
        return &children.back();
    }

    /**
     * @brief Return the last sampled state among the states family
     * @note Node should be root
     */
    double get_last_sampled_state() {
        assert(!is_root);
        return states.back();
    }

    /** @brief Return the next expansion actions among the available actions */
    int get_next_expansion_action() {
        return actions.at(children.size());
    }

    /**
     * @brief Copy the input state to the nodes state
     * @note Node should be root
     */
    void set_state(const double &s) {
        assert(is_root);
        state = s;
    }

    // END ///////////////////////////////////////////////////////////////////////////////////

    /** @brief Display basic informations for debug */
    void display() {
        if(is_root) {
            std::cout << "ROOT\nState: " << state << "\n";

            std::cout << "CHECK EMPTY: Parent: " << parent << " ";
            std::cout << "Val: " << value << " ";
            std::cout << "IncAction: " << incoming_action << " ";
            std::cout << "States: ";
            for(auto & elt : states) {
                std::cout << elt << " ";
            }
        } else {
            std::cout << "Parent: " << parent << " ";
            std::cout << "Val: " << value << " ";
            std::cout << "IncAction: " << incoming_action << " ";
            std::cout << "States: ";
            for(auto & elt : states) {
                std::cout << elt << " ";
            }
        }
        std::cout << "\nAdress: " << this << " ";
        std::cout << "Actions: ";
        for(auto & elt : actions) {
            std::cout << elt << " ";
        }
        std::cout << "Children: " << get_nb_children() << "\n\n";
    }
};

#endif
