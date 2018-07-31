//
// Created by eancaba on 6/29/2018.
//

#pragma once

#ifndef CASSANDRACXX_SESSION_HPP
#define CASSANDRACXX_SESSION_HPP

#include <memory>
#include <atomic>
#include <cstdint>
#include <stdint-gcc.h>

#include <cassandra.h>

#include "error.hpp"
#include "cluster.hpp"
#include "future.hpp"
#include "statement.hpp"

namespace cassandraxx {

struct CassSessionDeleter {
    void operator() (CassSession* ptr) {
        cass_session_free(ptr);
    }
};

enum class SessionState : int_fast32_t {
    CLOSED = 0,
    OPEN,
};

class Session {
private:
    Cluster *const parent_;

    std::unique_ptr<CassSession, CassSessionDeleter> session_;
    std::atomic<SessionState> state_;
    Error last_error_;

    CassSession* mem() { return session_.get(); }

public:
    Session(Cluster *const parent) : parent_(parent) {
        session_.reset(cass_session_new());
        std::atomic_init(&state_, SessionState::CLOSED);
    }
    Session(Session const& o) = delete;
    Session(Session && o)
            : session_(std::move(o.session_)),
              parent_(std::move(o.parent_)) {
        std::atomic_init(&state_, o.state_.load(std::memory_order_acquire));
    }
    ~Session() = default;

    bool isConnected() const {
        return state_ == SessionState::OPEN;
    }

    SessionState connect() { return connect(""); }
    SessionState connect(std::string const& keyspace);
    Future close();

    Future prepareQuery(std::string const& query);
    Future prepareExisting(Statement & statement);

    Future execute(Statement & statement);
    Future executeBatch(CassBatch const* batch);

    const CassSchemaMeta* getSchemaMeta();

    Error const& getLastSessionError() const;
    void setLastSessionError(Error && e) {
        last_error_ = std::move(e);
    }
};

} // namespace cassandraxx

#endif //CASSANDRACXX_SESSION_HPP
