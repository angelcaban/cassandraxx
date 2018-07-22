//
// Created by eancaba on 6/29/2018.
//

#include <thread>
#include <string>

#include "client.hpp"

using std::string;
using std::chrono::milliseconds;

namespace cassandraxx {

Client::Client(LogCallback & cp)
        : session_(&cluster_), logger_(cp) {
}

void Client::storeCredentails(string const &un,
                              string const &pw) {
    cluster_.setCredentials(un, pw);
}

template <class ITER>
void Client::connectSession(ITER begin, ITER end) {
    string cps;
    while (begin != (end - 1)) {
        cps += *begin++;
        cps += ",";
    }

    cluster_.setContactPoints(cps);

    SessionState state = SessionState::CLOSED;
    while (state != SessionState::OPEN) {
        state = session_.connect(initial_keyspace_);
        if (state == SessionState::CLOSED) {
            logger_(session_.getLastSessionError());
        }
        logger_("Retrying Connection to [" + cps + "]");

        std::this_thread::sleep_for(milliseconds{1000});
    }
}


}
