#ifndef DISPLAY_HPP_
#define DISPLAY_HPP_

/**
 * @brief Print track
 *
 * Print a nice overview of the track with the agents position.
 * @param {track &} tr; reference to the track
 * @param {agent &} ag; reference to the agent
 */
void print_track(track &tr, agent &ag) {
    double s = std::abs(ag.s);
    double m = tr.track_length / 10.;
    int pos = 0;
    for(unsigned i=0; i<=10; ++i) {
        if(is_less_than(s,.5*m + ((double)i)*m)) {
            pos = (int) i;
            break;
        }
    }
    if(ag.s<0) {pos = -pos;}
    for(int i=-10; i<=10; ++i) {
        if(i == pos) {
            std::cout << "X";
        } else {
            if(i==-10 || i==0 || i==10) {std::cout << "|";}
            else {std::cout << "-";}
        }
    }
}

/**
 * @brief Print
 *
 * Print some informations about the state, action and reward. Also print the track.
 * @param {track &} tr; reference to the track
 * @param {agent &} ag; reference to the agent
 */
void print(track &tr, agent &ag) {
    std::cout << "t:" << tr.t << " ";
    if(tr.t<10){std::cout << " ";}
    print_track(tr,ag);
    std::cout << " s:" << ag.s << " ";
    std::cout << "a:" << ag.a << " ";
    std::cout << "r:" << tr.reward(ag.s,0,0.) << "\n";
}

#endif // DISPLAY_HPP_
