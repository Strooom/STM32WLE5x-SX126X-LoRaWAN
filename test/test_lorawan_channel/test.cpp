// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################
#include <unity.h>
#include "lorachannelcollection.h"

void setUp(void) {}           // before test
void tearDown(void) {}        // after test

void test_initialize() {
    loRaChannelCollection theChannels;
    TEST_ASSERT_EQUAL(3U, theChannels.nmbrAvailableChannels);
    TEST_ASSERT_EQUAL_UINT32(868'100'000U, theChannels.txRxChannels[0].frequency);
    TEST_ASSERT_EQUAL_UINT32(868'300'000U, theChannels.txRxChannels[1].frequency);
    TEST_ASSERT_EQUAL_UINT32(868'500'000U, theChannels.txRxChannels[2].frequency);
}

void test_add_channel() {
    loRaChannelCollection theChannels;
    theChannels.addChannel(1234U);
    TEST_ASSERT_EQUAL(4U, theChannels.nmbrAvailableChannels);
    TEST_ASSERT_EQUAL_UINT32(1234U, theChannels.txRxChannels[3].frequency);
}

void test_remove_channel() {
    TEST_IGNORE_MESSAGE("To Be Implemented");
}

void test_add_remove_boundaries() {
    TEST_IGNORE_MESSAGE("To Be Implemented");
}


int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_add_channel);
    RUN_TEST(test_remove_channel);
    RUN_TEST(test_add_remove_boundaries);
    UNITY_END();
}
