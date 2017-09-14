#ifndef AGENT_HPP_
#define AGENT_HPP_

/** @brief TODO */

struct agent {
    /**
     * @brief Attributes
     * @param {double} s; current state: value on the track
     * @param {unsigned} a; current action in {-1,0,1}
     */
    double s;
    int a;

    /** @brief Constructor */
    agent(double _s) : s(_s) {}

    /**
     * @brief Take an action based on current state s and set the action attribute
     * @note Action must be in {-1, 0, 1}
     */
    void take_action() {
        a = 1;
    }
};

#endif
