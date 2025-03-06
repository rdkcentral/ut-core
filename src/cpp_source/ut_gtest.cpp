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
#include <regex>
#include <algorithm>

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

// Initialize static variables
std::unordered_map<std::string, UT_groupID_t> UTCore::suiteToGroup;

class UTTestRunner
{

private:
    static std::vector<TestSuiteInfo> suites;

public:
    static std::unordered_set<UT_groupID_t> enabledGroups;
    static std::unordered_set<UT_groupID_t> disabledGroups;

    /**
     * @brief Constructs a UTTestRunner object and initializes Google Test framework.
     *
     * This constructor initializes the Google Test framework with a single argument
     * "test_runner". It retrieves the test filter string from UTCore, parses it into
     * active and inactive filters, and processes each test suite accordingly.
     *
     * The constructor performs the following steps:
     * 1. Initializes Google Test with a single argument.
     * 2. Retrieves the test filter string from UTCore.
     * 3. Parses the filter string into active and inactive filters.
     * 4. Checks if both filters are empty.
     * 5. Iterates through all test suites and determines their active status based on the filters.
     * 6. Collects test information for each test suite and stores it in the `suites` vector.
     * 7. Formats the inactive filter string and sets it as the test filter.
     *
     * The `suites` vector contains information about each test suite, including its
     * index, name, active status, and a list of tests with their respective indices,
     * names, and active statuses.
     */
    explicit UTTestRunner()
    {
        int argc = 1;
        char *argv[1] = {(char *)"test_runner"};
        ::testing::InitGoogleTest(&argc, argv);
        const ::testing::UnitTest &unit_test = *::testing::UnitTest::GetInstance();
        std::string filter = UTCore::UT_get_test_filter();
        std::vector<std::string> activeFilters;
        std::vector<std::string> inactiveFilters;
        parseFilterString(filter, activeFilters, inactiveFilters);

        // Check if both filters are empty
        bool noFilters = activeFilters.empty() && inactiveFilters.empty();

        for (int i = 0; i < unit_test.total_test_suite_count(); ++i)
        {
            const ::testing::TestSuite *test_suite = unit_test.GetTestSuite(i);
            std::string suiteName = test_suite->name();

            // If no filters are provided, consider all suites as active
            bool isActive = noFilters || (!matchesAnyPattern(suiteName, activeFilters) && !matchesAnyPattern(suiteName, inactiveFilters)) ||
                            (matchesAnyPattern(suiteName, activeFilters) && !matchesAnyPattern(suiteName, inactiveFilters));

            std::vector<TestInfo> testInfos;
            testInfos.reserve(test_suite->total_test_count());

            for (int j = 0; j < test_suite->total_test_count(); ++j)
            {
                const ::testing::TestInfo *test_info = test_suite->GetTestInfo(j);
                bool testIsActive = isActive;
                testInfos.push_back(TestInfo{j + 1, test_info->name(), testIsActive});
            }

            suites.push_back(TestSuiteInfo{i + 1, suiteName, isActive, std::move(testInfos)});
        }

        std::string inactiveFilterString = formatPatterns(inactiveFilters);
        setTestFilter(inactiveFilterString);
    }

    /**
     * @brief Formats a list of patterns into a single string with a specific format.
     *
     * This function takes a vector of strings (patterns) and concatenates them into a single
     * string. Each pattern is separated by ":*". If a pattern starts with a '-', it is removed.
     * The resulting string starts with a leading '-'.
     *
     * @param patterns A vector of strings representing the patterns to be formatted.
     * @return A formatted string with each pattern separated by ":*".
     */
    std::string formatPatterns(const std::vector<std::string> &patterns)
    {
        std::string result = "-"; // Leading '-'

        for (size_t i = 0; i < patterns.size(); ++i)
        {
            std::string modifiedPattern = patterns[i];

            // Remove leading '-' if present
            if (!modifiedPattern.empty() && modifiedPattern[0] == '-')
            {
                modifiedPattern = modifiedPattern.substr(1);
            }

            // Append to result with separator ":*" (skip separator for the last item)
            result += modifiedPattern;
            if (i < patterns.size() - 1)
            {
                result += ":*";
            }
        }

        return result;
    }

