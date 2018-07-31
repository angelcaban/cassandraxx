//
// Created by eancaba on 6/29/2018.
//

#pragma once

#ifndef CASSANDRACXX_CLIENT_HPP
#define CASSANDRACXX_CLIENT_HPP

#include <array>
#include <type_traits>

#include <cassandra.h>
#include "session.hpp"

namespace cassandraxx {

struct LogCallback {
    virtual void operator()(Error const& err) {
        this->operator()(cass_error_desc(err.getErrorCode()));
        if (auto & msg = err.errorMessage(); !msg.empty()) {
            this->operator()(msg);
        }
    }
    virtual void operator()(std::string const& message) = 0;
};

class Client {
private:
    Cluster cluster_;
    Session session_;
    std::string initial_keyspace_;
    LogCallback & logger_;
    int connect_retry_count_ = -1;

public:
    Client(LogCallback &);
    ~Client();

    void setInitialKeyspace(std::string const& ks) {
        initial_keyspace_ = ks;
    }
    std::string const& initialKeyspace() const {
        return initial_keyspace_;
    }

    void setConnectRetryCount(int count) {
        connect_retry_count_ = count;
    }
    int connectRetryCount() const {
        return connect_retry_count_;
    }

    operator bool() const {
        return session_.isConnected();
    }

    template <class... Ts>
    void connectSession(Ts... contact_points) {
        using T = typename std::common_type<Ts...>::type;
        std::array<T, sizeof...(Ts)> cp_list{contact_points...};
        std::string cps;
        auto b = std::begin(cp_list);
        auto e = std::end(cp_list);
        while (b != (e - 1)) {
            cps += *b++; cps += ",";
        }
        cps += *(e - 1);
        return connectSession_priv(cps);
    }

    void storeCredentials(std::string const &un,
                          std::string const &pw);

    Cluster& cluster() {
        return cluster_;
    }
    Session& session() {
        return session_;
    }

    Error const& lastSessionError() const {
        return session_.getLastSessionError();
    }

    bool query(std::string const& qu);
    bool query(Statement & statement);
    bool query(StatementBuilder & builder);

private:
    void connectSession_priv(std::string & cps);
};

} // namespace cassandraxx

#endif //CASSANDRACXX_CLIENT_HPP
