#ifndef PARAMETERS_HPP_
#define PARAMETERS_HPP_

/**
 * @brief Simulation parameters
 *
 * Parameters container for a simulation
 */
struct parameters {
    double TRACK_LEN; ///< Track length (half of the length of the track).
    double STDDEV; ///< Environment noise standard deviation.
    double FAILURE_PROBABILITY; ///< Probability with chich the oposite action effect is applied (randomness of the transition function)
    double INIT_S; ///< Initial state.

    std::vector<int> ACTION_SPACE; ///< Action space used by every nodes (bandit arms).
    unsigned BUDGET; ///< Algorithm budget (number of expanded nodes).
    unsigned HORIZON; ///< Algorithm horizon for the default policy.
    double UCT_CST; ///< UCT constant factor.
    double DISCOUNT_FACTOR; ///< Discount factor for the MDP.
    double EPSILON; ///< Epsilon for the epsilon-optimal default policy.
    double MODEL_TRACK_LEN; ///< Model track length (half of the length of the track).
    double MODEL_STDDEV; ///< Model noise standard deviation.
    double MODEL_FAILURE_PROBABILITY; ///< Probability with chich the oposite action effect is applied in the model (randomness of the transition function).
    bool REUSE; ///< Set to true if the policy is able to reuse the tree.

    /**
     * @brief Simulation parameters default constructor
     *
     * The parameters are set to the values defined in this constructor.
     */
    parameters(
        double track_len = 2.,
        double stddev = 0.,
        double failure_probability = .1,
        double init_s = 0.,
        std::vector<int> action_space = std::vector<int>{-1,1},
        unsigned budget = 20,
        unsigned horizon = 50,
        double uct_cst = .7,
        double discount_factor = .9,
        double epsilon = 1.,
        double model_track_len = 2.,
        double model_stddev = 0.,
        double model_failure_probability = .1,
        bool reuse = false) :
        TRACK_LEN(track_len),
        STDDEV(stddev),
        FAILURE_PROBABILITY(failure_probability),
        INIT_S(init_s),
        ACTION_SPACE(action_space),
        BUDGET(budget),
        HORIZON(horizon),
        UCT_CST(uct_cst),
        DISCOUNT_FACTOR(discount_factor),
        EPSILON(epsilon),
        MODEL_TRACK_LEN(model_track_len),
        MODEL_STDDEV(model_stddev),
        MODEL_FAILURE_PROBABILITY(model_failure_probability),
        REUSE(reuse)
    {}
};

#endif // PARAMETERS_HPP_
