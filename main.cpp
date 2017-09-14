#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>
#include <random>
#include <utils.hpp>
#include <agent.hpp>
#include <track.hpp>

/**
 * @brief Simulation parameters
 * @param {constexpr double} TRACK_LEN; track length
 * @param {constexpr double} STDDEV; environment noise standard deviation
 * @param {constexpr double} INIT_S; initial state
 */
constexpr double TRACK_LEN = 10.;
constexpr double STDDEV = 1.;
constexpr double INIT_S = 0.;

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

int main() {
    srand(time(NULL));

	track tr(TRACK_LEN,STDDEV);
	agent ag(INIT_S);

	run(tr,ag);
}
