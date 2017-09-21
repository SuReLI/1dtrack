#ifndef TEST_HPP_
#define TEST_HPP_

/** @brief print many informations about a node */
void print_node_complete(node &v) {
    if(v.is_root()) {
        std::cout << "ROOT\nState: " << v.get_state() << "\n";

        std::cout << "CHECK EMPTY: Parent: " << v.parent << " ";
        std::cout << "Val: " << v.get_value() << " ";
        std::cout << "IncAction: " << v.get_incoming_action() << " ";
        std::cout << "States: ";
        for(auto & elt : v.get_states()) {
            std::cout << elt << " ";
        }
    } else {
        std::cout << "Parent: " << v.parent << " ";
        std::cout << "Val: " << v.get_value() << " ";
        std::cout << "IncAction: " << v.get_incoming_action() << " ";
        std::cout << "States: ";
        for(auto & elt : v.get_states()) {
            std::cout << elt << " ";
        }
    }
    std::cout << "\nAdress: " << &v << " ";
    std::cout << "Actions: ";
    for(unsigned i=0; i<v.get_nb_of_actions(); ++i) {
        std::cout << v.get_action_at(i) << " ";
    }
    std::cout << "Children: " << v.get_nb_children() << " ";
    std::cout << "Visits: " << v.get_visits_count() << "\n\n";
}

/** @brief print minimum informations about a node */
void print_node(node &v) {
    if(v.is_root()) {
        std::cout << "ROOT: s:" << v.get_state();
    } else {
        std::cout << "s:";
        for(auto &elt : v.get_states()) {
            std::cout << elt << " ";
        }
    }
    std::cout << "  a:";
    for(unsigned i=0; i<v.get_nb_of_actions(); ++i) {
        std::cout << v.get_action_at(i) << " ";
    }
    std::cout << "   v:" << v.get_value() << "\n";
}

/** @brief Print a node and its children */
void print_node_and_children(node &v) {
    print_node(v);
    for(auto &elt : v.children) {
        print_node(elt);
    }
    std::cout << "\n";
}

/**
 * @brief Nodes unit tests
 * @todo Add assertions
 */
void node_test() {
    std::vector<int> action_space = {-1,0,1};

    std::cout << "#1 ##### Root node v + its 3 children\n";

    node v(0.01,action_space);
    //std::unique_ptr<node> v(new node(0.01));
    //node *v = new node(0.01);

    v.create_child(v.get_next_expansion_action(),1.06);
    v.create_child(v.get_next_expansion_action(),2.06);
    v.create_child(v.get_next_expansion_action(),3.06);
    print_node_complete(v);
    print_node_complete(v.children.at(0));
    print_node_complete(v.children.at(1));
    print_node_complete(v.children.at(2));

    std::cout << "#2 ##### Add child to last v child + child to this child, print them:\n";
    node * ptr = v.get_last_child();
    ptr->create_child(ptr->get_next_expansion_action(),4.06);
    node * ptrbis = ptr->get_last_child();
    ptrbis->create_child(ptrbis->get_next_expansion_action(),5.06);
    std::cout << "# v child\n";
    print_node_complete(*ptr);
    std::cout << "# v child child\n";
    print_node_complete(*ptr->get_last_child());
    std::cout << "# v child child child\n";
    print_node_complete(*ptrbis->get_last_child());

    std::cout << "#3 ##### Last child of v as a root, print tree:\n";
    v.absorb_child(2,3.33);

    print_node_complete(v);
    print_node_complete(*v.get_last_child());
    print_node_complete(*v.get_last_child()->get_last_child());
}

#endif // TEST_HPP_