    // Function to split a string by a delimiter
    /**
     * @brief Splits a given string into a vector of substrings based on a specified delimiter.
     *
     * This function takes a string and a delimiter character, and splits the string into
     * substrings wherever the delimiter character is found. The substrings are stored in
     * a vector and returned.
     *
     * @param str The input string to be split.
     * @param delimiter The character used to split the string.
     * @return std::vector<std::string> A vector containing the substrings obtained by splitting the input string.
     */
    std::vector<std::string> split(const std::string &str, char delimiter)
    {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, delimiter))
        {
            tokens.push_back(token);
        }
        return tokens;
    }

    // Function to check if a string matches any regex pattern in a vector
    /**
     * @brief Checks if a given string matches any pattern in a list of patterns.
     *
     * This function iterates through a list of patterns and checks if the given string
     * matches any of the patterns using regular expressions. If a pattern starts with
     * a '-', it is removed before performing the match.
     *
     * @param str The string to be checked against the patterns.
     * @param patterns A vector of patterns to match against the string.
     * @return true if the string matches any of the patterns, false otherwise.
     */
    bool matchesAnyPattern(const std::string &str, const std::vector<std::string> &patterns)
    {
        for (const auto &pattern : patterns)
        {
            std::string modifiedPattern = pattern;

            // Remove leading '-' if present
            if (!modifiedPattern.empty() && modifiedPattern[0] == '-')
            {
                modifiedPattern = modifiedPattern.substr(1);
            }

            std::regex regexPattern(modifiedPattern);
            if (std::regex_search(str, regexPattern))
            {
                return true;
            }
        }
        return false;
    }

    // Function to parse the filter string into active and inactive filters
    /**
     * @brief Parses a filter string and separates it into active and inactive filters.
     *
     * This function takes a filter string, splits it by the ':' character, and categorizes
     * each filter as either active or inactive. Filters that start with a '-' character
     * are considered inactive, while all other filters are considered active. The special
     * filter "*" is ignored.
     *
     * @param filter The filter string to be parsed.
     * @param activeFilters A vector to store the active filters.
     * @param inactiveFilters A vector to store the inactive filters.
     */
    void parseFilterString(const std::string &filter, std::vector<std::string> &activeFilters, std::vector<std::string> &inactiveFilters)
    {
        auto filters = split(filter, ':');
        for (const auto &f : filters)
        {
            if (f == "*")
            {
                continue; // Skip the default filter
            }

            if (f.front() == '-')
            {
                inactiveFilters.push_back(f);
            }
            else
            {
                activeFilters.push_back(f);
            }
        }
    }

    /**
     * @brief Sets the Google Test filter for test execution.
     *
     * This function sets the filter for Google Test to determine which tests
     * should be run. The filter is a string that can include positive and
     * negative patterns separated by colons (e.g., "FooTest.*:BarTest.*-BazTest.*").
     *
     * @param filter A string representing the test filter pattern.
     */
    void setTestFilter(const std::string &filter)
    {
        ::testing::GTEST_FLAG(filter) = filter;
    }

    /**
     * @brief Sets a Google Test flag to a specified value.
     *
     * This function allows setting various Google Test flags by specifying the flag name and its corresponding value.
     * Currently, it supports setting the "filter" flag, which is used to filter test cases.
     *
     * @param flagName The name of the Google Test flag to set (e.g., "filter").
     * @param value The value to set for the specified flag.
     */
    void setGTestFlag(const std::string &flagName, const std::string &value)
    {
        if (flagName == "filter")
        {
            setTestFilter(value);
        }
        // Add more flags as needed
    }

    /**
     * @brief Runs all the Google Test unit tests.
     *
     * This function executes all the tests that have been defined using the
     * Google Test framework. It returns the result of the test execution.
     *
     * @return int The result of running all tests. Typically, it returns 0 if
     * all tests pass, and a non-zero value if any test fails.
     */
    int runTests() const
    {
        return RUN_ALL_TESTS();
    }

    /**
     * @brief Runs all tests with an optional custom setup function.
     *
     * This function allows you to run all tests, optionally providing a custom setup function
     * that will be executed before the tests are run. If no setup function is provided, the tests
     * will be run without any additional setup.
     *
     * @param setup A std::function<void()> representing the custom setup function to be executed
     *              before running the tests. If nullptr, no setup function will be executed.
     * @return An integer representing the result of RUN_ALL_TESTS().
     */
    int runTestsWithCustomSetup(std::function<void()> setup = nullptr) const
    {
        if (setup)
        {
            setup();
        }
        return RUN_ALL_TESTS();
    }

    /**
     * @brief Lists all registered test suites and prints their details.
     *
     * This function retrieves all registered test suites from the Google Test framework,
     * prints their details in a formatted manner, and returns a vector of TestSuiteInfo objects.
     * The details printed include the suite name, the number of tests in each suite, and whether
     * the suite is active or not.
     *
     * @return std::vector<TestSuiteInfo> A vector containing information about all registered test suites.
     */
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

    /**
     * @brief Prompts the user to select a test suite from a list of available test suites.
     *
     * This function displays a prompt to the user to enter the number corresponding to a test suite
     * from the provided list of test suites. It validates the user input to ensure it is within the
     * valid range and returns the selected test suite number. If the input is invalid, it clears the
     * error state and ignores the invalid input, returning 0.
     *
     * @param suites A vector of TestSuiteInfo objects representing the available test suites.
     * @return The number of the selected test suite if the input is valid, otherwise 0.
     */
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

    /**
     * @brief Prompts the user to select a test from a list and optionally toggles its activation status.
     *
     * This function displays a list of tests and prompts the user to enter the number corresponding to the test they want to select.
     * If the input is invalid or out of range, an error message is displayed and an empty string is returned.
     * If the `isActivate` flag is true, the function will toggle the activation status of the selected test within the provided test suite.
     *
     * @param tests A vector of TestInfo objects representing the available tests.
     * @param suite A reference to a TestSuiteInfo object representing the test suite containing the tests.
     * @param isActivate A boolean flag indicating whether to toggle the activation status of the selected test.
     * @return A string representing the fully qualified name of the selected test in the format "suite_name.test_name",
     * or an empty string if the selection was invalid.
     */
    std::string getUserSelectedTest(std::vector<TestInfo> tests, TestSuiteInfo &suite, bool isActivate)
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

        if (isActivate)
        {
            if (suite.isActive == false)
            {
                std::cout << "\n" << suite.name << " is inactive.\n";
            }
            else
            {
                suite.tests[test_number - 1].isActive = !suite.tests[test_number - 1].isActive;
            }
        }

        return suite.name + "." + tests[test_number - 1].name; // Construct the filter
    }

    /**
     * @brief Lists all tests from a given test suite and prints them in a formatted manner.
     *
     * This function takes a TestSuiteInfo object as input, which contains information about a test suite
     * and its associated tests. It prints the suite name and details of each test in a formatted table.
     * The details include the test number, test name, and whether the test is active or not.
     * Finally, it prints the total number of tests in the suite.
     *
     * @param suite A constant reference to a TestSuiteInfo object containing the test suite information.
     * @return A vector of TestInfo objects representing the tests in the suite.
     */
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

    /**
     * @brief Toggles the active status of a test suite and updates the GTest filter accordingly.
     *
     * This function toggles the active status of the specified test suite by its number.
     * It then updates the GTest filter to include or exclude tests based on the active status
     * of all test suites. If all test suites are inactive, the filter will exclude all tests.
     *
     * @param suite_num The number of the test suite to toggle (1-based index).
     */
    void ToggleTestSuiteExclusion(int suite_num)
    {
        // Get the current filter
        std::string &filter = ::testing::GTEST_FLAG(filter);
        std::vector<TestSuiteInfo> &suites = UTTestRunner::suites;

        // Toggle the suite's active status
        TestSuiteInfo &suite = suites[suite_num - 1];
        suite.isActive = !suite.isActive;

        // Reflect the change in the tests' active statuses
        for (auto &test : suite.tests)
        {
            test.isActive = suite.isActive;
        }

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

    /**
     * @brief Sets the GTest filter flag based on the active status of tests in the given test suite.
     *
     * This function updates the GTest filter flag to include only the active tests in the specified
     * test suite. If all tests in the suite are active, the filter is set to include all tests in the
     * suite. If some tests are active, the filter is constructed to include only the active tests.
     * If no tests are active, the filter is set to exclude all tests.
     *
     * @param suite The test suite containing the tests to be filtered.
     */
    void setFlag(const TestSuiteInfo &suite)
    {
        // Get the current filter
        std::string &filter = ::testing::GTEST_FLAG(filter);

        // Check the active status of all tests in the suite
        bool allActive = true;
        bool anyActive = false;

        std::string filterString;

        for (const auto &test : suite.tests)
        {
            if (test.isActive)
            {
                anyActive = true;
                filterString += suite.name + "." + test.name + ":";
            }
            else
            {
                allActive = false;
            }
        }

        if (allActive)
        {
            // If all tests are active, use the default filter to execute all tests
            filter = suite.name + "*";
        }
        else if (anyActive)
        {
            // If some tests are active, construct a filter for the active ones
            // Remove the trailing colon (":") from the filter string
            if (!filterString.empty() && filterString.back() == ':')
            {
                filterString.pop_back();
            }
            filter = filterString;
        }
        else
        {
            // If no tests are active, set the filter to exclude everything
            filter = "-*";
        }
    }

    // Function to list all tests in a specific suite and allow the user to select one
    /**
     * @brief Handles the selection and execution of tests from a given test suite.
     *
     * This function provides a console-based menu for interacting with a specified test suite.
     * Users can list tests, select and run specific tests, activate tests, display failures,
     * move up in the menu hierarchy, display help, and quit the menu.
     *
     * @param suite Reference to the TestSuiteInfo object representing the test suite.
     * @return UT_STATUS indicating the status of the operation (e.g., continue, stop, move up).
     *
     * The menu options include:
     * - (L)ist: List all tests in the suite.
     * - (S)elect: Select and run a specific test from the suite.
     * - (R)un: Run all tests in the suite.
     * - (Q)uit: Exit the menu.
     * - (M)oveUp: Move up in the menu hierarchy.
     * - (H)elp: Display help information for the suite.
     * - (A)ctivate: Activate a specific test in the suite.
     * - (F)ailures: Display failed tests.
     * - (O)ptions: Display options menu.
     *
     * The function ensures that user input is validated and handles invalid input gracefully.
     */
    UT_STATUS selectTestFromSuite(TestSuiteInfo &suite)
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
                      << STRING_FORMAT("*********************** UT CORE CONSOLE - SUITE MENU ******************************") << "\n"
                      << STRING_FORMAT("(R)un  (S)elect  (L)ist  (A)ctivate  (F)ailures  (M)oveUp  (H)elp (O)ptions (Q)uit") << "\n"
                      << STRING_FORMAT("Enter command: ")
                      << std::flush; // Ensures the buffer is flushed immediately

            char choice = '\0';
            std::cin >> choice;                                                 // Read the user input
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the buffer

            // Check if the input was valid
            if (std::cin.fail())
            {
                std::cin.clear(); // Clear the error state
                std::cerr << "Invalid input. Please enter a single character.\n";
                continue; // Retry
            }

            choice = std::toupper(choice); // Convert input to uppercase for consistency

            if (choice == STRING_FORMAT("L")[0])
            {
                listTestsFromSuite(suite);
            }
            else if (choice == STRING_FORMAT("S")[0])
            {
                std::vector<TestInfo> tests = listTestsFromSuite(suite);
                std::string testName = getUserSelectedTest(tests, suite, false);
                if (testName.empty())
                {             // Handle empty test name
                    continue; // Return to the menu without changing eStatus
                }

                // Extract the second part of the testName
                std::string targetTestName = testName.substr(testName.find('.') + 1);

                // Make sure the test is active before executing it
                for (const auto &test : tests)
                {
                    if (test.name == targetTestName && test.isActive)
                    {
                        ::testing::GTEST_FLAG(filter) = testName;
                        runTests();
                        break; // Exit the loop once the test is found and executed
                    }
                }
            }
            else if (choice == STRING_FORMAT("R")[0])
            {
                setFlag(suite);
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
            else if (choice == STRING_FORMAT("H")[0])
            {
                printUsageForSuite(suite);
            }
            else if (choice == STRING_FORMAT("A")[0])
            {
                std::vector<TestInfo> tests = listTestsFromSuite(suite);
                std::string testName = getUserSelectedTest(tests, suite, true);
                if (testName.empty())
                { // Handle empty test name
                    continue; // Return to the menu without changing eStatus
                }
                listTestsFromSuite(suite);
            }
            else if (choice == STRING_FORMAT("F")[0])
            {
                displayFailedTests();
            }
            else if ((choice == STRING_FORMAT("O")[0]))
            {
                displayOptionsMenu();
            }
            else
            {
                std::cerr << "Invalid choice. Please enter a valid option (e.g., L).\n";
            }
        }
        return eStatus;
    }

    /**
     * @brief Prints the usage instructions for the application.
     *
     * This function outputs a list of commands that the user can use to interact with the application.
     * The commands include:
     * - R: Run tests in suite
     * - S: Select a suite to run
     * - L: List all registered suites
     * - H: Show this help message
     * - Q: Quit the application
     * - A: Activates/De-activates a suite
     * - F: Show failures
     * - O: Show gtest options menu
     */
    void printUsage()
    {
        std::cout << "\n\n"
                      << STRING_FORMAT("Commands:  R - Run tests in suite") << "\n"
                      << STRING_FORMAT("           S - Select a suite to run") << "\n"
                      << STRING_FORMAT("           L - List all registered suites") << "\n"
                      << STRING_FORMAT("           H - Show this help message") << "\n"
                      << STRING_FORMAT("           Q - Quit the application") << "\n"
                      << STRING_FORMAT("           A - Activates/De-activates a suite") << "\n"
                      << STRING_FORMAT("           F - Show failures") << "\n"
                      << STRING_FORMAT("           O - Show gtest options menu") << "\n"
                      << std::flush;

    }

    /**
     * @brief Prints the usage instructions for a given test suite.
     *
     * This function outputs a list of commands and their descriptions to the console,
     * providing guidance on how to interact with the test suite.
     *
     * @param suite The TestSuiteInfo object containing information about the test suite.
     * The name of the suite is used in the printed instructions.
     */
    void printUsageForSuite(const TestSuiteInfo &suite)
    {
        std::cout << "\n\n"
                      << STRING_FORMAT("Commands:  R - Run tests in suite: ") << suite.name << "\n"
                      << STRING_FORMAT("           S - Select a test to run ") << "\n"
                      << STRING_FORMAT("           L - List all registered tests in suite: ") << suite.name << "\n"
                      << STRING_FORMAT("           M - Move up to the main menu") << "\n"
                      << STRING_FORMAT("           H - Show this help message") << "\n"
                      << STRING_FORMAT("           Q - Quit the application") << "\n"
                      << STRING_FORMAT("           A - Activates/De-activates a test") << "\n"
                      << STRING_FORMAT("           F - Show failures") << "\n"
                      << STRING_FORMAT("           O - Show gtest options menu") << "\n"
                      << std::flush;

    }

    /**
     * @brief Displays a list of all failed tests in the current test run.
     *
     * This function iterates through all test suites and their respective tests
     * in the current Google Test instance. It prints the names of the test suites
     * and tests that have failed, along with a count of the total number of failed tests.
     * If no tests have failed, it prints a message indicating that there are no failed tests.
     */
    void displayFailedTests()
    {
        const ::testing::UnitTest &unit_test = *::testing::UnitTest::GetInstance();

        std::cout << "\n"
                  << "------------------------- Failed Tests -------------------------\n";

        int failedTestCount = 0;

        for (int i = 0; i < unit_test.total_test_suite_count(); ++i)
        {
            const ::testing::TestSuite *test_suite = unit_test.GetTestSuite(i);

            for (int j = 0; j < test_suite->total_test_count(); ++j)
            {
                const ::testing::TestInfo *test_info = test_suite->GetTestInfo(j);

                if (test_info->result()->Failed())
                {
                    ++failedTestCount;
                    std::cout << "Suite: " << test_suite->name()
                              << " | Test: " << test_info->name()
                              << " - FAILED\n";
                }
            }
        }

        if (failedTestCount == 0)
        {
            std::cout << "No failed tests.\n";
        }

        std::cout << "----------------------------------------------------------------\n";
    }

    /**
     * @brief Displays a menu for configuring Google Test (GTest) options.
     *
     * This function presents a menu to the user with various options to configure
     * GTest settings such as enabling/disabling test shuffle, setting a random seed,
     * repeating tests multiple times, enabling/disabling break on failure, and setting
     * colored output. The user can select an option by entering the corresponding number
     * or quit the menu by entering 'Q'.
     *
     * Menu options:
     * 1. Enable/Disable test shuffle
     * 2. Set random seed for shuffling
     * 3. Repeat tests multiple times
     * 4. Enable/Disable break on failure
     * 5. Enable/Disable colored output
     * Q. Quit options menu
     *
     * The function runs in a loop, allowing the user to make multiple changes until
     * they choose to quit.
     */
    void displayOptionsMenu()
    {
        while (true)
        {
            std::cout << "\n"
                      << "------------------------- GTest Options -------------------------\n"
                      << "(1) Enable/Disable test shuffle\n"
                      << "(2) Set random seed for shuffling\n"
                      << "(3) Repeat tests multiple times\n"
                      << "(4) Enable/Disable break on failure\n"
                      << "(5) Enable/Disable colored output\n"
                      << "(Q) Quit options menu\n"
                      << "-----------------------------------------------------------------\n"
                      << "Enter your choice: ";

            char choice;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            switch (std::toupper(choice))
            {
            case '1':
            {
                ::testing::GTEST_FLAG(shuffle) = !::testing::GTEST_FLAG(shuffle);
                std::cout << "Shuffle " << (::testing::GTEST_FLAG(shuffle) ? "enabled" : "disabled") << ".\n";
                break;
            }
            case '2':
            {
                std::cout << "Enter random seed (integer): ";
                int seed;
                std::cin >> seed;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                ::testing::GTEST_FLAG(random_seed) = seed;
                std::cout << "Random seed set to: " << seed << "\n";
                break;
            }
            case '3':
            {
                std::cout << "Enter number of repetitions: ";
                int repeat;
                std::cin >> repeat;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                ::testing::GTEST_FLAG(repeat) = repeat;
                std::cout << "Tests will be repeated " << repeat << " times.\n";
                break;
            }
            case '4':
            {
                ::testing::GTEST_FLAG(break_on_failure) = !::testing::GTEST_FLAG(break_on_failure);
                std::cout << "Break on failure " << (::testing::GTEST_FLAG(break_on_failure) ? "enabled" : "disabled") << ".\n";
                break;
            }
            case '5':
            {
                std::cout << "Enter color option (yes/no/auto): ";
                std::string color;
                std::getline(std::cin, color);
                ::testing::GTEST_FLAG(color) = color;
                std::cout << "Color output set to: " << color << "\n";
                break;
            }
            case 'Q':
            {
                return;
            }
            default:
            {
                std::cout << "Invalid choice. Please try again.\n";
                break;
            }
            }
        }
    }

    /**
     * @brief Enables a specified group if it is not already disabled.
     *
     * This function checks if the given group is not present in the set of disabled groups.
     * If the group is not found in the disabled groups, it adds the group to the set of enabled groups.
     *
     * @param group The identifier of the group to be enabled.
     */
    static void enableGroup(UT_groupID_t group)
    {
        if (disabledGroups.find(group) == disabledGroups.end()) // Only enable if not disabled
        {
            enabledGroups.insert(group);
        }
    }

    void displayRunSummary()
    {
        const ::testing::UnitTest &unit_test = *::testing::UnitTest::GetInstance();
        std::vector<TestSuiteInfo> &suites = UTTestRunner::suites;

        int total_suites = unit_test.total_test_suite_count();
        int disabled_suites = 0; // GoogleTest does not track disabled test suites directly

        int total_tests = unit_test.total_test_count();
        int failed_tests = unit_test.failed_test_count();
        int disabled_tests = unit_test.disabled_test_count();
        int skipped_tests = unit_test.skipped_test_count();
        int passed_tests = total_tests - (failed_tests + disabled_tests + skipped_tests);

        int total_asserts = 0;
        int failed_asserts = 0;

        // Iterate through test suites to gather assertion details
        for (int i = 0; i < unit_test.total_test_suite_count(); ++i)
        {
            const ::testing::TestSuite *test_suite = unit_test.GetTestSuite(i);
            if (test_suite)
            {
                total_asserts += test_suite->test_to_run_count();
                failed_asserts += test_suite->failed_test_count();
            }
        }

        int passed_asserts = total_asserts - failed_asserts;

        for (size_t i = 0; i < suites.size(); ++i)
        {
            if (!suites[i].isActive)
            {
                ++disabled_suites;
            }

            for (size_t j = 0; j < suites[i].tests.size(); ++j)
            {
                if (!suites[i].tests[j].isActive)
                {
                    ++disabled_tests;
                }
            }
        }

        std::cout << "\n"
                  << "Run Summary:"
                  << std::setw(10) << "  Type"
                  << std::setw(10) << "Total"
                  << std::setw(10) << "Ran"
                  << std::setw(10) << "Passed"
                  << std::setw(10) << "Failed"
                  << std::setw(10) << "Inactive"
                  << std::setw(10) << "Skipped\n"
                  << std::setw(24) << "Suites"
                  << std::right << std::setw(9) << total_suites
                  << std::right << std::setw(9) << total_suites - disabled_suites
                  << std::right << std::setw(9) << "n/a"
                  << std::right << std::setw(9) << "n/a"
                  << std::right << std::setw(9) << disabled_suites
                  << std::right << std::setw(9) << "n/a" << "\n"
                  << std::setw(23) << "Tests"
                  << std::right << std::setw(10) << total_tests
                  << std::right << std::setw(9) << passed_tests + failed_tests + disabled_tests + skipped_tests
                  << std::right << std::setw(9) << passed_tests
                  << std::right << std::setw(9) << failed_tests
                  << std::right << std::setw(9) << disabled_tests
                  << std::right << std::setw(9) << skipped_tests << "\n"
                  << std::setw(25) << "Asserts"
                  << std::right << std::setw(8) << total_asserts
                  << std::right << std::setw(9) << passed_asserts + failed_asserts
                  << std::right << std::setw(9) << passed_asserts
                  << std::right << std::setw(9) << failed_asserts
                  << std::right << std::setw(9) << "n/a"
                  << std::right << std::setw(9) << "n/a" << "\n"
                  << "\n"
                  << "Elapsed time = " << std::fixed << std::setprecision(3) << unit_test.elapsed_time() / 1000.0 << " seconds\n";
    }

    /**
     * @brief Disables a test group by adding it to the disabledGroups set and removing it from the enabledGroups set.
     *
     * @param group The identifier of the test group to be disabled.
     */
    static void disableGroup(UT_groupID_t group)
    {
        disabledGroups.insert(group);
        enabledGroups.erase(group);
    }
};

