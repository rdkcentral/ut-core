/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2023 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include <ut.h>
#include <ut_internal.h>

static TestMode_t  gTestMode;
class UTTestRunner
{
public:
    explicit UTTestRunner()
    {
        int argc = 1;
        char *argv[1] = {(char *)"test_runner"};
        ::testing::InitGoogleTest(&argc, argv);
        std::cout <<"\n***************** UT-CORE CONSOLE - MAIN MENU ******************************\n";
    }

    void setTestFilter(const std::string &filter)
    {
        ::testing::GTEST_FLAG(filter) = filter;
    }

    void setGTestFlag(const std::string &flagName, const std::string &value)
    {
        if (flagName == "filter")
        {
            setTestFilter(value);
        }
        // Add more flags as needed
    }

    int runTests() const
    {
        return RUN_ALL_TESTS();
    }

    int runTestsWithCustomSetup(std::function<void()> setup = nullptr) const
    {
        if (setup)
        {
            setup();
        }
        return RUN_ALL_TESTS();
    }

    void listTestSuites()
    {
        const ::testing::UnitTest &unit_test = *::testing::UnitTest::GetInstance();

        std::cout << "--------------------- Registered Suites -----------------------------\n";
        for (int i = 0; i < unit_test.total_test_suite_count(); ++i)
        {
            const ::testing::TestSuite *test_suite = unit_test.GetTestSuite(i);
            std::cout << test_suite->name() << ":\n";
            // for (int j = 0; j < test_suite->total_test_count(); ++j)
            // {
            //     const ::testing::TestInfo *test_info = test_suite->GetTestInfo(j);
            //     std::cout << "  " << test_info->name() << "\n";
            // }
        }
    }

    std::string getUserSelectedTestSuites()
    {
        std::cout << "\nEnter the test suite names to run (separate by spaces): ";
        std::string input;
        std::string output;
        std::getline(std::cin, input);

        // Replace spaces with `:`, as Google Test uses `:` to combine multiple filters
        for (auto &ch : input)
        {
            if (ch == ' ')
            {
                output += ".*:"; // Replace space with ".*:"
            }
            else
            {
                output += ch; // Keep other characters as is
            }
        }

        return output + ".*"; // Append wildcard to select all tests in the suite(s)
    }

};

void UT_set_results_output_filename(const char* szFilenameRoot)
{
    std::string filepath = szFilenameRoot;

    // Find the position of the last dot
    size_t lastDot = filepath.find_last_of('.');

    // Strip the extension if found
    if (lastDot != std::string::npos)
    {
        filepath = filepath.substr(0, lastDot);
    }

    // Set the output format and path programmatically
    ::testing::FLAGS_gtest_output = std::string("xml:") + filepath + "-report.xml";
}

void UT_set_test_mode(TestMode_t  mode)
{
    gTestMode = mode;
    return;
}

TestMode_t UT_get_test_mode()
{
    return gTestMode;
}

void UT_Manage_Suite_Activation(int groupID, bool enable_disable)
{
    return;
}

void UT_toggle_suite_activation_based_on_groupID(UT_groupID_t groupId, bool enable_disable)
{
    return;
}

UT_status_t startup_system( void )
{
    return UT_STATUS_OK;
}

UT_status_t UT_run_tests()
{
    UTTestRunner testRunner;
    if (UT_get_test_mode() == UT_MODE_CONSOLE)
    {
        testRunner.listTestSuites();
        std::string selected_suites = testRunner.getUserSelectedTestSuites();
        if (selected_suites != ".*")
        {
            testRunner.setTestFilter(selected_suites);
            // testRunner.setTestFilter("UTKVPProfileTestL1.*:SampleTestSuite.*");
        }
    }
    testRunner.runTests();
    return UT_STATUS_OK;
}