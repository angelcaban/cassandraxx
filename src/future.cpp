//
// Created by angel on 7/10/18.
//

#include "future.hpp"

namespace cassandraxx {

void Future::wait() {
    cass_future_wait(mem());
}

bool Future::isReady() {
    return cass_future_ready(mem());
}

bool Future::timedWait(uint64_t timeout_us) {
    return cass_future_wait_timed(mem(), timeout_us);
}

const CassResult* Future::getResult() {
    return cass_future_get_result(mem());
}

Error Future::getError() {
    return std::move(Error{cass_future_get_error_result(mem())});
}

const CassPrepared* Future::getPrepared() {
    return cass_future_get_prepared(mem());
}

CassError Future::getErrorCode() {
    return cass_future_error_code(mem());
}

std::string Future::getErrorMessage() {
    const char* msg_out;
    size_t msg_len;
    cass_future_error_message(mem(), &msg_out, &msg_len);

    return (std::move(std::string{msg_out, msg_len}));
}

size_t Future::getPayloadItemCount() {
    return cass_future_custom_payload_item_count(mem());
}

}
