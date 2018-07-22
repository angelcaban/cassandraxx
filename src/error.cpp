//
// Created by angel on 7/12/18.
//

#include "error.hpp"


namespace cassandraxx {

CassError Error::getErrorCode() const {
    return cass_error_result_code(mem());
}

CassConsistency Error::getConsistency() const {
    return cass_error_result_consistency(mem());
}

int32_t Error::getNumNodesFailed() const {
    return cass_error_result_num_failures(mem());
}

int32_t Error::getNumReceivedResponses() const {
    return cass_error_result_responses_received(mem());
}

int32_t Error::getNumRequiredResponses() const {
    return cass_error_result_responses_required(mem());
}

bool Error::wasDataPresent() const {
    return cass_error_result_data_present(mem());
}

CassWriteType Error::getWriteType() const {
    return cass_error_result_write_type(mem());
}

}
