#include <unity.h>
#include "bytebuffer.h"

void setUp(void) {}           // before test
void tearDown(void) {}        // after test

void test_initialize() {
    byteBuffer theBuffer;
    TEST_ASSERT_EQUAL_UINT32(0U, theBuffer.length);
}

void test_set() {
    byteBuffer theBuffer;
    uint8_t testData[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    theBuffer.set(testData, 5);
    TEST_ASSERT_EQUAL_UINT32(5U, theBuffer.length);
    TEST_ASSERT_EQUAL_UINT32(1U, theBuffer.buffer[0]);
    TEST_ASSERT_EQUAL_UINT32(5U, theBuffer.buffer[4]);
    uint8_t testData2[] = {0xAA, 0xBB, 0xCC};
    theBuffer.set(testData2, 3);
    TEST_ASSERT_EQUAL_UINT32(3U, theBuffer.length);
    TEST_ASSERT_EQUAL_UINT32(0xAA, theBuffer.buffer[0]);
    TEST_ASSERT_EQUAL_UINT32(0xCC, theBuffer.buffer[2]);
    TEST_ASSERT_EQUAL_UINT32(4U, theBuffer.buffer[3]);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_set);
    UNITY_END();
}