//
// Created by angel on 7/10/18.
//

#pragma once

#ifndef CASSANDRACXX_STATEMENT_HPP
#define CASSANDRACXX_STATEMENT_HPP

#include <memory>
#include <vector>
#include <tuple>
#include <variant>
#include <utility>
#include <cstdint>
#include <stdint-gcc.h>

#include <boost/format.hpp>

#include <cassandra.h>

namespace cassandraxx {

struct CassStatementDeleter {
    void operator()(CassStatement *ptr) {
        cass_statement_free(ptr);
    }
};

enum class StatementAction : int_fast32_t {
    SELECT = 0,
    UPDATE,
    DELETE,
    INSERT,
    CREATE_INDEX,
    CREATE_FUNCTION,
    CREATE_KEYSPACE,
    CREATE_MATERIALIZED_VIEW,
    CREATE_TABLE,
    CREATE_TRIGGER,
    CREATE_TYPE,
    CREATE_ROLE,
    DROP,
    TRUNCATE,
};

using ParamValue = std::variant<
        uint8_t, uint16_t, uint32_t,
        int8_t, int16_t, int32_t,
        float, double,
        bool, std::string>;
using KeyValuePair = std::tuple<std::string, ParamValue>;
using Parameters = std::vector<KeyValuePair>;


/*! Builds a statement object based on one of two paths:
 *    1) The full query is specified in as a string, which results in
 *       the creation of zero-parameter explicit statement:
 *    \code{.cpp}
 *    StatementBuilder builder;
 *    builder.query("CQL STATEMENT");
 *    // any other method other than build will not affect end result
 *    Statement s = builder.build();
 *    \endcode
 *
 *    2) Action, table, keyspace, constraints & parameters
 *       are specified individually:
 *    \code{.cpp}
 *    StatementBuilder builder;
 *    builder.keyspace("keyspace")
 *      .target("my_table")
 *      .param("a", "w");
 *    Statement s = builder.build();
 *    // s is "SELECT * FROM keyspace.my_table WHERE a = 'w'"
 *    \endcode
 */
class StatementBuilder {
private:
    StatementAction action_;
    CassError err_ = CASS_OK;
    std::string target_;
    std::string keyspace_;
    std::string query_;
    Parameters params_;
    std::vector<std::string> constraints_;

    using Me = StatementBuilder;

    void calculateWhereClause(std::vector<std::string> &);
    void separateKeys(std::string & keys);
    void calculateTarget(std::string &);
    void handleSelect(std::string const& constraints,
                      std::string const& target);
    void handleUpdate(std::string const& target,
                      std::string const& constraints,
                      std::vector<std::string>& kvps);
    void handleDelete(std::string const& constraints,
                      std::string const& target,
                      std::vector<std::string>& kvps);
    void handleInsert(std::string const& target,
                      size_t & param_size);

    CassError bindParameters(CassStatement * s);

public:
    StatementBuilder()
            : action_(StatementAction::SELECT) {
    }
    explicit StatementBuilder(std::string const& keyspace)
            : action_(StatementAction::SELECT),
              keyspace_(keyspace) {
    }

    operator bool() const {
        return err_ == CASS_OK;
    }
    CassError getErrorCode() const {
        return err_;
    }

    Me& query(std::string const& query) {
        query_.assign(query);
        return *this;
    }
    Me& action(StatementAction act) {
        action_ = act;
        return *this;
    }
    Me& keyspace(std::string const& ks) {
        keyspace_.assign(ks);
        return *this;
    }
    Me& target(std::string const& targ) {
        target_.assign(targ);
        return *this;
    }
    Me& addParam(std::string const& param,
                 ParamValue const& value) {
        params_.emplace_back(param, value);
        return *this;
    }
    Me& addConstraint(std::string const& restraint) {
        constraints_.push_back(restraint);
        return *this;
    }
    class Statement build();
};

class Statement {
    friend class Session;

private:
    std::unique_ptr<CassStatement, CassStatementDeleter> statement_;
    CassStatement* mem() { return statement_.get(); }

public:
    Statement() = default;
    Statement(std::string const& query) {
        statement_.reset(cass_statement_new_n(query.c_str(), query.length(), 0));
    }
    Statement(CassStatement *s) : statement_(s) {
    }
    Statement(Statement && o) : statement_(std::move(o.statement_)) {
    }
    Statement(Statement const& o) = delete;
    ~Statement() = default;

    CassError addKeyIndex(size_t index);
    CassError setConsistency(CassConsistency consistency);
    CassError setSerialConsistency(CassConsistency consistency);
    CassError setPagingSize(int page_size);
    CassError setPagingState(CassResult const* result);
    CassError setRequestTimeout(uint64_t timeout_ms);
    CassError setIndempotent(bool enable);
};

} // namesapce cassandraxx

#endif //CASSANDRACXX_STATEMENT_HPP
