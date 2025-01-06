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
#define STRING_FORMAT(x) x
typedef enum
{
  UT_STATUS_CONTINUE = 1,   /**< Continue processing commands in current menu. */
  UT_STATUS_MOVE_UP,        /**< Move up to the previous menu. */
  UT_STATUS_STOP            /**< Stop processing (user selected 'Quit'). */
} UT_STATUS;

// Struct to hold test information within a suite
struct TestInfo {
    int number;
    std::string name;
    bool isActive;
};

struct TestSuiteInfo {
    int number;
    std::string name;
    bool isActive;
    std::vector<TestInfo> tests;
};

class UTTestRunner
{

private:
    static std::vector<TestSuiteInfo> suites;

public:
    explicit UTTestRunner()
    {
        int argc = 1;
        char *argv[1] = {(char *)"test_runner"};
        ::testing::InitGoogleTest(&argc, argv);
        const ::testing::UnitTest &unit_test = *::testing::UnitTest::GetInstance();

        for (int i = 0; i < unit_test.total_test_suite_count(); ++i)
        {
            const ::testing::TestSuite *test_suite = unit_test.GetTestSuite(i);
            std::vector<TestInfo> testInfos;

            for (int j = 0; j < test_suite->total_test_count(); ++j)
            {
                const ::testing::TestInfo *test_info = test_suite->GetTestInfo(j);
                testInfos.push_back({j + 1, test_info->name(), true});
            }

            suites.push_back({i + 1, test_suite->name(), true, testInfos});
        }
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

        std::vector<TestSuiteInfo> &suites = UTTestRunner::suites;
        std::string filter = ::testing::GTEST_FLAG(filter);

        std::cout << "\n"
                      << STRING_FORMAT("--------------------- Registered Suites -----------------------------") << "\n"
                      << std::flush;
        std::cout << std::setw(1) << "#" << "  "  // Right-aligned
              << std::left << std::setw(50) << STRING_FORMAT("Suite Name") // Left-aligned
              << std::left << std::setw(10) << STRING_FORMAT("#Tests") // Left-aligned
              << std::left << std::setw(10) << STRING_FORMAT("Active?") // Left-aligned
              << std::endl << "\n";
        for (size_t i = 0; i < suites.size(); ++i)
        {

            std::cout << std::setw(1) << i + 1 << ". "
                      << std::left << std::setw(50) << suites[i].name
                      << std::left << std::setw(10) << suites[i].tests.size()
                      << std::left << std::setw(10) << (suites[i].isActive ? "Yes" : "No")
                      << "\n";
        }
        std::cout  << STRING_FORMAT("---------------------------------------------------------------------") << "\n"
                   << std::flush;
        std::cout << "\n"
                      <<"Total Number of Suites : "<< unit_test.total_test_suite_count() << "\n";
        return suites;
    }

    int getUserSelectedTestSuites(const std::vector<TestSuiteInfo>& suites)
    {
        std::cout << "\nEnter number of suite to select (1-" << suites.size() << ") : ";
        int number;
        std::cin >> number;

        if (std::cin.fail() || number <= 0 || number > static_cast<int>(suites.size()))
        {
            // Clear the error state and ignore the invalid input
            std::cin.clear();                                                   // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore invalid input
            return 0;
        }

        return number;
    }

    std::string getUserSelectedTest(std::vector<TestInfo> tests, const TestSuiteInfo &suite)
    {
        std::cout << "\nEnter number of test to select (1-" << tests.size() << ") : ";
        int test_number;
        std::cin >> test_number;

        if (std::cin.fail() || test_number <= 0 || test_number > static_cast<int>(tests.size()))
        {
            // Clear the error state and ignore the invalid input
            std::cin.clear();                                                   // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore invalid
            std::cout << "\n" << "Test not found.\n";
            return "";
        }

        return suite.name + "." + tests[test_number - 1].name; // Construct the filter
    }

    std::vector<TestInfo> listTestsFromSuite(const TestSuiteInfo &suite)
    {

        std::cout << "\n"
                  << STRING_FORMAT("---------------------------Test List ------------------------------") << "\n"
                  << std::flush;
        std::cout << "Suite: "<< suite.name << "\n\n";
        std::cout << std::setw(1) << "#" << "  "  // Right-aligned
              << std::left << std::setw(50) << STRING_FORMAT("Test Name") // Left-aligned
              << std::left << std::setw(10) << STRING_FORMAT("Active?") // Left-aligned
              << std::endl << "\n";

        for (size_t i = 0; i < suite.tests.size(); ++i)
        {
            std::cout << std::setw(1) << suite.tests[i].number << ". "
            << std::left << std::setw(50) << suite.tests[i].name
            << std::left << std::setw(10) << (suite.tests[i].isActive ? "Yes" : "No")
            << "\n";
        }

        std::cout  << STRING_FORMAT("-------------------------------------------------------------------") << "\n"
                   << std::flush;
        std::cout << "\n"
                      <<"Total Number of Tests : "<< suite.tests.size() << "\n";

        return suite.tests;
    }

