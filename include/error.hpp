//
// Created by angel on 7/12/18.
//

#ifndef CASSANDRACXX_ERROR_HPP
#define CASSANDRACXX_ERROR_HPP

#include <cstdint>
#include <memory>
#include <variant>

#include <cassandra.h>

namespace cassandraxx {

struct CassErrorDeleter {
    void operator()(CassErrorResult *ptr) {
        cass_error_result_free(ptr);
    }
};

using CassErrorResultPtr = std::unique_ptr<CassErrorResult, CassErrorDeleter>;
using ErrorVariant = std::variant<CassErrorResultPtr, CassError>;

class Error {
private:
    ErrorVariant error_;
    std::string err_msg_;
    CassErrorResult const* mem() const {
        const auto pval = std::get_if<CassErrorResultPtr>(&error_);
        return (pval == nullptr) ? nullptr : pval->get();
    }

public:
    Error() = default;
    Error(CassErrorResult const* err)
        : error_{std::in_place_type_t<CassErrorResultPtr>{},
                 const_cast<CassErrorResult*>(err)} {
    }
    Error(CassError err)
        : error_{std::in_place_type_t<CassError>{}, err} {
    }
    Error(Error const &o) = delete;
    Error(Error &&o) : error_(std::move(o.error_)) {
    }
    Error& operator=(Error&& o) {
        error_ = std::move(o.error_);
        return *this;
    }
    ~Error() = default;

    void setErrorMessage(std::string msg) {
        err_msg_ = msg;
    }
    std::string const& errorMessage() const {
        return err_msg_;
    }

    void resetResultError(CassErrorResult *future = nullptr) {
        error_ = ErrorVariant{std::in_place_type_t<CassErrorResultPtr>{},
                              future};
    }
    bool isResultErrorSet() const {
        return mem() != nullptr;
    }

    CassError getErrorCode() const;
    CassConsistency getConsistency() const;
    int32_t getNumReceivedResponses() const;
    int32_t getNumRequiredResponses() const;
    int32_t getNumNodesFailed() const;
    bool wasDataPresent() const;
    CassWriteType getWriteType() const;
};

} // namespace cassandraxx

#endif //CASSANDRACXX_ERROR_HPP
