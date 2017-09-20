/**
 * @brief Simple unit tests
 * @todo Add assertions
 */

void node_test() {
    std::cout << "#1 ##### Root node v + its 3 children\n";

    node v(0.01);
    //std::unique_ptr<node> v(new node(0.01));
    //node *v = new node(0.01);

    v.create_child(v.get_next_expansion_action(),1.06);
    v.create_child(v.get_next_expansion_action(),2.06);
    v.create_child(v.get_next_expansion_action(),3.06);
    v.display();
    v.children.at(0).display();
    v.children.at(1).display();
    v.children.at(2).display();

    std::cout << "#2 ##### Add child to last v child + child to this child, print them:\n";
    node * ptr = v.get_last_child();
    ptr->create_child(ptr->get_next_expansion_action(),4.06);
    node * ptrbis = ptr->get_last_child();
    ptrbis->create_child(ptrbis->get_next_expansion_action(),5.06);
    std::cout << "# v child\n";
    ptr->display();
    std::cout << "# v child child\n";
    ptr->get_last_child()->display();
    std::cout << "# v child child child\n";
    ptrbis->get_last_child()->display();

    std::cout << "#3 ##### Last child of v as a root, print tree:\n";
    v.absorb_child(2,3.33);

    v.display();
    v.get_last_child()->display();
    v.get_last_child()->get_last_child()->display();
}
