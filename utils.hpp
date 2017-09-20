#ifndef UTILS_HPP_
#define UTILS_HPP_

constexpr double COMPARISON_THRESHOLD = 1e-6;
constexpr unsigned NB_ACTIONS = 3; // Number of actions: fixed parameter of the simulator

/** @brief Return true if a == b up to a certain precision */
template <class T1, class T2>
constexpr bool is_equal_to(T1 a, T2 b) {
    return std::fabs(a-b)<COMPARISON_THRESHOLD;
}

/** @brief Return true if a < b  up to a certain precision */
template <class T1, class T2>
constexpr bool is_less_than(T1 a, T2 b) {
    return a<(b-COMPARISON_THRESHOLD);
}

/** @brief Return true if a > b  up to a certain precision */
template <class T1, class T2>
constexpr bool is_greater_than(T1 a, T2 b) {
    return a>(b+COMPARISON_THRESHOLD);
}

/** @brief Shuffle the content randomly */
template <class T>
inline void shuffle(std::vector<T> &v) {
    std::random_shuffle(v.begin(), v.end());
}

/**
 * @brief Pick a random indice of the input vector
 * @param {const std::vector<T> &} v; input vector
 * @return {unsigned} a random indice
 * @note Initialize a random seed when executing your program
 * @note Template method, independent of the content type
 */
template <class T>
inline unsigned rand_indice(const std::vector<T> &v) {
    assert(v.size() != 0);
    return rand() % v.size();
}

/**
 * @brief Pick a random element of the input vector
 * @param {const std::vector<T> &} v; input vector
 * @return {int} a random indice
 * @note initialize a random seed when executing your program
 * @note template method, independent of the content type
 */
template <class T>
inline T rand_element(const std::vector<T> &v) {
    return v.at(rand_indice(v));
}

/**
 * @brief Get the indice of the maximum element in the input vector, ties are
 * broken arbitrarily
 * @param {const std::vector<T> &} v; input vector
 * @return Indice of the maximum element in the input vector
 * @note template method
 */
template <class T>
inline unsigned argmax(const std::vector<T> &v) {
    double maxval = *std::max_element(v.begin(),v.end());
    std::vector<unsigned> up_ind;
    for (unsigned j=0; j<v.size(); ++j) {
        if(!is_less_than(v[j],maxval)) {up_ind.push_back(j);}
    }
    return rand_element(up_ind);
}

/** @brief See 'argmax' method */
template <class T>
inline unsigned argmin(const std::vector<T> &v) {
    double minval = *std::min_element(v.begin(),v.end());
    std::vector<unsigned> lo_ind;
    for (unsigned j=0; j<v.size(); ++j) {
        if(!is_greater_than(v[j],minval)) {lo_ind.push_back(j);}
    }
    return rand_element(lo_ind);
}

/** @brief Generate a uniformly distributed integer */
int uniform_i(const int &int_min, const int &int_max) {
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> distribution(int_min,int_max);
    return distribution(generator);
}

/** @brief Generate a uniformly distributed double */
double uniform_d(const double &double_min, const double &double_max) {
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<double> distribution(double_min,double_max);
    return distribution(generator);
}

/** @brief Generate a normally distributed double */
double normal_d(const double &mean, const double &stddev) {
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::normal_distribution<double> distribution(mean,stddev);
    return distribution(generator);
}

template <class T>
void print(T t) {
	std::cout << t << std::endl;
}

template <class T>
void printv(std::vector<T> v) {
	for (auto &elt : v) {
		std::cout << elt << " ";
	}
	std::cout << std::endl;
}

#endif
