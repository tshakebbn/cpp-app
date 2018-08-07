/** Copyright [year] [organization]
 *
 * @file test_log.cpp
 *
 * @brief Implementation of unit test fixture for logging utility.
 * A more elaborate description of unit test fixture. (optional)
 *
 * @author [organization]
 * @author [name] (optional)
 *
 * @par [unique category]
 * [designation]
 *
 * @par Notifications
 * [distribution designation]
 * [other designations]
 */

#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
#include <boost/filesystem.hpp>

#include "../src/utils/log.hpp"
#include "config.hpp"

namespace test_log {

class TestLog : public ::testing::Test {
 protected:
    TestLog(void) {
    }

    virtual ~TestLog(void) {
    }

    // helper function  to create good config file
    void create_config(std::string level) {
        std::ofstream conf_file(LOCAL_CONFIG_FILE, std::ofstream::out);
        if (!conf_file) {
            throw std::runtime_error("Unable to create local config file");
        }
        conf_file << "[logger]\n" << "level=" << level << "\n\n";
        conf_file << "[options]\n" << "name=example_one\n";
        conf_file.close();
    }

    // helper function  to create a bad config file
    void create_bad_config(void) {
        std::ofstream conf_file(LOCAL_CONFIG_FILE, std::ofstream::out);
        if (!conf_file) {
            throw std::runtime_error("Unable to create local config file");
        }
        conf_file.close();
    }

    // helper function  to create another bad config file
    void create_another_bad_config(void) {
        std::ofstream conf_file(LOCAL_CONFIG_FILE, std::ofstream::out);
        if (!conf_file) {
            throw std::runtime_error("Unable to create local config file");
        }
        conf_file << "[logger]\n" << "level=bad\n\n";
        conf_file.close();
    }

    // helper function to delete config file
    void delete_config(void) {
        std::remove(LOCAL_CONFIG_FILE);
    }

    // helper function to delete log file
    void delete_log(void) {
        std::remove(LOCAL_LOG_FILE);
    }
};

TEST_F(TestLog, ThrowsExceptions) {
    EXPECT_THROW(cpp_log::initialize(), std::runtime_error);
    create_bad_config();
    EXPECT_THROW(cpp_log::initialize(), boost::exception);
    delete_config();
    create_another_bad_config();
    EXPECT_THROW(cpp_log::initialize(), std::invalid_argument);
    delete_config();
    EXPECT_THROW(cpp_log::log_trace("A trace severity message", __FILE__,
        __LINE__), std::runtime_error);
    EXPECT_THROW(cpp_log::log_debug("A debug severity message", __FILE__,
        __LINE__), std::runtime_error);
    EXPECT_THROW(cpp_log::log_info("An informational severity message",
        __FILE__, __LINE__), std::runtime_error);
    EXPECT_THROW(cpp_log::log_warning("A warning severity message", __FILE__,
        __LINE__), std::runtime_error);
    EXPECT_THROW(cpp_log::log_error("An error severity message", __FILE__,
        __LINE__), std::runtime_error);
    EXPECT_THROW(cpp_log::log_fatal("A fatal severity message", __FILE__,
        __LINE__), std::runtime_error);
}

TEST_F(TestLog, WritesLogMessages) {
    create_config("TRACE");
    cpp_log::initialize();
    cpp_log::log_trace("A trace severity message", __FILE__, __LINE__);
    cpp_log::log_debug("A debug severity message", __FILE__, __LINE__);
    cpp_log::log_info("An informational severity message", __FILE__, __LINE__);
    cpp_log::log_warning("A warning severity message", __FILE__, __LINE__);
    cpp_log::log_error("An error severity message", __FILE__, __LINE__);
    cpp_log::log_fatal("A fatal severity message", __FILE__, __LINE__);
    std::ifstream log_file(LOCAL_LOG_FILE, std::ifstream::in);
    if (!log_file) {
        throw std::runtime_error("Missing log file");
    }
    std::string line;
    std::getline(log_file, line);
    line.erase(0, line.find('<'));
    EXPECT_STREQ(line.c_str(),
        "<trace> [test_log.cpp: 103]: A trace severity message");

    std::getline(log_file, line);
    line.erase(0, line.find('<'));
    EXPECT_STREQ(line.c_str(),
        "<debug> [test_log.cpp: 104]: A debug severity message");

    std::getline(log_file, line);
    line.erase(0, line.find('<'));
    EXPECT_STREQ(line.c_str(),
        "<info> [test_log.cpp: 105]: An informational severity message");

    std::getline(log_file, line);
    line.erase(0, line.find('<'));
    EXPECT_STREQ(line.c_str(),
        "<warning> [test_log.cpp: 106]: A warning severity message");

    std::getline(log_file, line);
    line.erase(0, line.find('<'));
    EXPECT_STREQ(line.c_str(),
        "<error> [test_log.cpp: 107]: An error severity message");

    std::getline(log_file, line);
    line.erase(0, line.find('<'));
    EXPECT_STREQ(line.c_str(),
        "<fatal> [test_log.cpp: 108]: A fatal severity message");
    delete_config();
    delete_log();
}

}  // namespace test_log

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
