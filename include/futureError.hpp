//
// Created by angel on 7/30/18.
//

#pragma once

#ifndef CASSANDRACXX_FUTUREERROR_HPP
#define CASSANDRACXX_FUTUREERROR_HPP

#include <string>
#include <exception>
#include <stdexcept>
#include <future.hpp>

namespace cassandraxx {

class FutureError : public std::exception {
    mutable Future err_;
public:
    explicit FutureError(Future && f) noexcept
            : std::exception(), err_{std::move(f)} {
    }
    ~FutureError() noexcept override = default;
    const char* what() const noexcept {
        return err_.getErrorMessage().c_str();
    }
};

}


#endif //CASSANDRACXX_FUTUREERROR_HPP
