// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include <unity.h>
#include "hextools.h"

void setUp(void) {}           // before test
void tearDown(void) {}        // after test

void test_toUpperCase() {
    TEST_ASSERT_EQUAL('A', toUpperCase('a'));
    TEST_ASSERT_EQUAL('Z', toUpperCase('z'));
    TEST_ASSERT_EQUAL('0', toUpperCase('0'));
    TEST_ASSERT_EQUAL('9', toUpperCase('9'));
}

void test_isHexCharacter() {
    TEST_ASSERT_TRUE(isHexCharacter('0'));
    TEST_ASSERT_TRUE(isHexCharacter('9'));
    TEST_ASSERT_TRUE(isHexCharacter('A'));
    TEST_ASSERT_TRUE(isHexCharacter('F'));
    TEST_ASSERT_TRUE(isHexCharacter('a'));
    TEST_ASSERT_TRUE(isHexCharacter('f'));
    TEST_ASSERT_FALSE(isHexCharacter('G'));
    TEST_ASSERT_FALSE(isHexCharacter('Z'));
    TEST_ASSERT_FALSE(isHexCharacter('g'));
    TEST_ASSERT_FALSE(isHexCharacter('z'));
}

void test_valueFromHexCharacter() {
    TEST_ASSERT_EQUAL(0x00, valueFromHexCharacter('0'));
    TEST_ASSERT_EQUAL(0x09, valueFromHexCharacter('9'));
    TEST_ASSERT_EQUAL(0x0A, valueFromHexCharacter('A'));
    TEST_ASSERT_EQUAL(0x0F, valueFromHexCharacter('F'));
    TEST_ASSERT_EQUAL(0x0A, valueFromHexCharacter('a'));
    TEST_ASSERT_EQUAL(0x0F, valueFromHexCharacter('f'));
    // Edge cases
    TEST_ASSERT_EQUAL(0x00, valueFromHexCharacter(' '));
}

void test_hexCharacterFromValue() {
    TEST_ASSERT_EQUAL('0', hexCharacterFromValue(0x00));
    TEST_ASSERT_EQUAL('9', hexCharacterFromValue(0x09));
    TEST_ASSERT_EQUAL('A', hexCharacterFromValue(0x0A));
    TEST_ASSERT_EQUAL('F', hexCharacterFromValue(0x0F));
    // Edge cases
    TEST_ASSERT_EQUAL('?', hexCharacterFromValue(0x10));
}

void test_hexStringToBinaryArray() {
    const char input[] = "0123456789ABCDEF";
    uint8_t output[8];
    uint8_t expectedOutput[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    hexStringToBinaryArray(input, output);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedOutput, output, 8);
}

void test_binaryArrayToHexString() {
    uint8_t input[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    char output[9];        // 1 extra for terminating zero
    const char expectedOutput[] = "0123456789ABCDEF";
    binaryArrayToHexString(input, 8, output);
    TEST_ASSERT_EQUAL_STRING(expectedOutput, output);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_toUpperCase);
    RUN_TEST(test_isHexCharacter);
    RUN_TEST(test_valueFromHexCharacter);
    RUN_TEST(test_hexCharacterFromValue);
    RUN_TEST(test_hexStringToBinaryArray);
    RUN_TEST(test_binaryArrayToHexString);
    UNITY_END();
}