#ifndef UTILS_HPP_
#define UTILS_HPP_

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
