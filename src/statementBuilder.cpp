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

namespace {
string quoteVisitor(ParamValue &val) {
    string ret;
    std::visit([&](auto &&item) {
        using T = std::decay_t<decltype(item)>;
        if constexpr (std::is_integral<T>::value ||
                      std::is_floating_point<T>::value) {
            ret += std::to_string(item);
        } else {
            ret += "'";
            ret += item;
            ret += "'";
        }
    }, val);
    return std::move(ret);
}
}

void StatementBuilder::calculateWhereClause(vector<string> & kvps) {
    kvps.reserve(params_.size());
    for (auto& param : params_) {
        string kvp;
        kvp += std::get<0>(param);
        kvp += "=";
        kvp += quoteVisitor(std::get<1>(param));
        kvps.push_back(kvp);
    }
}

void StatementBuilder::calculateTarget(string & target) {
    if (!keyspace_.empty()) {
        target += keyspace_;
        target += ".\"";
        target += target_;
        target += "\"";
    }
}

void StatementBuilder::separateKeys(string &keys) {
    for (auto& param : params_) {
        keys += std::get<0>(param);
    }
}

CassError StatementBuilder::bindParameters(CassStatement * s) {
    CassError ret = CASS_OK;
    for (auto && param : params_) {
        string & name = std::get<0>(param);
        std::visit([&](auto&& value) {
            using T = std::decay_t<decltype(value)>;

            if constexpr (std::is_same_v<T, uint8_t>) {
                ret = cass_statement_bind_uint32_by_name_n(
                    s, name.c_str(), name.length(),
                    static_cast<cass_uint32_t>(value));

            } else if constexpr (std::is_same_v<T, uint16_t>) {
                ret = cass_statement_bind_uint32_by_name_n(
                    s, name.c_str(), name.length(),
                    static_cast<cass_uint32_t>(value));

            } else if constexpr (std::is_same_v<T, uint32_t>) {
                ret = cass_statement_bind_uint32_by_name_n(
                    s, name.c_str(), name.length(),
                    static_cast<cass_uint32_t>(value));

            } else if constexpr (std::is_same_v<T, int8_t>) {
                ret = cass_statement_bind_int32_by_name_n(
                    s, name.c_str(), name.length(),
                    static_cast<cass_int32_t>(value));

            } else if constexpr (std::is_same_v<T, int16_t>) {
                ret = cass_statement_bind_int32_by_name_n(
                    s, name.c_str(), name.length(),
                    static_cast<cass_int32_t>(value));

            } else if constexpr (std::is_same_v<T, int32_t>) {
                ret = cass_statement_bind_int32_by_name_n(
                    s, name.c_str(), name.length(),
                    static_cast<cass_int32_t>(value));

            } else if constexpr (std::is_same_v<T, float>) {
                ret = cass_statement_bind_float_by_name_n(
                    s, name.c_str(), name.length(),
                    static_cast<cass_float_t>(value));

            } else if constexpr (std::is_same_v<T, double>) {
                ret = cass_statement_bind_double_by_name_n(
                    s, name.c_str(), name.length(),
                    static_cast<cass_double_t>(value));

            } else if constexpr (std::is_same_v<T, bool>) {
                ret = cass_statement_bind_bool_by_name_n(
                    s, name.c_str(), name.length(),
                    static_cast<cass_bool_t>(value));

            } else if constexpr (std::is_same_v<T, string>) {
                ret = cass_statement_bind_string_by_name_n(
                    s, name.c_str(), name.length(),
                    value.c_str(), value.length());
            }
        }, std::get<1>(param));

        if (ret != CASS_OK) { break; }
    } // for-each parameter

    return ret;
}

void StatementBuilder::handleSelect(string const &constraints,
                                    string const &target) {
    fmt query_fmt("SELECT %1% FROM %2%");
    query_ = (query_fmt % constraints % target).str();

    // Calculate WHERE-clause
    if (!params_.empty()) {
        vector<string> kvps;
        calculateWhereClause(kvps);
        query_ += " WHERE ";
        query_ += boost::algorithm::join(kvps, " AND ");
    }
}

void StatementBuilder::handleUpdate(string const &target,
                                    string const &constraints,
                                    vector<string>& kvps) {
    fmt query_fmt("UPDATE %1% SET %2% WHERE %3%");
    calculateWhereClause(kvps);
    query_ = (query_fmt % target
              % constraints
              % boost::algorithm::join(kvps, " AND ")).str();
}

void StatementBuilder::handleDelete(string const &constraints,
                                    string const &target,
                                    std::vector<std::string>& kvps) {
    calculateWhereClause(kvps);
    fmt query_fmt("DELETE %1% FROM %2% WHERE %3%");
    query_ = (query_fmt % constraints % target
              % boost::algorithm::join(kvps, " AND ")).str();
}

void StatementBuilder::handleInsert(std::string const &target,
                                    size_t & param_size) {
    string keys;
    separateKeys(keys);

    string qmarks;
    param_size = params_.size();
    qmarks.reserve(sizeof ("?,") * param_size);
    for (int i = 0; i < param_size - 1; ++i) {
        qmarks += "?,";
    }
    qmarks += "?";

    fmt query_fmt("INSERT INTO %1% (%2%) VALUES (%3%)");
    query_ = (query_fmt % target % keys % qmarks).str();
}

Statement StatementBuilder::build() {
    if (!query_.empty()) {
        return std::move(Statement{query_});
    }

    vector<string> kvps;
    size_t param_size = 0;
    vector<string> values;

    string target;
    calculateTarget(target);

    string constraints;
    if (!constraints_.empty()) {
        constraints = boost::algorithm::join(constraints_, ",");
    }

    switch (action_) {
        case StatementAction::SELECT:
            handleSelect((constraints.empty() ? "*" : constraints),
                         target);
            break;

        case StatementAction::TRUNCATE:
            query_ = "TRUNCATE ";
            query_ += target;
            break;

        case StatementAction::UPDATE:
            handleUpdate(target, constraints, kvps);
            break;

        case StatementAction::DELETE:
            handleDelete(constraints, target, kvps);
            break;

        case StatementAction::INSERT:
            handleInsert(target, param_size);
            break;
    }

    CassStatement * s = cass_statement_new_n(
            query_.c_str(), query_.length(), param_size);
    err_ = bindParameters(s);
    if (err_ == CASS_OK) {
        return std::move(Statement{s});
    }
    return std::move(Statement{});
}

} // namespace cassandraxx
