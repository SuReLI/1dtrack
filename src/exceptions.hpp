#ifndef EXCEPTIONS_HPP_
#define EXCEPTIONS_HPP_

/**
 * @brief Decision criterion selector exception
 *
 * Exception for wron decision criterion selector choice.
 */
struct decision_criterion_selector_exception : std::exception {
    explicit decision_criterion_selector_exception() noexcept {}
    virtual ~decision_criterion_selector_exception() noexcept {}

    virtual const char * what() const noexcept override {
        return "wrong decision criterion selector";
    }
};

#endif // EXCEPTIONS_HPP_
