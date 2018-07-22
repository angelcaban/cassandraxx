//
// Created by eancaba on 6/29/2018.
//

#ifndef CASSANDRACXX_CLIENT_HPP
#define CASSANDRACXX_CLIENT_HPP

#include <array>
#include <type_traits>

#include "session.hpp"

namespace cassandraxx {

struct LogCallback {
    virtual void operator()(Error const& err) {
        this->operator()(cass_error_desc(err.getErrorCode()));
    }
    virtual void operator()(std::string const& message) = 0;
};

class Client {
private:
    Cluster cluster_;
    Session session_;
    std::string initial_keyspace_;
    LogCallback & logger_;

public:
    Client(LogCallback &);
    ~Client() = default;

    void setInitialKeyspace(std::string const& ks) {
        initial_keyspace_ = ks;
    }
    std::string & getInitialKeyspace() const {
        return initial_keyspace_;
    }

    operator bool() const {
        return session_.isConnected();
    }

    template <>
    void connectSession() {
        std::static_assert(std::false_type::value,
                           "Must supply contact points to connect to.");
    }
    template <class... Ts>
    void connectSession(Ts... contact_points) {
        using T = std::common_type<Ts...>::type;
        std::array<T, sizeof...(Ts)> cp_list{contact_points...};
        return connectSession(std::begin(cp_list), std::end(cp_list));
    }
    template <class ITER>
    void connectSession(ITER begin, ITER end);

    void storeCredentails(std::string const& un,
                          std::string const& pw);

    Cluster& cluster() {
        return cluster_;
    }
    Session& session() {
        return session_;
    }

    bool query(std::string const& qu);
    bool query(Statement & statement);
    bool query(StatementBuilder & builder);
};

} // namespace cassandraxx

#endif //CASSANDRACXX_CLIENT_HPP
