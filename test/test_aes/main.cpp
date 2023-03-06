#include <unity.h>
#include "aes.h"


void setUp(void) {}           // before test
void tearDown(void) {}        // after test
void noInterrupts() {}        // mock for unitTesting
void interrupts() {}          // idem

void initialization() {
    // eventBuffer theBuffer = eventBuffer();
    // TEST_ASSERT_EQUAL_UINT32(0U, theBuffer.head);
    // TEST_ASSERT_EQUAL_UINT32(0U, theBuffer.level);
    // TEST_ASSERT_EQUAL_UINT32(0U, theBuffer.levelMax);
    // TEST_ASSERT_EQUAL_UINT32(0U, theBuffer.getLevelMax());
    // TEST_ASSERT_FALSE(theBuffer.hasEvents());
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(initialization);
    // RUN_TEST(ignoreEventNone);
    // RUN_TEST(pushPop);
    // RUN_TEST(boundariesUnderflow);
    // RUN_TEST(boundariesOverflow);
    // RUN_TEST(test_initialize);
    UNITY_END();
}