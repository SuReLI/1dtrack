#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include <utils.hpp>
#include <parameters.hpp>
#include <agent.hpp>
#include <track.hpp>
#include <display.hpp>
#include <test.hpp>
#include <save.hpp>

/**
 * @brief Simulate a single episode
 *
 * Run a single 1D track simulation given its parameters.
 * @warning The values should be saved in the same ordering as in the 'get_saved_values_names'
 * method (edit 22/09/2017).
 * @param {track &} tr; environment
 * @param {agent &} ag; agent
 * @param {const bool &} prnt; if true, print some informations during the simulation
 * @param {const bool &} bckp; if true, save some informations in the end of the simulation
 * @param {std::vector<std::vector<double>>} bckp_vector; backup vector into which each
 * simulation records its backed up values
 */
void simulate_episode(
    track &tr,
    agent &ag,
    const bool &prnt,
    const bool &bckp,
    std::vector<std::vector<double>> &bckp_vector)
{
	std::clock_t c_start = std::clock();
	while(!tr.is_terminal(ag.s)) {
		ag.take_action(); // take action based on current state (attribute of the agent)
		if(prnt) {print(tr,ag);}
		ag.s = tr.transition(ag.s, ag.a); // get next state
	}
    std::clock_t c_end = std::clock();
    if(prnt) {print(tr,ag);}
    if(bckp) { // warning refers to this section
        double time_elapsed_ms = 1000. * (c_end-c_start) / CLOCKS_PER_SEC;
        std::vector<double> simulation_backup = { //
            (double) tr.time,
            time_elapsed_ms,
            (double) ag.get_nb_calls()
        };
        bckp_vector.push_back(simulation_backup);
    }
}

/**
 * @brief Bunch of run with the same parameters
 *
 * Bunch of run with the same parameters.
 * @param {const parameters &} sp; parameters used for all the simulations
 * @param {const unsigned &} nbsim; number of simulations
 * @param {const bool &} prnt; if true, print some informations during the simulation
 * @param {const bool &} bckp; if true, save some informations in the end of the simulation
 * @param {const std::string &} outpth; output saving path is backup
 */
void run_with(
    const parameters &sp,
    const unsigned &nbsim,
    const bool &prnt,
    const bool &bckp,
    const std::string &outpth)
{
    std::vector<std::vector<double>> bckp_vector;
    std::string sep = ",";
    if(bckp) {
        initialize_backup(get_saved_values_names(),outpth,sep);
    }
    for(unsigned i=0; i<nbsim; ++i) {
        std::cout << "Simulation " << i+1 << "/" << nbsim << std::endl;
        track tr(sp.TRACK_LEN, sp.STDDEV, sp.FAILURE_PROBABILITY);
        policy_parameters p(sp.BUDGET, sp.HORIZON, sp.UCT_CST, sp.DISCOUNT_FACTOR, sp.EPSILON, sp.REUSE, sp.ACTION_SPACE, sp.INIT_S);
        model m(sp.MODEL_TRACK_LEN, sp.MODEL_STDDEV, sp.MODEL_FAILURE_PROBABILITY);
        agent ag(sp.INIT_S,p,m);

        simulate_episode(tr,ag,prnt,bckp,bckp_vector);
    }
    if(bckp) {
        save_matrix(bckp_vector,outpth,sep,std::ofstream::app);
    }
}

/**
 * @brief Bunch of run with different parameters
 *
 * Run a bunch of run with different parameters. The latter are set in this function.
 * @param {const unsigned &} nbsim; number of simulations
 */
void bunch_of_run(unsigned nbsim) {
    /*
    for(unsigned i=0; i<10; ++i) {
        parameters sp;
        sp.FAILURE_PROBABILITY = (double) i * .1;
        sp.MODEL_FAILURE_PROBABILITY = sp.FAILURE_PROBABILITY;
        std::string path = get_backup_path(sp);
        std::cout << path << std::endl;
        run_with(sp,nbsim,false,true,path);
    }
    */
    parameters sp;
    sp.FAILURE_PROBABILITY = .1;
    sp.MODEL_FAILURE_PROBABILITY = sp.FAILURE_PROBABILITY;
    sp.EPSILON = 1.;
    sp.UCT_CST = 10.;
    std::string path = get_backup_path(sp);
    std::cout << path << std::endl;
    run_with(sp,nbsim,false,true,path);

}

int main() {
    srand(time(NULL));

    bunch_of_run(1000);
}
