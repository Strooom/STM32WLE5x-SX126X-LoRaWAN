#include <unity.h>
#include "sx126x.h"


void setUp(void) {}           // before test
void tearDown(void) {}        // after test
void test_calculateFrequencyRegisterValue() {
TEST_ASSERT_EQUAL_UINT32(0x36419999, sx126x::calculateFrequencyRegisterValue(868100000));
TEST_ASSERT_EQUAL_UINT32(0x3644CCCC, sx126x::calculateFrequencyRegisterValue(868300000));
TEST_ASSERT_EQUAL_UINT32(0x36380000, sx126x::calculateFrequencyRegisterValue(867500000));
TEST_ASSERT_EQUAL_UINT32(0x363B3333, sx126x::calculateFrequencyRegisterValue(867700000));
TEST_ASSERT_EQUAL_UINT32(0x36586666, sx126x::calculateFrequencyRegisterValue(869525000));
TEST_ASSERT_EQUAL_UINT32(0x363E6666, sx126x::calculateFrequencyRegisterValue(867900000));
}



// void test_deviceDetection() {
//     // read address 0x0000320 :  53 58 31 32 36 31 20 56 32 44 20 32 44 30 32 00 | SX1261 V2D 2D02.
// }

// void test_writeRegister() {
//     // set eg PacketType and verify it
//     // HAL_SUBGHZ_ExecGetCmd(&hsubghz, RADIO_GET_PACKETTYPE, responseData, 1);
//     // theLog.snprintf("2.0 PacketType before = [%02X]\n", responseData[0]);
//     // commandData[0] = 1; // packetType LORA
//     // HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_PACKETTYPE, commandData, 1);
//     // HAL_SUBGHZ_ExecGetCmd(&hsubghz, RADIO_GET_PACKETTYPE, responseData, 1);
//     // theLog.snprintf("2.1 PacketType after = [%02X]\n", responseData[0]);
// }

// void test_writeRegister() {
//     // HAL_SUBGHZ_ReadRegisters(&hsubghz, 0x0740, responseData, 2);
//     // 	theLog.snprintf("10.0 Syncword before [%02X %02X]\n", responseData[0], responseData[1]);
//     // 	commandData[0] = 0x34; // LoRa Syncword MSB
//     // 	commandData[1] = 0x44; // LoRa Syncword LSB
//     // 	HAL_SUBGHZ_WriteRegisters(&hsubghz, 0x0740, commandData, 2);
//     // 	HAL_SUBGHZ_ReadRegisters(&hsubghz, 0x0740, responseData, 2);
//     // 	theLog.snprintf("10.1 Syncword after [%02X %02X]\n", responseData[0], responseData[1]);
// }

// void test_calculateMic0() {
//     aesKey theKey;
//     theKey.setFromASCII("2b7e151628aed2a6abf7158809cf4f3c");        // test key from rfc4493 : https://www.rfc-editor.org/rfc/rfc4493

//     byteBuffer clearText;
//     clearText.clear();        // test vector from rfc4493 : https://www.rfc-editor.org/rfc/rfc4493
//     sBuffer sourceData;
//     sourceData.Data    = clearText.buffer;        // copy to an sBuffer as this is what the 3rd party library expects
//     sourceData.Counter = clearText.length;        // idem

//     unsigned char micAsBinaryArray[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//     Calculate_MIC2(&sourceData, theKey.asUnsignedChar(), micAsBinaryArray);
//     char micAsHexString[33];
//     binaryArrayToHexString(micAsBinaryArray, 16, micAsHexString);
//     TEST_ASSERT_EQUAL_STRING("BB1D6929E95937287FA37D129B756746", micAsHexString);
// }

// void test_calculateMic1() {
//     aesKey theKey;
//     theKey.setFromASCII("2b7e151628aed2a6abf7158809cf4f3c");        // test key from rfc4493 : https://www.rfc-editor.org/rfc/rfc4493

//     byteBuffer clearText;
//     clearText.setFromASCII("6bc1bee22e409f96e93d7e117393172a");        // test vector from rfc4493 : https://www.rfc-editor.org/rfc/rfc4493
//     sBuffer sourceData;
//     sourceData.Data    = clearText.buffer;        // copy to an sBuffer as this is what the 3rd party library expects
//     sourceData.Counter = clearText.length;        // idem

//     unsigned char micAsBinaryArray[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//     Calculate_MIC2(&sourceData, theKey.asUnsignedChar(), micAsBinaryArray);
//     char micAsHexString[33];
//     binaryArrayToHexString(micAsBinaryArray, 16, micAsHexString);
//     TEST_ASSERT_EQUAL_STRING("070A16B46B4D4144F79BDD9DD04A287C", micAsHexString);
// }

// void test_calculateMic2() {
//     aesKey theKey;
//     theKey.setFromASCII("2b7e151628aed2a6abf7158809cf4f3c");        // test key from rfc4493 : https://www.rfc-editor.org/rfc/rfc4493

//     byteBuffer clearText;
//     clearText.setFromASCII("6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411");        // test vector from rfc4493 : https://www.rfc-editor.org/rfc/rfc4493
//     sBuffer sourceData;
//     sourceData.Data    = clearText.buffer;        // copy to an sBuffer as this is what the 3rd party library expects
//     sourceData.Counter = clearText.length;        // idem

//     unsigned char micAsBinaryArray[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//     Calculate_MIC2(&sourceData, theKey.asUnsignedChar(), micAsBinaryArray);
//     char micAsHexString[33];
//     binaryArrayToHexString(micAsBinaryArray, 16, micAsHexString);
//     TEST_ASSERT_EQUAL_STRING("DFA66747DE9AE63030CA32611497C827", micAsHexString);
// }

// void test_calculateMic3() {
//     aesKey theKey;
//     theKey.setFromASCII("2b7e151628aed2a6abf7158809cf4f3c");        // test key from rfc4493 : https://www.rfc-editor.org/rfc/rfc4493

//     byteBuffer clearText;
//     clearText.setFromASCII("6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710");        // test vector from rfc4493 : https://www.rfc-editor.org/rfc/rfc4493
//     sBuffer sourceData;
//     sourceData.Data    = clearText.buffer;        // copy to an sBuffer as this is what the 3rd party library expects
//     sourceData.Counter = clearText.length;        // idem

//     unsigned char micAsBinaryArray[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//     Calculate_MIC2(&sourceData, theKey.asUnsignedChar(), micAsBinaryArray);
//     char micAsHexString[33];
//     binaryArrayToHexString(micAsBinaryArray, 16, micAsHexString);
//     TEST_ASSERT_EQUAL_STRING("51F0BEBF7E3B9D92FC49741779363CFE", micAsHexString);
// }

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_calculateFrequencyRegisterValue);
    UNITY_END();
}