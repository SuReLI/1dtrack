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
