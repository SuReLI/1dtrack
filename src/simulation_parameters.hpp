#ifndef SIMULATION_PARAMETERS_HPP_
#define SIMULATION_PARAMETERS_HPP_

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
     * @brief Simulation parameters default constructor
     *
     * The parameters are set to the values defined in this constructor. In order to change
     * them, one should modify this 'cpp' file or use the other constructor.
     */
    simulation_parameters() {
        TRACK_LEN = 2.;
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

#endif // SIMULATION_PARAMETERS_HPP_
