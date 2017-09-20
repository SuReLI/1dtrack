#ifndef TRACK_HPP_
#define TRACK_HPP_

struct track {
    /**
     * @brief Attributes
     * @param {double} len; length of the 1D track
     * @param {double} stddev; noise standard deviation
     * @param {double} failure_probability; probability with chich the oposite action
     * effect is applied (randomness of the transition function)
     * @param {unsigned} t; time
     */
    double len;
    double stddev;
    double failure_probability;
    unsigned t;

    /** @brief constructor */
    track(
        double _len,
        double _stddev,
        double _failure_prob) :
        len(_len),
        stddev(_stddev),
        failure_probability(_failure_prob)
    {
        t = 0;
    }

    /** @brief Return true if terminal */
    bool is_terminal(const double &s) {
        return !is_less_than(std::abs(s),len);
    }

    /**
     * @brief Simulate a transition wrt the environment parameters
     * @return The resulting state
     */
    double transition(const double &s, const int &a) {
        double noise = normal_d(0.,stddev);
        double action_effect = (double) a;
        if(is_less_than(uniform_d(0.,1.),failure_probability)) {
            action_effect *= (-1.);
        }
        ++t;
        return s + action_effect + noise;
    }

    /** @brief Reward function */
    double reward(const double &s) {
        return is_less_than(std::abs(s),len) ? 0. : 1.;
    }
};

#endif
