//
// Created by angel on 7/12/18.
//

#include "error.hpp"
#include "errorNotSetException.hpp"

namespace cassandraxx {

CassError Error::getErrorCode() const {
    if (isResultErrorSet()) {
        return cass_error_result_code(mem());
    } else {
        return std::get<CassError>(error_);
    }
}

CassConsistency Error::getConsistency() const {
    if (!isResultErrorSet()) throw ErrorNotSetException("Use isResultErrorSet first");
    return cass_error_result_consistency(mem());
}

int32_t Error::getNumNodesFailed() const {
    if (!isResultErrorSet()) throw ErrorNotSetException("Use isResultErrorSet first");
    return cass_error_result_num_failures(mem());
}

int32_t Error::getNumReceivedResponses() const {
    if (!isResultErrorSet()) throw ErrorNotSetException("Use isResultErrorSet first");
    return cass_error_result_responses_received(mem());
}

int32_t Error::getNumRequiredResponses() const {
    if (!isResultErrorSet()) throw ErrorNotSetException("Use isResultErrorSet first");
    return cass_error_result_responses_required(mem());
}

bool Error::wasDataPresent() const {
    if (!isResultErrorSet()) throw ErrorNotSetException("Use isResultErrorSet first");
    return cass_error_result_data_present(mem());
}

CassWriteType Error::getWriteType() const {
    if (!isResultErrorSet()) throw ErrorNotSetException("Use isResultErrorSet first");
    return cass_error_result_write_type(mem());
}

}
