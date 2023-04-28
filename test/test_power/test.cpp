#include <unity.h>
#include "power.h"

void setUp(void) {}           // before test
void tearDown(void) {}        // after test

// Notes : there is not a lot about power which is hardware independent
// Most unit tests will be in the hardware specific test

void test_some() {
    TEST_IGNORE_MESSAGE("To Be Implemented...");
}


int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_some);
    UNITY_END();
}