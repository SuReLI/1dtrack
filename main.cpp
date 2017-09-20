#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include <utils.hpp>
#include <agent.hpp>
#include <track.hpp>
#include <display.hpp> // only for displaying
#include <test.hpp> // only for testing

/**
 * @brief Run a simulation given its parameters
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

/**
 * @brief Basic method initialising the parameters and running one simulation
 * @note One should modifiy this method in order to change the parameters
 */
void run_1dtrack() {
    /**
     * @brief Step 1: initialisation
     * @brief Simulation parameters:
     * @param {double} TRACK_LEN; track length (half of the length)
     * @param {double} STDDEV; environment noise standard deviation
     * @param {double} FAILURE_PROBABILITY; probability with chich the oposite action
     * effect is applied (randomness of the transition function)
     * @param {double} INIT_S; initial state
     *
     * @brief Policy parameters:
     * @param {unsigned} BUDGET; algorithm budget (number of expanded nodes)
     * @param {unsigned} HORIZON; algorithm horizon for the default policy
     * @param {double} UCT_CST; UCT constant factor
     * @param {double} DISCOUNT_FACTOR; discount factor for the MDP
     * @param {double} MODEL_STDDEV; model noise standard deviation
     * @param {double} MODEL_FAILURE_PROBABILITY; probability with chich the oposite
     * action effect is applied in the model (randomness of the transition function)
     * @param {double} REUSE; set to true if the policy is able to reuse the tree
     * @param {std::vector<int>} ACTION_SPACE; action space used by every nodes (bandit arms)
     */
    double TRACK_LEN = 5.;
    double STDDEV = 0.;
    double FAILURE_PROBABILITY = .1;
    double INIT_S = 0.;

    unsigned BUDGET = 1000;
    unsigned HORIZON = 100;
    double UCT_CST = .7;
    double DISCOUNT_FACTOR = 1.;
    double MODEL_TRACK_LEN = TRACK_LEN;
    double MODEL_STDDEV = 0.01;
    double MODEL_FAILURE_PROBABILITY = 0.;
    bool REUSE = false;
    std::vector<int> ACTION_SPACE = {-1,1};

    track tr(TRACK_LEN,STDDEV,FAILURE_PROBABILITY);
    policy_parameters p(BUDGET,HORIZON,UCT_CST,DISCOUNT_FACTOR,REUSE,ACTION_SPACE,INIT_S);
    model m(MODEL_TRACK_LEN,MODEL_STDDEV,MODEL_FAILURE_PROBABILITY);
    agent ag(INIT_S,p,m);

    /** @brief Step 2: run the simulation */
    run(tr,ag,true);
}

int main() {
    srand(time(NULL));

    run_1dtrack();
}
