//
// Created by Stepan on 12.09.2021.
//

#ifndef MODERN_CPP_DESIGN_ERROR_TYPE_H
#define MODERN_CPP_DESIGN_ERROR_TYPE_H

#include <utility>

#define FUNCTION_NAME __PRETTY_FUNCTION__

#define make_error_pair(error) (error == ErrorType::None \
    ? std::make_pair(error, nullptr) \
    : std::make_pair(error, FUNCTION_NAME))

#define make_error_pair_if(is_not_ok, error) (is_not_ok \
    ? make_error_pair(error) \
    : make_error_pair(ErrorType::None))

enum class ErrorType {
    None = 0,
    UnknownDevice = 'd',
    WriteError = 'w',
    ReadError = 'r',
    ConvertError = 'c',
    UnknownError = 'u'
};

using ErrorPair = std::pair<ErrorType, const char*>;

#endif //MODERN_CPP_DESIGN_ERROR_TYPE_H
