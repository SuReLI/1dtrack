#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <utils.hpp>
#include <agent.hpp>
#include <track.hpp>

/**
 * @brief Simulation parameters
 * @param {constexpr double} TRACK_LEN; track length
 * @param {constexpr double} STDDEV; environment noise standard deviation
 * @param {constexpr double} FAILURE_PROBABILITY; probability with chich the oposite action
 * effect is applied (randomness of the transition function)
 * @param {constexpr double} INIT_S; initial state
 */
constexpr double TRACK_LEN = 10.;
constexpr double STDDEV = 0.;
constexpr double FAILURE_PROBABILITY = .1;
constexpr double INIT_S = 0.;

/**
 * @brief Policy parameters
 * @param {constexpr unsigned} HORIZ; tree horizon
 * @param {constexpr double} CST; UCT constant factor
 * @param {constexpr double} MODEL_STDDEV; model noise standard deviation
 * @param {constexpr double} REUSE; set to true if the policy is able to reuse the tree
 */
constexpr unsigned HORIZ = 2;
constexpr double CST = .7;
constexpr double MODEL_STDDEV = 1.;
constexpr bool REUSE = false;

/** @brief Print some informations */
void print(track &tr, agent &ag) {
    std::cout << "time = " << tr.t;
    std::cout << " state = " << ag.s;
    std::cout << " reward = " << tr.reward(ag.s) << std::endl;
}

/**
 * @brief Run a simulation
 * @param {track &} tr; environment
 * @param {agent &} ag; agent
 */
void run(track &tr, agent &ag) {
	while(!tr.is_terminal(ag.s)) {
		print(tr,ag);
		ag.take_action(); // take action based on current state
		tr.transition(ag.s, ag.a); // get next state
	}
    print(tr,ag);
}

void test() { //TRM

}

int main() {
    srand(time(NULL));

    //test();

    policy_parameters param(HORIZ,CST,MODEL_STDDEV,REUSE);
	track tr(TRACK_LEN,STDDEV,FAILURE_PROBABILITY);
	agent ag(INIT_S,param);

	run(tr,ag);
}
