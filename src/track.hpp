#ifndef TRACK_HPP_
#define TRACK_HPP_

/**
 * @brief Track class
 *
 * Class for the environment including the state transitions and the reward function.
 */
struct track {
    double track_length; ///< track_lengthgth of the 1D track
    double stddev; ///< Noise standard deviation
    double failure_probability; ///< Probability with chich the oposite action effect is applied (randomness of the transition function)
    unsigned time; ///< Time

    /** @brief constructor */
    track(
        double _track_length,
        double _stddev,
        double _failure_prob) :
        track_length(_track_length),
        stddev(_stddev),
        failure_probability(_failure_prob)
    {
        time = 0;
    }

    /**
     * @brief Is terminal
     *
     * Test if the state is terminal.
     * @param {const double &} s; tested state
     * @return Return 'true' if the state is terminal.
     */
    bool is_terminal(const double &s) {
        return !is_less_than(std::abs(s),track_length);
    }

    /**
     * @brief Transition method
     *
     * Simulate a transition w.r.t. the environments parameters.
     * @param {const double &} s; state
     * @param {const in &} a; action
     * @return The resulting state
     */
    double transition(const double &s, const int &a) {
        double noise = normal_double(0.,stddev);
        double action_effect = (double) a;
        if(is_less_than(uniform_double(0.,1.),failure_probability)) {
            action_effect *= (-1.);
        }
        ++time;
        return s + action_effect + noise;
    }

    /**
     * @brief Reward method
     *
     * Sample a reward w.r.t. the environments parameters. So far, only the current state is
     * used to get teh reward.
     * @param {const double &} s; state
     * @param {const int &} a; action
     * @param {const double &} s_p; next state
     */
    double reward(const double &s, const int &a, const double &s_p) {
        (void) a; (void) s_p; //default
        return is_less_than(std::abs(s),track_length) ? 0. : 1.;
    }
};

#endif // TRACK_HPP_