std::vector<TestSuiteInfo> UTTestRunner::suites;
std::unordered_set<UT_groupID_t> UTTestRunner::enabledGroups;
std::unordered_set<UT_groupID_t> UTTestRunner::disabledGroups;

/**
 * @brief Set up resources before each test.
 *
 * This function is called before each test case is executed. It is used to
 * initialize any resources or state that are required for the tests.
 */
void UTCore::SetUp()
{
    // Code to set up resources before each test
}

/**
 * @brief Cleans up resources after each test.
 *
 * This method is called after each test case is executed to perform any necessary
 * cleanup operations. It ensures that resources allocated during the test are
 * properly released.
 */
void UTCore::TearDown()
{
    // Code to clean up resources after each test
}

/**
 * @brief Generates a test filter string for Google Test based on enabled and disabled test groups.
 *
 * This function constructs a filter string that can be used with Google Test to include or exclude
 * specific test suites based on the enabled and disabled test groups defined in UTTestRunner.
 *
 * @return A string representing the test filter. If no groups are enabled or disabled, returns "*"
 *         to indicate that all tests should be run. Otherwise, constructs a filter string that includes
 *         suites from enabled groups and excludes suites from disabled groups.
 */
std::string UTCore::UT_get_test_filter()
{
    if (UTTestRunner::enabledGroups.empty() && UTTestRunner::disabledGroups.empty())
    {
        return "*"; // No restrictions, run all tests
    }

    std::ostringstream includeFilter, excludeFilter;
    bool firstInclude = true, firstExclude = true;

    for (const auto &[suiteName, group] : suiteToGroup)
    {
        if (UTTestRunner::disabledGroups.find(group) != UTTestRunner::disabledGroups.end())
        {
            // If the group is explicitly disabled, add it to the exclude filter
            if (!firstExclude)
                excludeFilter << ":";
            excludeFilter << "-" << suiteName << ".*"; // Exclude tests from this suite
            firstExclude = false;
        }
        else if (UTTestRunner::enabledGroups.empty() || UTTestRunner::enabledGroups.find(group) != UTTestRunner::enabledGroups.end())
        {
            // If no groups are explicitly enabled, allow all (unless disabled)
            if (!firstInclude)
                includeFilter << ":";
            includeFilter << suiteName << ".*"; // Include tests from this suite
            firstInclude = false;
        }
    }

    // Combine the include and exclude filters
    if (firstInclude)
    {
        std::string excludeFilterString = excludeFilter.str().empty() ? "*" : excludeFilter.str();
        return excludeFilterString;
    }

    std::string includeFilterString = includeFilter.str() + (excludeFilter.str().empty() ? "" : ":" + excludeFilter.str());
    return includeFilterString;
}

