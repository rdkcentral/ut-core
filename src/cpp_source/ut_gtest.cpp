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
#include <ut_log.h>
#include <ut_internal.h>

#include <iomanip>

static TestMode_t  gTestMode;
#define _(x) x
typedef enum
{
  UT_STATUS_CONTINUE = 1,   /**< Continue processing commands in current menu. */
  UT_STATUS_MOVE_UP,        /**< Move up to the previous menu. */
  UT_STATUS_STOP            /**< Stop processing (user selected 'Quit'). */
} UT_STATUS;

struct TestSuiteInfo {
    int number;
    std::string name;
};

class UTTestRunner
{
public:
    explicit UTTestRunner()
    {
        int argc = 1;
        char *argv[1] = {(char *)"test_runner"};
        ::testing::InitGoogleTest(&argc, argv);
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

    std::vector<TestSuiteInfo> listTestSuites()
    {
        const ::testing::UnitTest &unit_test = *::testing::UnitTest::GetInstance();

        std::vector<TestSuiteInfo> suites;
        std::cout << "\n"
                      << _("--------------------- Registered Suites -----------------------------") << "\n"
                      << std::flush;
        std::cout << std::setw(1) << "#" << "  "  // Right-aligned
              << std::left << std::setw(20) << _("Suite Name") // Left-aligned
              << std::endl;
        for (int i = 0; i < unit_test.total_test_suite_count(); ++i)
        {
            const ::testing::TestSuite *test_suite = unit_test.GetTestSuite(i);
            suites.push_back({i + 1, test_suite->name()});
            std::cout << i + 1 << ". " << test_suite->name() << "\n";
        }
        std::cout  << _("---------------------------------------------------------------------") << "\n"
                   << std::flush;
        std::cout << "\n"
                      <<"Total Number of Suites : "<< unit_test.total_test_suite_count() << "\n";
        return suites;
    }

    std::string getUserSelectedTestSuites(const std::vector<TestSuiteInfo>& suites)
    {
        std::cout << "\nEnter number of suite to select (1-" << suites.size() << ") : ";
        int number;
        std::cin >> number;

        if (std::cin.fail() || number <= 0 || number > static_cast<int>(suites.size()))
        {
            // Clear the error state and ignore the invalid input
            std::cin.clear();                                                   // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore invalid input
            return "";
        }

        // Return the selected test suite's filter
        return suites[number - 1].name + ".*";
    }

    void printUsage()
    {
        std::cout << "\n\n"
                      << _("Commands:  R - Run all tests in suite") << "\n"
                      << _("           S - Select a suite to run or modify") << "\n"
                      << _("           L - List all registered suites") << "\n"
                      << _("           H - Show this help message") << "\n"
                      << _("           Q - Quit the application") << "\n"
                      << _("           A - Activate - implementation pending") << "\n"
                      << _("           O - Option - implementation pending") << "\n"
                      << _("           F - Failures - implementation pending") << "\n"
                      << std::flush;

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
    UT_STATUS eStatus = UT_STATUS_CONTINUE;

    UTTestRunner testRunner;

    if (UT_get_test_mode() == UT_MODE_CONSOLE)
    {
        while (eStatus == UT_STATUS_CONTINUE)
        {
            std::cout << "\n\n"
                      << _("***************** UT CORE CONSOLE - MAIN MENU ******************************") << "\n"
                      << _("(R)un  (S)elect  (L)ist  (A)ctivate  (F)ailures  (O)ptions  (H)elp  (Q)uit") << "\n"
                      << _("Enter command: ")
                      << std::flush; // Ensures the buffer is flushed immediately

            char choice = '\0';
            std::cin >> choice; // Read the user input
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the buffer

            choice = std::toupper(choice); // Convert input to uppercase for consistency

            if (choice == _("L")[0])
            {
                testRunner.listTestSuites();
            }
            else if (choice == _("S")[0])
            {
                auto suites = testRunner.listTestSuites();
                std::string selected_suites = testRunner.getUserSelectedTestSuites(suites);

                if (!selected_suites.empty())
                {
                    std::cout << "Suite '" << selected_suites << "' selected.\n";
                    // Set the GTest filter
                    ::testing::GTEST_FLAG(filter) = selected_suites;
                }
                else
                {
                    std::cout << "\nTest not found.\n";
                }
            }
            else if (choice == _("R")[0])
            {
                testRunner.runTests();
            }
            else if (choice == _("Q")[0])
            {
                eStatus = UT_STATUS_STOP;
            }
            else if ((choice == _("H")[0]) || (choice == _("?")[0]))
            {
                testRunner.printUsage();
            }
            else if ((choice == _("A")[0]) || (choice == _("F")[0]) || (choice == _("O")[0]))
            {
                std::cout << "To be implemented soon\n" << std::flush;
            }
        }
    }
    else
    {
        testRunner.runTests();
    }

    UT_LOG( UT_LOG_ASCII_GREEN "Logfile" UT_LOG_ASCII_NC ":[" UT_LOG_ASCII_YELLOW "%s" UT_LOG_ASCII_NC "]\n", UT_log_getLogFilename() );
    UT_exit();

    return UT_STATUS_OK;
}