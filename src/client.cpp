//
// Created by eancaba on 6/29/2018.
//

#include <thread>
#include <string>

#include "futureError.hpp"
#include "client.hpp"

using std::string;
using std::chrono::milliseconds;

namespace cassandraxx {

Client::Client(LogCallback & cp)
        : session_(&cluster_), logger_(cp) {
}

Client::~Client() {
    if (session_.isConnected()) {
        Future future = session_.close();
        if (CASS_OK != future.getErrorCode()) {
            logger_(future.getErrorMessage());
        }
    }
}

void Client::storeCredentials(string const &un,
                              string const &pw) {
    cluster_.setCredentials(un, pw);
}

void Client::connectSession_priv(string & cps) {
    cluster_.setContactPoints(cps);
    bool keep_retrying = (connect_retry_count_ != -1);
    int retry_count = 0;

    SessionState state = SessionState::CLOSED;
    while (state != SessionState::OPEN) {
        state = session_.connect(initial_keyspace_);
        if (state == SessionState::CLOSED) {
            logger_(session_.getLastSessionError());
        }
        if (keep_retrying) {
            ++retry_count;
            keep_retrying = retry_count < connect_retry_count_;
        } else {
            break;
        }
        logger_("Retrying Connection to [" + cps + "]");

        std::this_thread::sleep_for(milliseconds{1000});
    } // while connection !open
}


}
