#ifndef DISPLAY_HPP_
#define DISPLAY_HPP_

/** @brief Some display methods */

/** @brief Print an overview of the track */
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
    if(is_less_than(s,0.)) {pos *= -1;}
    for(int i=-10; i<=10; ++i) {
        if(i == pos) {
            std::cout << "X";
        } else {
            if(i==-10 || i==0 || i==10) {std::cout << "|";}
            else {std::cout << "-";}
        }
    }
}

/** @brief Print some informations */
void print(track &tr, agent &ag) {
//    std::cout << "time = " << tr.t;
//    std::cout << " state = " << ag.s;
//    std::cout << " action = " << ag.a;
//    std::cout << " reward = " << tr.reward(ag.s) << std::endl;
    std::cout << "t:" << tr.t << " ";
    if(tr.t<10){std::cout << " ";}
    print_track(tr,ag);
    std::cout << " s:" << ag.s << " ";
    std::cout << "a:" << ag.a << " ";
    std::cout << "r:" << tr.reward(ag.s) << "\n";
}

#endif // DISPLAY_HPP_
