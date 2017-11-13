#ifndef PARAMETERS_HPP_
#define PARAMETERS_HPP_

#include <libconfig.h++>
#include <exceptions.hpp>

/**
 * @brief Simulation parameters
 *
 * Parameters container for a simulation
 */
struct parameters {
    std::string MAIN_CFG_PATH;
    double TRACK_LEN; ///< Track length (half of the length of the track)
    double STDDEV; ///< Environment noise standard deviation
    double FAILURE_PROBABILITY; ///< Probability with chich the oposite action effect is applied (randomness of the transition function)
    double INIT_S; ///< Initial state
    std::vector<int> ACTION_SPACE; ///< Action space used by every nodes (bandit arms)
    unsigned POLICY_SELECTOR; ///< Policy selector (0: vanilla UCT; 1: plain OLUCT; default: epsilon-optimal policy)
    unsigned BUDGET; ///< Algorithm budget (number of expanded nodes)
    unsigned HORIZON; ///< Algorithm horizon for the default policy
    double UCT_CST; ///< UCT constant factor
    double DISCOUNT_FACTOR; ///< Discount factor for the MDP
    double EPSILON; ///< Epsilon for the epsilon-optimal default policy
    double MODEL_TRACK_LEN; ///< Model track length (half of the length of the track)
    double MODEL_STDDEV; ///< Model noise standard deviation
    double MODEL_FAILURE_PROBABILITY; ///< Probability with chich the oposite action effect is applied in the model (randomness of the transition function)

    /**
     * @brief Simulation parameters 'default' constructor
     *
     * The parameters are set to the values defined in this constructor.
     */
    parameters(
        double track_len = 2.,
        double stddev = 0.,
        double failure_probability = .1,
        double init_s = 0.,
        std::vector<int> action_space = std::vector<int>{-1,1},
        unsigned policy_selector = 0,
        unsigned budget = 20,
        unsigned horizon = 10,
        double uct_cst = .7,
        double discount_factor = 1.,
        double epsilon = 1.,
        double model_track_len = 2.,
        double model_stddev = 0.,
        double model_failure_probability = .1) :
        TRACK_LEN(track_len),
        STDDEV(stddev),
        FAILURE_PROBABILITY(failure_probability),
        INIT_S(init_s),
        ACTION_SPACE(action_space),
        POLICY_SELECTOR(policy_selector),
        BUDGET(budget),
        HORIZON(horizon),
        UCT_CST(uct_cst),
        DISCOUNT_FACTOR(discount_factor),
        EPSILON(epsilon),
        MODEL_TRACK_LEN(model_track_len),
        MODEL_STDDEV(model_stddev),
        MODEL_FAILURE_PROBABILITY(model_failure_probability)
    {}

    /**
     * @brief Simulation parameters 'from-configuration-file' constructor
     *
     * The parameters are taken into the config file whose path is given as an argument.
     * The used library for parsing is libconfig.
     * @param {const char *} cfg_path; path of the configuration file
     */
    parameters(const char *cfg_path) : MAIN_CFG_PATH(cfg_path) {
        libconfig::Config cfg;
        try {
            cfg.readFile(cfg_path);
        }
        catch(const libconfig::ParseException &e) {
            display_libconfig_parse_exception(e);
        }
        unsigned nbac= 0;
        if(cfg.lookupValue("track_len",TRACK_LEN)
        && cfg.lookupValue("stddev",STDDEV)
        && cfg.lookupValue("failure_probability",FAILURE_PROBABILITY)
        && cfg.lookupValue("init_s",INIT_S)
        && cfg.lookupValue("nb_actions",nbac)
        && cfg.lookupValue("policy_selector",POLICY_SELECTOR)
        && cfg.lookupValue("budget",BUDGET)
        && cfg.lookupValue("horizon",HORIZON)
        && cfg.lookupValue("uct_cst",UCT_CST)
        && cfg.lookupValue("discount_factor",DISCOUNT_FACTOR)
        && cfg.lookupValue("epsilon",EPSILON)
        && cfg.lookupValue("model_track_len",MODEL_TRACK_LEN)
        && cfg.lookupValue("model_stddev",MODEL_STDDEV)
        && cfg.lookupValue("model_failure_probability",MODEL_FAILURE_PROBABILITY)) {
            for(unsigned i=0; i<nbac; ++i) { // actions parsing
                std::string name = "a";
                name += std::to_string(i);
                int action = 0;
                if(cfg.lookupValue(name,action)) {
                    ACTION_SPACE.push_back(action);
                } else { // Error in action names syntax
                    throw action_names_configuration_file_exception();
                }
            }
        }
        else { // Error in config file
            throw wrong_syntax_configuration_file_exception();
        }
    }

    /**
     * @brief Display libconfig ParseException
     *
     * @param {const libconfig::ParseException &} e; displayed exception
     */
    void display_libconfig_parse_exception(const libconfig::ParseException &e) {
        std::cerr << "Error in parameters(const char *cfg_path): ParseException ";
        std::cerr << "in file " << e.getFile() << " ";
        std::cerr << "at line " << e.getLine() << ": ";
        std::cerr << e.getError() << std::endl;
    }

    /**
     * @brief Parse decision criterion
     *
     * Parse the decision criteria boolean.
     * @param {std::vector<bool> &} v; vector containing every boolean
     */
    void parse_decision_criterion(std::vector<bool> &v) {
        libconfig::Config cfg;
        try {
            cfg.readFile(MAIN_CFG_PATH.c_str());
        }
        catch(const libconfig::ParseException &e) {
            display_libconfig_parse_exception(e);
        }
        v.reserve(5);
        bool b0, b1, b2, b3, b4;
        if(cfg.lookupValue("b0",b0)
        && cfg.lookupValue("b1",b1)
        && cfg.lookupValue("b2",b2)
        && cfg.lookupValue("b3",b3)
        && cfg.lookupValue("b4",b4)) {
            v.push_back(b0);
            v.push_back(b1);
            v.push_back(b2);
            v.push_back(b3);
            v.push_back(b4);
        } else {
            throw wrong_syntax_configuration_file_exception();
        }
    }
};

#endif // PARAMETERS_HPP_