    void ToggleTestSuiteExclusion(int suite_num)
    {
        // Get the current filter
        std::string &filter = ::testing::GTEST_FLAG(filter);
        std::vector<TestSuiteInfo> &suites = UTTestRunner::suites;

        // Toggle the suite's active status
        suites[suite_num - 1].isActive = !suites[suite_num - 1].isActive;

        // Check if all suites are inactive
        bool all_inactive = std::all_of(suites.begin(), suites.end(), [](const TestSuiteInfo &suite)
                                        { return !suite.isActive; });

        // Rebuild the filter
        if (all_inactive)
        {
            // Exclude all suites
            filter = "-*";
        }
        else
        {
            // Build the include and exclude filters
            std::string include_filter;
            std::string exclude_filter;

            for (const auto &suite : suites)
            {
                if (suite.isActive)
                {
                    if (!include_filter.empty())
                        include_filter += ":";
                    include_filter += suite.name + ".*";
                }
                else
                {
                    if (!exclude_filter.empty())
                        exclude_filter += ":";
                    exclude_filter += "-" + suite.name + ".*";
                }
            }

            // Combine include and exclude filters
            if (!include_filter.empty())
            {
                filter = include_filter;
                if (!exclude_filter.empty())
                {
                    filter += ":" + exclude_filter;
                }
            }
            else
            {
                filter = exclude_filter;
            }
        }

        // Provide feedback to the user
        std::cout << "Updated GTest filter: " << filter << "\n";
    }

    // Function to list all tests in a specific suite and allow the user to select one
    UT_STATUS selectTestFromSuite(const TestSuiteInfo &suite)
    {
        UT_STATUS eStatus = UT_STATUS_CONTINUE;
        const ::testing::UnitTest &unit_test = *::testing::UnitTest::GetInstance();
        const ::testing::TestSuite *test_suite = unit_test.GetTestSuite(suite.number - 1);

        if (!test_suite)
        {
            std::cerr << "Invalid test suite selection.\n";
            return UT_STATUS_CONTINUE;
        }

        while (eStatus == UT_STATUS_CONTINUE)
        {
            std::cout << "\n\n"
                      << STRING_FORMAT("***************** UT CORE CONSOLE - SUITE MENU ******************************") << "\n"
                      << STRING_FORMAT("(R)un  (S)elect  (L)ist  (A)ctivate  (F)ailures  (O)ptions  (H)elp  (Q)uit") << "\n"
                      << STRING_FORMAT("Enter command: ")
                      << std::flush; // Ensures the buffer is flushed immediately

            char choice = '\0';
            std::cin >> choice; // Read the user input
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the buffer

            choice = std::toupper(choice); // Convert input to uppercase for consistency

            if (choice == STRING_FORMAT("L")[0])
            {
                listTestsFromSuite(suite);
            }
            else if (choice == STRING_FORMAT("S")[0])
            {
                std::vector<TestInfo> tests = listTestsFromSuite(suite);
                std::string testName = getUserSelectedTest(tests, suite);
                if (testName.empty())
                { // Handle empty test name
                    continue; // Return to the menu without changing eStatus
                }
                ::testing::GTEST_FLAG(filter) = testName;
                runTests();
            }
            else if (choice == STRING_FORMAT("R")[0])
            {
                runTests();
            }
            else if (choice == STRING_FORMAT("Q")[0])
            {
                eStatus = UT_STATUS_STOP;
            }
            else if (choice == STRING_FORMAT("M")[0])
            {
                eStatus = UT_STATUS_MOVE_UP;
            }
            else if ((choice == STRING_FORMAT("H")[0]) || (choice == STRING_FORMAT("?")[0]))
            {
                printUsageForSuite(suite);
            }
            else if ((choice == STRING_FORMAT("A")[0]))
            {
                std::vector<TestInfo> tests = listTestsFromSuite(suite);
                std::string testName = getUserSelectedTest(tests, suite);
                if (testName.empty())
                { // Handle empty test name
                    continue; // Return to the menu without changing eStatus
                }
                ::testing::GTEST_FLAG(filter) = "-" + testName;
            }
            else if ((choice == STRING_FORMAT("F")[0]) || (choice == STRING_FORMAT("O")[0]))
            {
                std::cout << "To be implemented soon\n" << std::flush;
            }
        }
        return eStatus;
    }

