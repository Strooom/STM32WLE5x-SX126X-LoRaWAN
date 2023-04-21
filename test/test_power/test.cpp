#include <unity.h>
#include "power.h"

void setUp(void) {}           // before test
void tearDown(void) {}        // after test

void test_some() {
    TEST_IGNORE_MESSAGE("To Be Implemented...");
}


int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_some);
    UNITY_END();
}