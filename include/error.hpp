//
// Created by angel on 7/12/18.
//

#ifndef CASSANDRACXX_ERROR_HPP
#define CASSANDRACXX_ERROR_HPP

#include <cstdint>
#include <memory>

#include <cassandra.h>

namespace cassandraxx {

struct CassErrorDeleter {
    void operator()(CassErrorResult *ptr) {
        cass_error_result_free(ptr);
    }
};

class Error {
private:
    std::unique_ptr<CassErrorResult, CassErrorDeleter> error_;
    CassErrorResult const* mem() const { return error_.get(); }

public:
    Error() = default;
    Error(CassErrorResult const* err) {
        error_.reset(const_cast<CassErrorResult*>(err));
    }
    Error(Error const &o) = delete;
    Error(Error &&o) : error_(std::move(o.error_)) {
    }
    Error& operator=(Error&& o) {
        error_ = std::move(o.error_);
        return *this;
    }
    ~Error() = default;

    void reset(CassErrorResult *future = nullptr) {
        error_.reset(future);
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
