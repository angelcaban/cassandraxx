//
// Created by angel on 7/22/18.
//

#include <sstream>
#include <cstdlib>

#include <client.hpp>
#include <errorNotSetException.hpp>

#include "gtest/gtest.h"

using namespace std;

constexpr char test_database_name[] = "c_tester";
constexpr char test_database_ver[] = "3.11.2";
static const char* ccm_cmd = secure_getenv("CCM");

namespace {

struct TestLogger : public cassandraxx::LogCallback {
    std::stringstream test_messages;
    TestLogger() : test_messages{""} {
    }
    void operator()(std::string const &message) {
        test_messages << message << "\n";
    }
};

int sysexec(std::string const& cmd) {
    return std::system(cmd.c_str());
}

bool ccm_create_db() {
    string cmd_line{ccm_cmd};
    cmd_line += " create ";
    cmd_line += test_database_name;
    cmd_line += " -v ";
    cmd_line += test_database_ver;

    if (0 == sysexec(cmd_line)) {
        cmd_line.assign(ccm_cmd);
        cmd_line += " populate -n 1";
        if (0 == sysexec(cmd_line)) {
            cmd_line.assign(ccm_cmd);
            cmd_line += " start";
            return 0 == sysexec(cmd_line);
        }
    }

    return false;
}

bool ccm_destroy_db() {
    string cmd_line{ccm_cmd};
    cmd_line += " remove";
    return 0 == sysexec(cmd_line);
}

static TestLogger logger;

class ClientDbTests : public ::testing::Test {
public:
    cassandraxx::Client client;

    ClientDbTests() : client{logger} {
        logger.test_messages.str("");
    }
    ~ClientDbTests() override = default;

    void SetUp() override {
        ASSERT_EQ(true, ccm_create_db());
    }

    void TearDown() override {
        ASSERT_EQ(true, ccm_destroy_db());
        if (auto msgs = logger.test_messages.str(); msgs.empty()) {
            std::cerr << msgs << "\n";
        }
    }
};

TEST(Client, DefaultCtor) {
    cassandraxx::Client client{logger};

    EXPECT_EQ(false, (bool)client);
    EXPECT_EQ(false, client.session().isConnected());
    EXPECT_EQ("", client.initialKeyspace());

    auto const& lastError = client.lastSessionError();
    EXPECT_EQ(false, lastError.isResultErrorSet());
    EXPECT_EQ(CASS_OK, lastError.getErrorCode());

    auto msgs = logger.test_messages.str();
    EXPECT_EQ("", msgs);
}

TEST_F(ClientDbTests, ConnectNoKeyspace) {
    EXPECT_EQ(false, (bool)client);
    EXPECT_NO_THROW(client.connectSession("127.0.0.1"));
    EXPECT_EQ(true, (bool)client);
}

} // inline namespace
