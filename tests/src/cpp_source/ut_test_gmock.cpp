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

/**
 * @file ut_test_gmock.cpp
 * @brief Example: mocking a C++ interface with the ut-core gmock wrappers.
 *
 * Demonstrates UT_MOCK_METHOD, UT_MOCK_EXPECT_CALL, matchers (UT_MOCK_ANY / UT_MOCK_GE /
 * UT_MOCK_STR_EQ), actions (UT_MOCK_RETURN), cardinalities (UT_MOCK_EXACTLY / UT_MOCK_ANY_NUMBER)
 * and strictness wrappers (UT_MOCK_NICE) — all without including
 * <gmock/gmock.h> directly.
 */

#include <ut.h>

/* ---- Interface under test (normally supplied by a HAL/AIDL header) ------- */
class ISensor
{
public:
    virtual ~ISensor() = default;
    virtual int  read(const char *name) = 0;
    virtual bool calibrate(int channel, int value) = 0;
    virtual void reset() = 0;
};

/* ---- Mock of the interface, using the UT_ gmock wrappers ----------------- */
class MockSensor : public ISensor
{
public:
    UT_MOCK_METHOD(int,  read,      (const char *name), (override));
    UT_MOCK_METHOD(bool, calibrate, (int channel, int value), (override));
    UT_MOCK_METHOD(void, reset,     (), (override));
};

/* ---- A tiny "system under test" that consumes the interface -------------- */
static bool sensorSelfTest(ISensor &sensor)
{
    sensor.reset();
    if (sensor.read("temperature") < 0)
    {
        return false;
    }
    return sensor.calibrate(0, 100);
}

class UTGMockL1 : public UTCore
{
public:
    UTGMockL1() : UTCore() {}
    ~UTGMockL1() override = default;
};

/* Register the suite with the ut-core group system, as for any gtest suite. */
UT_ADD_TEST_TO_GROUP(UTGMockL1, UT_TESTS_L1)

/* Happy path: exact expectations are all met. */
UT_ADD_TEST(UTGMockL1, SelfTestPasses)
{
    MockSensor sensor;
    UT_MOCK_EXPECT_CALL(sensor, reset()).Times(UT_MOCK_EXACTLY(1));
    UT_MOCK_EXPECT_CALL(sensor, read(UT_MOCK_STR_EQ("temperature"))).WillOnce(UT_MOCK_RETURN(21));
    UT_MOCK_EXPECT_CALL(sensor, calibrate(0, 100)).WillOnce(UT_MOCK_RETURN(true));

    UT_ASSERT_TRUE(sensorSelfTest(sensor));
}

/* Matchers + actions: any channel, any non-negative value calibrates. */
UT_ADD_TEST(UTGMockL1, MatchersAndActions)
{
    MockSensor sensor;
    UT_MOCK_EXPECT_CALL(sensor, reset()).Times(UT_MOCK_ANY_NUMBER);
    UT_MOCK_EXPECT_CALL(sensor, read(UT_MOCK_ANY)).WillRepeatedly(UT_MOCK_RETURN(5));
    UT_MOCK_EXPECT_CALL(sensor, calibrate(UT_MOCK_ANY, UT_MOCK_GE(0))).WillOnce(UT_MOCK_RETURN(true));

    UT_ASSERT_TRUE(sensorSelfTest(sensor));
}

/* NiceMock: uninteresting calls (reset/calibrate) are ignored; read drives
 * the failure path so we can assert the negative branch. */
UT_ADD_TEST(UTGMockL1, NiceMockFailurePath)
{
    UT_MOCK_NICE(MockSensor) sensor;
    UT_MOCK_EXPECT_CALL(sensor, read(UT_MOCK_ANY)).WillOnce(UT_MOCK_RETURN(-1));

    UT_ASSERT_FALSE(sensorSelfTest(sensor));
}

/* Negative example (kept DISABLED_ so it does not fail the suite): an unmet
 * expectation. Remove the DISABLED_ prefix to see gmock fail the run because
 * calibrate() is expected but sensorSelfTest short-circuits on read() < 0. */
UT_ADD_TEST(UTGMockL1, DISABLED_UnmetExpectationFails)
{
    MockSensor sensor;
    UT_MOCK_EXPECT_CALL(sensor, reset()).Times(UT_MOCK_ANY_NUMBER);
    UT_MOCK_EXPECT_CALL(sensor, read(UT_MOCK_ANY)).WillOnce(UT_MOCK_RETURN(-1));
    UT_MOCK_EXPECT_CALL(sensor, calibrate(UT_MOCK_ANY, UT_MOCK_ANY)).WillOnce(UT_MOCK_RETURN(true)); // never called

    UT_ASSERT_FALSE(sensorSelfTest(sensor));
}
