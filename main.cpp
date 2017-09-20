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

#include <test.hpp> // only for tests

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

int main() {
    srand(time(NULL));
    //node_test();

    track tr(TRACK_LEN,STDDEV,FAILURE_PROBABILITY);
    policy_parameters p(HORIZON,CST,REUSE,INIT_S);
    model m(MODEL_STDDEV,MODEL_FAILURE_PROBABILITY);
    agent ag(INIT_S,p,m);

    run(tr,ag,true);
}