/**
 * @brief Adds a test suite to a group.
 *
 * This function associates a test suite with a specific group ID.
 *
 * @param testSuiteName The name of the test suite to be added.
 * @param group The group ID to associate with the test suite.
 * @return true if the operation is successful.
 */
bool UTCore::UT_add_suite_withGroupID(const std::string& testSuiteName, UT_groupID_t group)
{
    //std::cout << "Setting group for suite: " << testSuiteName << " to " << group << std::endl;
    suiteToGroup[testSuiteName] = group;
    return true;
}

void UT_set_results_output_filename(const char* szFilenameRoot)
{
    // Null pointer check
    if (szFilenameRoot == nullptr)
    {
        std::cerr << "Error: Filename root is null." << std::endl;
        return; // Exit early to avoid undefined behavior
    }

    std::string filepath = szFilenameRoot;

    // Empty string check
    if (filepath.empty())
    {
        std::cerr << "Error: Filename root is empty." << std::endl;
        return; // Exit early as the filename is invalid
    }

    // Find the position of the last dot
    size_t lastDot = filepath.find_last_of('.');

    // Strip the extension if found
    if (lastDot != std::string::npos)
    {
        filepath = filepath.substr(0, lastDot);
    }

    // Set the output format and path programmatically
    ::testing::FLAGS_gtest_output = std::string("xml:") + filepath + "-report.xml";
    std::cout << "Listing Filename: [" << filepath << "-report.xml]\n" << std::flush;
    std::cout << "Results Filename: [" << filepath << ".log]\n" << std::flush;
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
    if (enable_disable)
        UTTestRunner::enableGroup(static_cast<UT_groupID_t>(groupID));
    else
        UTTestRunner::disableGroup(static_cast<UT_groupID_t>(groupID));
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
                      << STRING_FORMAT("(R)un  (S)elect  (L)ist  (A)ctivate  (F)ailures  (H)elp  (O)ptions (Q)uit") << "\n"
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
                testRunner.displayRunSummary();
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
                testRunner.ToggleTestSuiteExclusion(selected_suites);
                testRunner.listTestSuites();
            }
            else if ((choice == STRING_FORMAT("F")[0]))
            {
                testRunner.displayFailedTests();
            }
            else if ((choice == STRING_FORMAT("O")[0]))
            {
                testRunner.displayOptionsMenu();
            }
        }
    }
    else if (UT_get_test_mode() == UT_MODE_AUTOMATED)
    {
        testRunner.runTests();
    }
    else
    {
        testRunner.runTests();
        testRunner.displayRunSummary();
    }

    UT_LOG( UT_LOG_ASCII_GREEN "Logfile" UT_LOG_ASCII_NC ":[" UT_LOG_ASCII_YELLOW "%s" UT_LOG_ASCII_NC "]\n", UT_log_getLogFilename() );
    UT_exit();

    return UT_STATUS_OK;
}