//
// Created by angel on 7/10/18.
//

#include <type_traits>

#include <boost/algorithm/string/join.hpp>

#include "statement.hpp"

using fmt = boost::format;
using std::string;
using std::vector;

namespace cassandraxx {

CassError Statement::addKeyIndex(size_t index) {
    return cass_statement_add_key_index(mem(), index);
}

CassError Statement::setConsistency(CassConsistency consistency) {
    return cass_statement_set_consistency(mem(), consistency);
}

CassError Statement::setSerialConsistency(CassConsistency consistency) {
    return cass_statement_set_serial_consistency(mem(), consistency);
}

CassError Statement::setPagingSize(int page_size) {
    return cass_statement_set_paging_size(mem(), page_size);
}

CassError Statement::setPagingState(CassResult const *result) {
    return cass_statement_set_paging_state(mem(), result);
}

CassError Statement::setRequestTimeout(uint64_t timeout_ms) {
    return cass_statement_set_request_timeout(mem(), timeout_ms);
}

CassError Statement::setIndempotent(bool enable) {
    return cass_statement_set_is_idempotent(mem(),
                                            static_cast<cass_bool_t>(enable));
}

} // namespace cassandraxx
