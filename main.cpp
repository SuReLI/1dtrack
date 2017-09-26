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
#include <agent.hpp>
#include <track.hpp>
#include <display.hpp>
#include <test.hpp>
#include <save.hpp>

/**
 * @brief Simulation parameters
 *
 * Parameters container for a simulation
 */
struct simulation_parameters {
    double TRACK_LEN; ///< Track length (half of the length of the track).
    double STDDEV; ///< Environment noise standard deviation.
    double FAILURE_PROBABILITY; ///< Probability with chich the oposite action effect is applied (randomness of the transition function)
    double INIT_S; ///< Initial state.

    std::vector<int> ACTION_SPACE; ///< Action space used by every nodes (bandit arms).
    unsigned BUDGET; ///< Algorithm budget (number of expanded nodes).
    unsigned HORIZON; ///< Algorithm horizon for the default policy.
    double UCT_CST; ///< UCT constant factor.
    double DISCOUNT_FACTOR; ///< Discount factor for the MDP.
    double MODEL_TRACK_LEN; ///< Model track length (half of the length of the track).
    double MODEL_STDDEV; ///< Model noise standard deviation.
    double MODEL_FAILURE_PROBABILITY; ///< Probability with chich the oposite action effect is applied in the model (randomness of the transition function).
    bool REUSE; ///< Set to true if the policy is able to reuse the tree.

    /**
     * @brief Simulation parameters constructor
     *
     * The parameters are set to the values defined in this constructor. In order to change
     * them, one should modify this 'cpp' file.
     */
    simulation_parameters() {
        TRACK_LEN = 25.;
        STDDEV = 0.;
        FAILURE_PROBABILITY = 0.;
        INIT_S = 0.;

        ACTION_SPACE = std::vector<int>{-1,1};
        BUDGET = 20;
        HORIZON = 50;
        UCT_CST = .7;
        DISCOUNT_FACTOR = .9;
        MODEL_TRACK_LEN = TRACK_LEN;
        MODEL_STDDEV = STDDEV;
        MODEL_FAILURE_PROBABILITY = FAILURE_PROBABILITY;
        REUSE = true;
    }
};

/**
 * @brief Simulate a single episode
 *
 * Run a single 1D track simulation given its parameters.
 * @warning The values should be saved in the same ordering as in the 'get_backup_names'
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
    std::vector<double> simulation_backup;
	while(!tr.is_terminal(ag.s)) {
		ag.take_action(); // take action based on current state
		if(prnt) {print(tr,ag);}
		ag.s = tr.transition(ag.s, ag.a); // get next state
	}
    if(prnt) {print(tr,ag);}
    if(bckp) { // warning refers to this section
        std::clock_t c_end = std::clock();
        double time_elapsed_ms = 1000. * (c_end-c_start) / CLOCKS_PER_SEC;
        simulation_backup.push_back(tr.time);
        simulation_backup.push_back(time_elapsed_ms);
        bckp_vector.push_back(simulation_backup);
    }
}

/**
 * @brief Get the backup names
 *
 * Get a vector containing the names of the saved values during each simulation.
 * @warning The values should be saved in the same ordering as in the 'simulate_episode'
 * method (edit 22/09/2017).
 * @return Return a vector containing each name in order of appearance
 */
std::vector<std::string> get_backup_names() {
    std::vector<std::string> v;
    v.emplace_back("score");
    v.emplace_back("computational_cost");
    return v;
}

/**
 * @brief Run method
 *
 * Basic method initialising the parameters and running one simulation. One should modifiy
 * this method in order to change the parameters
 * @param {const simulation_parameters &} sp; parameters used for all the simulations
 * @param {const unsigned &} nbsim; number of simulations
 * @param {const bool &} prnt; if true, print some informations during the simulation
 * @param {const bool &} bckp; if true, save some informations in the end of the simulation
 * @param {const std::string &} outpth; output saving path is backup
 */
void run(
    const simulation_parameters &sp,
    const unsigned &nbsim,
    const bool &prnt,
    const bool &bckp,
    const std::string &outpth)
{
    std::vector<std::vector<double>> bckp_vector;
    std::string sep = ",";
    if(bckp) {
        initialize_backup(get_backup_names(),outpth,sep);
    }
    for(unsigned i=0; i<nbsim; ++i) {
        //std::cout << "Simulation " << i+1 << "/" << nbsim << std::endl;
        track tr(sp.TRACK_LEN, sp.STDDEV, sp.FAILURE_PROBABILITY);
        policy_parameters p(sp.BUDGET, sp.HORIZON, sp.UCT_CST, sp.DISCOUNT_FACTOR, sp.REUSE, sp.ACTION_SPACE, sp.INIT_S);
        model m(sp.MODEL_TRACK_LEN, sp.MODEL_STDDEV, sp.MODEL_FAILURE_PROBABILITY);
        agent ag(sp.INIT_S,p,m);

        simulate_episode(tr,ag,prnt,bckp,bckp_vector);
    }
    if(bckp) {
        save_matrix(bckp_vector,outpth,sep,std::ofstream::app);
    }
}

/**
 * @brief Bunch of run
 *
 * Run a bunch of run for plotting purpose
 */
void bunch_of_run() {
    unsigned nbsim = 1000;
    for(unsigned i=0; i<10; ++i) {
        simulation_parameters sp;
        sp.FAILURE_PROBABILITY = (double) i * .1;
        sp.MODEL_FAILURE_PROBABILITY = sp.FAILURE_PROBABILITY;
        std::string path = "data/1_25_0_0";
        path += std::to_string(i);
        path += "_20_50_07_09.csv";
        std::cout << path << std::endl;
        run(sp,nbsim,false,true,path);
    }
}

int main() {
    srand(time(NULL));

    bunch_of_run();
    //simulation_parameters sp;
    //run(sp,1e3,false,true,"data/data2.csv");
}
