#include <unity.h>
#include "aeskey.h"

void setUp(void) {}           // before test
void tearDown(void) {}        // after test

void test_toUpperCase() {
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>('A'), aesKey::toUpperCase(static_cast<uint8_t>('a')));
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>('A'), aesKey::toUpperCase(static_cast<uint8_t>('A')));
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>('0'), aesKey::toUpperCase(static_cast<uint8_t>('0')));
}

void test_isHexCharacter() {
    TEST_ASSERT_TRUE(aesKey::isHexCharacter(static_cast<uint8_t>('0')));
    TEST_ASSERT_TRUE(aesKey::isHexCharacter(static_cast<uint8_t>('9')));
    TEST_ASSERT_TRUE(aesKey::isHexCharacter(static_cast<uint8_t>('A')));
    TEST_ASSERT_TRUE(aesKey::isHexCharacter(static_cast<uint8_t>('F')));
    TEST_ASSERT_FALSE(aesKey::isHexCharacter(static_cast<uint8_t>('-')));
    TEST_ASSERT_FALSE(aesKey::isHexCharacter(static_cast<uint8_t>('O')));
    TEST_ASSERT_FALSE(aesKey::isHexCharacter(static_cast<uint8_t>('a')));        // we assume uppercase, so lowercase should fail...
}

void test_valueFromHexCharacter() {
    TEST_ASSERT_EQUAL_UINT8(0U, aesKey::valueFromHexCharacter(static_cast<uint8_t>('0')));
    TEST_ASSERT_EQUAL_UINT8(9U, aesKey::valueFromHexCharacter(static_cast<uint8_t>('9')));
    TEST_ASSERT_EQUAL_UINT8(10U, aesKey::valueFromHexCharacter(static_cast<uint8_t>('A')));
    TEST_ASSERT_EQUAL_UINT8(15U, aesKey::valueFromHexCharacter(static_cast<uint8_t>('F')));
}

void test_hexCharacterFromValue() {
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>('0'), aesKey::hexCharacterFromValue(0U));
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>('9'), aesKey::hexCharacterFromValue(9U));
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>('A'), aesKey::hexCharacterFromValue(10U));
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>('F'), aesKey::hexCharacterFromValue(15U));
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>('?'), aesKey::hexCharacterFromValue(16U));
}

void test_initialize() {
    aesKey theKey;
    // TODO : check that the key is all zeros...
    TEST_ASSERT_TRUE(theKey.fromASCII("00112233445566778899AABBCCDDEEFF"));

    // eventBuffer theBuffer = eventBuffer();

    // TEST_ASSERT_EQUAL_UINT32(0U, theBuffer.level);
    // TEST_ASSERT_EQUAL_UINT32(0U, theBuffer.levelMax);
    // TEST_ASSERT_EQUAL_UINT32(0U, theBuffer.getLevelMax());
    // TEST_ASSERT_FALSE(theBuffer.hasEvents());
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_toUpperCase);
    RUN_TEST(test_isHexCharacter);
    RUN_TEST(test_valueFromHexCharacter);
    //RUN_TEST(test_hexCharacterFromValue);
    RUN_TEST(test_initialize);
    UNITY_END();
}