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
 * @warning The values should be saved in the same order as in the 'get_saved_values_names'
 * method (edit 22/09/2017).
 * @param {track &} tr; environment
 * @param {agent &} ag; agent
 * @param {bool} prnt; if true, print some informations during the simulation
 * @param {bool} bckp; if true, save some informations in the end of the simulation
 * @param {std::vector<std::vector<double>>} bckp_vector; backup vector into which each
 * simulation records its backed up values
 */
void simulate_episode(
    track &tr,
    agent &ag,
    bool prnt,
    bool bckp,
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
    if(bckp) { // warning in comments refers to this section
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
 * @param {parameters &} sp; parameters used for all the simulations
 * @param {unsigned} nbsim; number of simulations
 * @param {bool} prnt; if true, print some informations during the simulation
 * @param {bool} bckp; if true, save some informations in the end of the simulation
 * @param {const std::string &} outpth; output saving path is backup
 */
void run(
    parameters &sp,
    unsigned nbsim,
    bool prnt,
    bool bckp,
    const std::string &outpth = "data/test.csv")
{
    std::vector<std::vector<double>> bckp_vector;
    std::string sep = ",";
    if(bckp) {
        initialize_backup(get_saved_values_names(),outpth,sep);
    }
    for(unsigned i=0; i<nbsim; ++i) {
        //std::cout << "Simulation " << i+1 << "/" << nbsim << std::endl;
        track tr(sp.TRACK_LEN, sp.STDDEV, sp.FAILURE_PROBABILITY);
        policy_parameters p(sp);
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
 * Run a bunch of run with different parameters. The latter are set in this function, you
 * can modify it as you wish.
 * @param {const unsigned &} nbsim; number of simulations
 */
void test(unsigned nbsim) {
    //std::vector<double> fp_range = {.0, .05, .1, .15, .2, .25, .3, .35, .4, .45, .5, .55, .6, .65, .7, .75, .8, .85, .9, .95, 1.};
    std::vector<double> fp_range = {.0, .05, .1, .15, .2, .25, .3, .35, .4, .45, .5};
    /*
    std::vector<unsigned> ps_range = {0, 1, 2};
    for(auto ps : ps_range) {
        for(auto fp : fp_range) {
            parameters sp("main.cfg");
            sp.POLICY_SELECTOR = ps;
            sp.FAILURE_PROBABILITY = fp;
            sp.MODEL_FAILURE_PROBABILITY = fp;
            std::string path = get_backup_path(sp);
            std::cout << "Output: " << path << std::endl;
            run(sp,nbsim,false,true,path);
        }
    }
    */
    std::string root_path = "data/short_";
    parameters sp("main.cfg");
    sp.POLICY_SELECTOR = 1;
    // OLUCT
    //for(unsigned i=0; i<5; ++i) { // for every decision criterion
	for(unsigned i=2; i<3; ++i) { // for return-var decision criterion
        for(unsigned j=0; j<sp.DECISION_CRITERIA.size(); ++j) { // every decision criterion off
            sp.DECISION_CRITERIA[j] = false;
        }
        sp.DECISION_CRITERIA[i] = true; // tested decision criterion on
        for(auto fp : fp_range) { // for every failure probability
            sp.FAILURE_PROBABILITY = fp;
            sp.MODEL_FAILURE_PROBABILITY = fp;
            std::string path = root_path + "plc1_b";
            path += std::to_string(i);
            path += "_fp";
            path += std::to_string((int)(fp*100.));
            path += ".csv";
            std::cout << "Output: " << path << std::endl;
            std::cout << "  fp  : " << fp << std::endl;
            std::cout << "  crit: ";
            for(unsigned j=0; j<sp.DECISION_CRITERIA.size(); ++j) { // every decision criterion off
                std::cout << sp.DECISION_CRITERIA[j] << " ";
            }
            std::cout << std::endl;
            run(sp,nbsim,false,true,path);
        }
    }
    // UCT
	/*
    sp.POLICY_SELECTOR = 0;
    for(auto fp : fp_range) { // for every failure probability
        sp.FAILURE_PROBABILITY = fp;
        sp.MODEL_FAILURE_PROBABILITY = fp;
        std::string path = root_path + "plc0_fp";
        path += std::to_string((int)(fp*100.));
        path += ".csv";
        std::cout << "Output: " << path << std::endl;
        std::cout << "  fp  : " << fp << std::endl;
        run(sp,nbsim,false,true,path);
    }
	*/
}

/**
 * @brief Main function
 *
 * Use first argument to set the number of simulations to perform.
 * Example: ./exe 1000 will produce 1000 simulations performed with the parameters initialized
 * by the user in the function.
 * Default is 100 simulations, set if nothing is specified.
 */
int main(int argc, char* argv[]) {
    try {
        srand(time(NULL));
        switch(argc) {
            case 1: { //default
                std::string cfg_path = "main.cfg";
                std::cout << "Run 1 simulation\n";
                std::cout << "Parameters taken at \'" << cfg_path << "\'\n";
                parameters sp(cfg_path.c_str());
                run(sp,1,true,false);
                break;
            }
            case 2: { // number of simulation given
                std::cout << "Run " << argv[1] << " simulation(s)\n";
                test(atoi(argv[1]));
                break;
            }
            default: {
                throw wrong_nb_input_argument_exception();
            }
        }
    }
    catch(const std::exception &e) {
        std::cerr<<"Error in main(): standard exception caught: "<<e.what()<<std::endl;
    }
    catch(...) {
        std::cerr<<"Error in main(): unknown exception caught"<<std::endl;
    }
}
