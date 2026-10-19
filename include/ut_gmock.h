/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2026 RDK Management
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

/** @brief
 * UT Unit wrapper to the GoogleMock (gmock) framework.
 *
 * Hides the functionality of gmock behind UT_-prefixed macros, so tests can
 * create and use mocks of C++ interfaces without including <gmock/gmock.h>
 * directly. Companion to ut_gtest.h.
 */
/** @addtogroup UT_GMOCK
 * @{
 */

#ifndef __UT_GMOCK_H
#define __UT_GMOCK_H

#include <gmock/gmock.h>

/**
 * @brief Declares a mocked method inside a mock class.
 *
 * Thin wrapper over gmock's MOCK_METHOD. Use the modern 3- or 4-argument form:
 *   UT_MOCK_METHOD(return_type, method_name, (args...))
 *   UT_MOCK_METHOD(return_type, method_name, (args...), (const, override))
 *
 * @code
 * class MockDriver : public IDriver {
 * public:
 *     UT_MOCK_METHOD(int, open, (const char *path), (override));
 *     UT_MOCK_METHOD(bool, read, (int fd, void *buf, size_t len), (override));
 * };
 * @endcode
 */
#define UT_MOCK_METHOD(...) MOCK_METHOD(__VA_ARGS__)

/**
 * @brief Sets an expectation on a mock method call.
 *
 * Wrapper over gmock's EXPECT_CALL. Chain the usual clauses
 * (.Times(), .WillOnce(), .WillRepeatedly(), .With(), ...).
 *
 * @code
 * UT_MOCK_EXPECT_CALL(mock, open(UT_MOCK_ANY)).Times(UT_MOCK_AT_LEAST(1)).WillOnce(UT_MOCK_RETURN(3));
 * @endcode
 */
#define UT_MOCK_EXPECT_CALL(mock_object, call) EXPECT_CALL(mock_object, call)

/**
 * @brief Sets the default behaviour of a mock method (no expectation on count).
 */
#define UT_MOCK_ON_CALL(mock_object, call) ON_CALL(mock_object, call)

/* ---- Mock strictness wrappers ------------------------------------------- */

/**
 * @brief A mock whose uninteresting calls are silently ignored.
 */
#define UT_MOCK_NICE(type) ::testing::NiceMock<type>

/**
 * @brief A mock whose uninteresting calls produce a warning (gmock default).
 */
#define UT_MOCK_NAGGY(type) ::testing::NaggyMock<type>

/**
 * @brief A mock whose uninteresting calls are treated as failures.
 */
#define UT_MOCK_STRICT(type) ::testing::StrictMock<type>

/* ---- Common matchers (argument matching in EXPECT_CALL) ------------------ */

/*
 * NOTE: these UT_MOCK_* matchers are NOT the UT_ASSERT_* assertions from
 * ut_gtest.h. A matcher (e.g. UT_MOCK_LT(5)) describes which argument values
 * satisfy an expectation and is used *inside*
 * UT_MOCK_EXPECT_CALL(mock, foo(UT_MOCK_LT(5))). An assertion (e.g.
 * UT_ASSERT_LESS(a, b)) checks a value and records pass/fail. The UT_MOCK_
 * prefix makes the distinction explicit: everything in this header is UT_MOCK_*.
 */

/** @brief Matches any argument value. */
#define UT_MOCK_ANY ::testing::_
/** @brief Matches an argument equal to @p value. */
#define UT_MOCK_EQ(value) ::testing::Eq(value)
/** @brief Matches an argument not equal to @p value. */
#define UT_MOCK_NE(value) ::testing::Ne(value)
/** @brief Matches an argument greater than @p value. */
#define UT_MOCK_GT(value) ::testing::Gt(value)
/** @brief Matches an argument greater than or equal to @p value. */
#define UT_MOCK_GE(value) ::testing::Ge(value)
/** @brief Matches an argument less than @p value. */
#define UT_MOCK_LT(value) ::testing::Lt(value)
/** @brief Matches an argument less than or equal to @p value. */
#define UT_MOCK_LE(value) ::testing::Le(value)
/** @brief Matches a non-null pointer argument. */
#define UT_MOCK_NOTNULL ::testing::NotNull()
/** @brief Matches a null pointer argument. */
#define UT_MOCK_ISNULL ::testing::IsNull()
/** @brief Matches a C-string argument equal to @p value. */
#define UT_MOCK_STR_EQ(value) ::testing::StrEq(value)
/** @brief Matches an argument within [@p lo, @p hi]. */
#define UT_MOCK_BETWEEN(lo, hi) ::testing::AllOf(::testing::Ge(lo), ::testing::Le(hi))

/* ---- Common actions (what a mocked call does) --------------------------- */

/** @brief Returns @p value from the mocked call. */
#define UT_MOCK_RETURN(value) ::testing::Return(value)
/** @brief Returns a reference to @p value from the mocked call. */
#define UT_MOCK_RETURN_REF(value) ::testing::ReturnRef(value)
/** @brief Performs the method's default action (e.g. the ON_CALL default, or
 *  gmock's built-in default return for the type). Note: a mocked method with
 *  no action already returns a default-constructed value automatically. */
#define UT_MOCK_DO_DEFAULT ::testing::DoDefault()
/** @brief Invokes @p f (a callable) with the mocked call's arguments. */
#define UT_MOCK_INVOKE(f) ::testing::Invoke(f)
/** @brief Writes @p value through the pointer/reference at argument index @p N. */
#define UT_MOCK_SET_ARG_POINTEE(N, value) ::testing::SetArgPointee<N>(value)
/** @brief Performs all of the supplied actions in order. */
#define UT_MOCK_DO_ALL(...) ::testing::DoAll(__VA_ARGS__)
/** @brief Throws @p exception from the mocked call. */
#define UT_MOCK_THROW(exception) ::testing::Throw(exception)

/* ---- Cardinalities (arguments to .Times()) ------------------------------ */

/** @brief Cardinality: at least @p n calls. */
#define UT_MOCK_AT_LEAST(n) ::testing::AtLeast(n)
/** @brief Cardinality: at most @p n calls. */
#define UT_MOCK_AT_MOST(n) ::testing::AtMost(n)
/** @brief Cardinality: exactly @p n calls. */
#define UT_MOCK_EXACTLY(n) ::testing::Exactly(n)
/** @brief Cardinality: any number of calls (including zero). */
#define UT_MOCK_ANY_NUMBER ::testing::AnyNumber()

/**
 * @brief Verifies and clears all expectations on @p mock immediately.
 *
 * Returns true if all expectations were satisfied. Normally verification runs
 * automatically when the mock is destroyed (gmock is initialised via
 * ::testing::InitGoogleMock in the UT test runner), but this is useful to
 * assert expectations mid-test.
 */
#define UT_MOCK_VERIFY_AND_CLEAR(mock) ::testing::Mock::VerifyAndClearExpectations(&(mock))

#endif  /* UT -> GMOCK - Wrapper */

/** @} */