    void printUsage()
    {
        std::cout << "\n\n"
                      << STRING_FORMAT("Commands:  R - Run tests in suite") << "\n"
                      << STRING_FORMAT("           S - Select a suite to run") << "\n"
                      << STRING_FORMAT("           L - List all registered suites") << "\n"
                      << STRING_FORMAT("           H - Show this help message") << "\n"
                      << STRING_FORMAT("           Q - Quit the application") << "\n"
                      << STRING_FORMAT("           A - Activate - implementation pending") << "\n"
                      << STRING_FORMAT("           O - Option - implementation pending") << "\n"
                      << STRING_FORMAT("           F - Failures - implementation pending") << "\n"
                      << std::flush;

    }

    void printUsageForSuite(const TestSuiteInfo &suite)
    {
        std::cout << "\n\n"
                      << STRING_FORMAT("Commands:  R - Run tests in suite: ") << suite.name << "\n"
                      << STRING_FORMAT("           S - Select a test to run ") << "\n"
                      << STRING_FORMAT("           L - List all registered tests in suite: ") << suite.name << "\n"
                      << STRING_FORMAT("           M - Move up to the main menu") << "\n"
                      << STRING_FORMAT("           H - Show this help message") << "\n"
                      << STRING_FORMAT("           Q - Quit the application") << "\n"
                      << STRING_FORMAT("           A - Activate - implementation pending") << "\n"
                      << STRING_FORMAT("           O - Option - implementation pending") << "\n"
                      << STRING_FORMAT("           F - Failures - implementation pending") << "\n"
                      << std::flush;

    }
};

std::vector<TestSuiteInfo> UTTestRunner::suites;

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
    std::cout << "Listing Filename:[" << filepath << "-report.xml]\n" << std::flush;
    std::cout << "Results Filename:[" << filepath << ".log]\n" << std::flush;
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
                      << STRING_FORMAT("***************** UT CORE CONSOLE - MAIN MENU ******************************") << "\n"
                      << STRING_FORMAT("(R)un  (S)elect  (L)ist  (A)ctivate  (F)ailures  (O)ptions  (H)elp  (Q)uit") << "\n"
                      << STRING_FORMAT("Enter command: ")
                      << std::flush; // Ensures the buffer is flushed immediately

            char choice = '\0';
            std::cin >> choice; // Read the user input
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the buffer

            choice = std::toupper(choice); // Convert input to uppercase for consistency

            if (choice == STRING_FORMAT("L")[0])
            {
                testRunner.listTestSuites();
            }
            else if (choice == STRING_FORMAT("S")[0])
            {
                auto suites = testRunner.listTestSuites();
                int selected_suites = testRunner.getUserSelectedTestSuites(suites);

                if (selected_suites)
                {
                    std::cout << "Suite '" << selected_suites << "' selected.\n";
                    // Set the GTest filter
                    ::testing::GTEST_FLAG(filter) = suites[selected_suites - 1].name + ".*";
                    // Select a test from the chosen suite
                    eStatus = testRunner.selectTestFromSuite(suites[selected_suites - 1]);
                    if (eStatus == UT_STATUS_MOVE_UP)
                    {
                        ::testing::GTEST_FLAG(filter) = "*"; //Reset the filter back to all
                        eStatus = UT_STATUS_CONTINUE;
                    }
                }
                else
                {
                    std::cout << "\nTest not found.\n";
                }
            }
            else if (choice == STRING_FORMAT("R")[0])
            {
                testRunner.runTests();
            }
            else if (choice == STRING_FORMAT("Q")[0])
            {
                eStatus = UT_STATUS_STOP;
            }
            else if ((choice == STRING_FORMAT("H")[0]) || (choice == STRING_FORMAT("?")[0]))
            {
                testRunner.printUsage();
            }
            else if ((choice == STRING_FORMAT("A")[0]))
            {
                auto suites = testRunner.listTestSuites();
                int selected_suites = testRunner.getUserSelectedTestSuites(suites);
                //::testing::GTEST_FLAG(filter) = "-" + suites[selected_suites - 1].name + ".*";
                testRunner.ToggleTestSuiteExclusion(selected_suites);
                testRunner.listTestSuites();
            }
            else if ((choice == STRING_FORMAT("F")[0]) || (choice == STRING_FORMAT("O")[0]))
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