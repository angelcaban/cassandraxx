//
// Created by eancaba on 6/29/2018.
//

#include "session.hpp"

namespace cassandraxx {

SessionState Session::connect(std::string const &keyspace) {
    Future f;
    SessionState ret = SessionState::CLOSED;

    if (keyspace.empty()) {
        f.reset(cass_session_connect(mem(), parent_->mem()));
    } else {
        f.reset(cass_session_connect_keyspace_n(mem(),
                                                parent_->mem(),
                                                keyspace.c_str(),
                                                keyspace.length()));
    }

    if (CASS_OK != f.getErrorCode()) {
        setLastSessionError(std::move(f.getError()));
    } else {
        ret = SessionState::OPEN;
    }

    state_.exchange(ret, std::memory_order_release);
    return ret;
}

Future Session::close() {
    return std::move(Future{cass_session_close(mem())});
}

Future Session::prepareQuery(std::string const &query) {
    return std::move(Future{cass_session_prepare_n(mem(),
                                                   query.c_str(),
                                                   query.length())});
}

Future Session::prepareExisting(Statement &statement) {
    return std::move(
            Future{cass_session_prepare_from_existing(mem(),
                                                      statement.mem())});
}

Future Session::execute(Statement &statement) {
    return std::move(
            Future{cass_session_execute(mem(), statement.mem())});
}

Future Session::executeBatch(CassBatch const *batch) {
    return std::move(
            Future{cass_session_execute_batch(mem(), batch)});
}

const CassSchemaMeta* Session::getSchemaMeta() {
    return cass_session_get_schema_meta(mem());
}

Error& Session::getLastSessionError() const {
    return last_error_;
}

}
