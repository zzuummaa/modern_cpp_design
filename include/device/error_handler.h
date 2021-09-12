//
// Created by Stepan on 12.09.2021.
//

#ifndef MODERN_CPP_DESIGN_ERROR_HANDLER_H
#define MODERN_CPP_DESIGN_ERROR_HANDLER_H

#include "error_type.h"
#include <utility>
#include <stdexcept>

class SimpleErrorHandler {
public:
    template<typename ErrorType>
    static bool process(ErrorType error) {
        return error == static_cast<ErrorType>(0);
    }

    template<typename ErrorType, typename ReasonType>
    static bool process(const std::pair<ErrorType, ReasonType>& error_pair) {
        return error_pair.first == static_cast<ErrorType>(0);
    }
};

class ThrowErrorHandler {
public:
    template<typename ErrorType>
    static bool process(ErrorType error) {
        char c[2] = { static_cast<char>(error), 0 };
        if (error != (ErrorType)0) throw std::runtime_error(c);
        return true;
    }

    template<typename ErrorType, typename ReasonType>
    static bool process(const std::pair<ErrorType, ReasonType>& error_pair) {
        if (error_pair.first != static_cast<ErrorType>(0)) throw std::runtime_error(error_pair.second);
        return true;
    }
};

#endif //MODERN_CPP_DESIGN_ERROR_HANDLER_H
