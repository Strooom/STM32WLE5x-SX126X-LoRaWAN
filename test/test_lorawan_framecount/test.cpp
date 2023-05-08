// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################
#include <unity.h>
#include "framecount.h"

void setUp(void) {}           // before test
void tearDown(void) {}        // after test

void test_initialize() {
    frameCount theCount;
    TEST_ASSERT_EQUAL(0U, theCount.asUint32);
}

void test_increment() {
    constexpr uint32_t testValue{0x0000'0F00};
    frameCount theCount;
    theCount.set(testValue);
    TEST_ASSERT_EQUAL(testValue, theCount.asUint32);
    theCount.increment();
    TEST_ASSERT_EQUAL(testValue + 1, theCount.asUint32);
}

void test_guess() {
    uint32_t testValue{0x0000'0F00};
    TEST_ASSERT_EQUAL(testValue + 16, frameCount::guessFromUint16(testValue, static_cast<uint16_t>(testValue + 16)));

    testValue = 0x0000'FFF0;
    TEST_ASSERT_EQUAL(testValue + 16, frameCount::guessFromUint16(testValue, static_cast<uint16_t>(testValue + 16)));
}

void test_guess_gap_too_large() {
    uint32_t testValue{0x0000'0F00};
    TEST_ASSERT_EQUAL(testValue, frameCount::guessFromUint16(testValue, static_cast<uint16_t>(testValue + frameCount::maximumGap + 16)));

    testValue = 0x0000'FFF0;
    TEST_ASSERT_EQUAL(testValue, frameCount::guessFromUint16(testValue, static_cast<uint16_t>(testValue + frameCount::maximumGap + 16)));
}



int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_increment);
    RUN_TEST(test_guess);
    RUN_TEST(test_guess_gap_too_large);
    UNITY_END();
}
