#ifndef TRACK_HPP_
#define TRACK_HPP_

struct track {
    /**
     * @brief Attributes
     * @param {double} len; length of the 1D track
     * @param {double} stddev; noise standard deviation
     * @param {unsigned} t; time
     */
    double len;
    double stddev;
    unsigned t;

    /** @brief constructor */
    track(double _len, double _stddev) : len(_len), stddev(_stddev) {
        t = 0;
    }

    /** @brief Return true if terminal */
    bool is_terminal(const double &s) {
        return !is_less_than(std::abs(s),len);
    }

    /** @brief Change the input state given the taken action */
    void transition(double &s, const int &a) {
        switch(a) {
        case (-1) : {
            s -= 1.;
            break;
        }
        case (0) : {break;}
        case (1) : {
            s += 1.;
            break;
        }
        default : {std::cout << "Illegal action" << std::endl;}
        }
        std::default_random_engine generator;
        std::normal_distribution<double> distribution(0.,stddev);
        double noise = distribution(generator);
        s += noise;
        ++t;
    }

    /** @brief Reward function */
    double reward(const double &s) {
        return is_less_than(std::abs(s),len) ? 0. : 1.;
    }
};

#endif
