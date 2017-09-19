#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>
#include <random>
#include <cassert>
#include <algorithm>
#include <memory>
#include <utils.hpp>
#include <agent.hpp>
#include <track.hpp>

/**
 * @brief Simulation parameters:
 * @param {constexpr double} TRACK_LEN; track length
 * @param {constexpr double} STDDEV; environment noise standard deviation
 * @param {constexpr double} FAILURE_PROBABILITY; probability with chich the oposite action
 * effect is applied (randomness of the transition function)
 * @param {constexpr double} INIT_S; initial state
 *
 * @brief Policy parameters:
 * @param {constexpr unsigned} HORIZON; tree horizon
 * @param {constexpr double} CST; UCT constant factor
 * @param {constexpr double} MODEL_STDDEV; model noise standard deviation
 * @param {constexpr double} MODEL_FAILURE_PROBABILITY; probability with chich the oposite
 * action effect is applied in the model (randomness of the transition function)
 * @param {constexpr double} REUSE; set to true if the policy is able to reuse the tree
 */
constexpr double TRACK_LEN = 10.;
constexpr double STDDEV = 0.;
constexpr double FAILURE_PROBABILITY = .1;
constexpr double INIT_S = 0.;

constexpr unsigned HORIZON = 2;
constexpr double CST = .7;
constexpr double MODEL_STDDEV = 1.;
constexpr double MODEL_FAILURE_PROBABILITY = .1;
constexpr bool REUSE = false;

/** @brief Print some informations */
void print(track &tr, agent &ag) {
    std::cout << "time = " << tr.t;
    std::cout << " state = " << ag.s;
    std::cout << " action = " << ag.a;
    std::cout << " reward = " << tr.reward(ag.s) << std::endl;
}

/**
 * @brief Run a simulation
 * @param {track &} tr; environment
 * @param {agent &} ag; agent
 * @param {bool} do_print; if true print some informations
 */
void run(track &tr, agent &ag, bool do_print) {
	while(!tr.is_terminal(ag.s)) {
		if(do_print){print(tr,ag);}
		ag.take_action(); // take action based on current state
		ag.s = tr.transition(ag.s, ag.a); // get next state
	}
    if(do_print){print(tr,ag);}
}

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

int main() {
    srand(time(NULL));

    node_test();

//    policy_parameters param(HORIZON,CST,MODEL_STDDEV,MODEL_FAILURE_PROBABILITY,REUSE);
//	track tr(TRACK_LEN,STDDEV,FAILURE_PROBABILITY);
//	agent ag(INIT_S,param);
//
//	run(tr,ag,true);
}
