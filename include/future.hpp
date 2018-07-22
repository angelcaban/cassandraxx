//
// Created by angel on 7/10/18.
//

#pragma once

#ifndef CASSANDRACXX_FUTURE_HPP
#define CASSANDRACXX_FUTURE_HPP

#include <memory>
#include <string>
#include <cassandra.h>

#include "error.hpp"

namespace cassandraxx {

struct FutureCallback {
    class Future & future_;
    void *data = nullptr;

    virtual bool operator() () = 0;
};

struct CassFutureDeleter {
    void operator() (CassFuture* ptr) {
        cass_future_free(ptr);
    }
};

class Future {
private:
    std::unique_ptr<CassFuture, CassFutureDeleter> future_;
    CassFuture* mem() { return future_.get(); }

public:
    Future() = default;
    Future(CassFuture * future) {
        future_.reset(future);
    }
    Future(Future const& o) = delete;
    Future(Future && o) : future_(std::move(o.future_)) {
    }
    ~Future() = default;

    void reset(CassFuture* future = nullptr) {
        future_.reset(future);
    }

    void wait();
    bool isReady();
    bool timedWait(uint64_t timeout_us);

    const CassResult* getResult();
    const CassPrepared* getPrepared();

    Error getError();
    CassError getErrorCode();
    std::string getErrorMessage();

    size_t getPayloadItemCount();

};

} // namespace cassandraxx

#endif //CASSANDRACXX_FUTURE_HPP
