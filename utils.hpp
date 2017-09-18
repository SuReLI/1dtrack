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
