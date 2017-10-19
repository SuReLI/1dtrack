#ifndef EXCEPTIONS_HPP_
#define EXCEPTIONS_HPP_

/**
 * @brief Wrong number of input argument exception
 *
 * Exception for wrong number of input arguments in main function.
 */
struct wrong_nb_input_argument_exception : std::exception {
    explicit wrong_nb_input_argument_exception() noexcept {}
    virtual ~wrong_nb_input_argument_exception() noexcept {}

    virtual const char * what() const noexcept override {
        return "number of input arguments unknown, see main function.\n";
    }
};

/**
 * @brief Decision criterion selector exception
 *
 * Exception for wrong decision criterion selector choice.
 */
struct decision_criterion_selector_exception : std::exception {
    explicit decision_criterion_selector_exception() noexcept {}
    virtual ~decision_criterion_selector_exception() noexcept {}

    virtual const char * what() const noexcept override {
        return "wrong decision criterion selector.\n";
    }
};

/**
 * @brief Action names configuration file exception
 *
 * Exception for wrong action names choice in configuration file.
 */
struct action_names_configuration_file_exception : std::exception {
    explicit action_names_configuration_file_exception() noexcept {}
    virtual ~action_names_configuration_file_exception() noexcept {}

    virtual const char * what() const noexcept override {
        return "in config file: actions names do not match.\n";
    }
};

/**
 * @brief Wrong syntax configuration file exception
 *
 * Exception for wrong syntax in configuration file.
 */
struct wrong_syntax_configuration_file_exception : std::exception {
    explicit wrong_syntax_configuration_file_exception() noexcept {}
    virtual ~wrong_syntax_configuration_file_exception() noexcept {}

    virtual const char * what() const noexcept override {
        return "in config file: please make sure that the syntaxes match.\n";
    }
};

#endif // EXCEPTIONS_HPP_
