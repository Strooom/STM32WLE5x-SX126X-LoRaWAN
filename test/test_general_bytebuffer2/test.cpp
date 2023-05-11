#include <unity.h>
#include "bytebuffer2.h"

void setUp(void) {}           // before test
void tearDown(void) {}        // after test

void test_initialize() {
    constexpr uint32_t testBufferLength{32};
    byteBuffer2<testBufferLength> aBuffer;
    TEST_ASSERT_EQUAL_UINT32(testBufferLength, aBuffer.length);
    TEST_ASSERT_EQUAL_UINT32(0U, aBuffer.getLevel());
    TEST_ASSERT_TRUE(aBuffer.isEmpty());
    aBuffer.append((uint8_t *)"1234567890", 10U);
    TEST_ASSERT_EQUAL_UINT32(10U, aBuffer.getLevel());
    TEST_ASSERT_FALSE(aBuffer.isEmpty());
    aBuffer.initialize();
    TEST_ASSERT_EQUAL_UINT32(0U, aBuffer.getLevel());
    TEST_ASSERT_TRUE(aBuffer.isEmpty());
}

void test_append() {
    constexpr uint32_t testBufferLength{32};
    byteBuffer2<testBufferLength> aBuffer;
    aBuffer.append((uint8_t *)"1234567890", 10U);
    TEST_ASSERT_EQUAL_UINT32(10U, aBuffer.getLevel());
    aBuffer.append((uint8_t *)"1234567890", 10U);
    TEST_ASSERT_EQUAL_UINT32(20U, aBuffer.getLevel());
    TEST_ASSERT_EQUAL_UINT8_ARRAY((uint8_t *)"12345678901234567890", aBuffer.get(), 20U);
    aBuffer.append((uint8_t *)"12345678901234567890", 20U);
    TEST_ASSERT_EQUAL_UINT32(testBufferLength, aBuffer.getLevel());
}

void test_shiftLeft() {
    constexpr uint32_t testBufferLength{32};
    byteBuffer2<testBufferLength> aBuffer;
    aBuffer.append((uint8_t *)"1234567890ABCDEFGHIJ", 20U);
    aBuffer.shiftLeft(10U);
    TEST_ASSERT_EQUAL_UINT32(10U, aBuffer.getLevel());
    TEST_ASSERT_EQUAL_UINT8_ARRAY((uint8_t *)"ABCDEFGHIJ", aBuffer.get(), 10U);
    aBuffer.shiftLeft(15U);
    TEST_ASSERT_EQUAL_UINT32(0U, aBuffer.getLevel());
}

void test_shiftRight() {
    constexpr uint32_t testBufferLength{32};
    byteBuffer2<testBufferLength> aBuffer;
    aBuffer.append((uint8_t *)"1234567890", 10U);
    TEST_ASSERT_EQUAL_UINT32(10U, aBuffer.getLevel());
    aBuffer.shiftRight(10U);
    TEST_ASSERT_EQUAL_UINT32(20U, aBuffer.getLevel());
    TEST_ASSERT_EQUAL_UINT8_ARRAY((uint8_t *)"12345678901234567890", aBuffer.get(), 20U);
    aBuffer.shiftRight(20U);
    TEST_ASSERT_EQUAL_UINT32(32U, aBuffer.getLevel());
}

void test_set() {
    constexpr uint32_t testBufferLength{32};
    byteBuffer2<testBufferLength> aBuffer;
    aBuffer.set((uint8_t *)"1234567890", 10U);
    TEST_ASSERT_EQUAL_UINT32(0U, aBuffer.getLevel());
    TEST_ASSERT_EQUAL_UINT8_ARRAY((uint8_t *)"1234567890", aBuffer.get(), 10U);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_append);
    RUN_TEST(test_shiftLeft);
    RUN_TEST(test_shiftRight);
    RUN_TEST(test_set);
    UNITY_END();
}