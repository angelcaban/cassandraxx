//
// Created by angel on 7/23/18.
//

#pragma once

#ifndef CASSANDRACXX_ERRORNOTSETEXCEPTION_HPP
#define CASSANDRACXX_ERRORNOTSETEXCEPTION_HPP

#include <string>
#include <exception>
#include <stdexcept>

namespace cassandraxx {

class ErrorNotSetException : public std::exception {
    std::string exception_msg_;
public:

    ErrorNotSetException() noexcept : std::exception() {
    }
    explicit ErrorNotSetException(std::string const& msg) noexcept
        : std::exception(), exception_msg_{msg} {
    }
    ErrorNotSetException(std::string && msg) noexcept
        : std::exception(), exception_msg_{std::move(msg)} {
    }
    ~ErrorNotSetException() noexcept override = default;

    const char* what() const noexcept {
        return exception_msg_.c_str();
    }
};

}

#endif //CASSANDRACXX_ERRORNOTSETEXCEPTION_HPP
