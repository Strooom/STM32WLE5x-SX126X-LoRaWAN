// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################
#include <unity.h>
#include <cstring>
#include "nvs.h"
#include "sx126x.h"
#include "eventbuffer.h"
#include "applicationevent.h"
#include "lorawan.h"

void setUp(void) {}           // before test
void tearDown(void) {}        // after test

nonVolatileStorage nvs;
eventBuffer<loRaWanEvent, 16U> loraWanEventBuffer;
eventBuffer<applicationEvent, 16U> applicationEventBuffer;
sx126x theRadio;
// extern uint8_t mockSx126xDataBuffer[256];

// Test vector is a message actually accepted on the network server
constexpr uint32_t testVectorLength        = 23;
const uint8_t testVector[testVectorLength] = {
    // 0x49, 0x00, 0x00, 0x00, 0x00, 0x00, 0x92, 0x3B, 0x0B, 0x26, 0x39, 0x07, 0x00, 0x00, 0x00, 0x13,        // B0 block
    0x40,                                                              // macHeader : uplink unconfirmed
    0x92, 0x3B, 0x0B, 0x26,                                            // deviceAddress = 0x260B3B92
    0x00,                                                              // frameControl
    0x39, 0x07,                                                        // FrameCount = 0x0739 = 1849
    0x10,                                                              // framePort = 16
    0x49, 0xE8, 0xFA, 0x19, 0xED, 0x8F, 0xE2, 0x5C, 0x48, 0xB1,        // framePayload, 10 bytes, encrypted
    0xB8, 0x59, 0x92, 0x49                                             // mic
};

void test_offsetsTx() {
    LoRaWAN theNetwork;

    memcpy(theNetwork.rawMessage + theNetwork.b0BlockLength, testVector, testVectorLength);

    theNetwork.setOffsetsAndLengthsTx(10U);
    TEST_ASSERT_EQUAL_UINT32(10U, theNetwork.framePayloadLength);
    TEST_ASSERT_EQUAL_UINT32(23U, theNetwork.loRaPayloadLength);
    TEST_ASSERT_EQUAL_UINT32(0U, theNetwork.frameOptionsLength);
    TEST_ASSERT_EQUAL_UINT32(7U, theNetwork.frameHeaderLength);
    TEST_ASSERT_EQUAL_UINT32(18U, theNetwork.macPayloadLength);

    TEST_ASSERT_EQUAL_UINT32(theNetwork.b0BlockLength + 8, theNetwork.framePortOffset);
    TEST_ASSERT_EQUAL_UINT32(theNetwork.b0BlockLength + 9, theNetwork.framePayloadOffset);
    TEST_ASSERT_EQUAL_UINT32(theNetwork.b0BlockLength + 19, theNetwork.micOffset);        // 9 for all headers, 10 for payload

    // Now let's double check and test some of the contents at the offsets
    TEST_ASSERT_EQUAL_UINT32(0x40, theNetwork.rawMessage[theNetwork.macHeaderOffset]);            // macHeader
    TEST_ASSERT_EQUAL_UINT32(0x92, theNetwork.rawMessage[theNetwork.deviceAddressOffset]);        // first byte (LSB) of DevAddr
    TEST_ASSERT_EQUAL_UINT32(0, theNetwork.rawMessage[theNetwork.frameControlOffset]);            // frameControl
    TEST_ASSERT_EQUAL_UINT32(0x39, theNetwork.rawMessage[theNetwork.frameCountOffset]);           // first byte (LSB) of frameCount
    TEST_ASSERT_EQUAL_UINT32(0xB8, theNetwork.rawMessage[theNetwork.micOffset]);                  // first byte of MIC

    // TODO : I need an additional test with frameOptions NOT empty
}

void test_insertFramePayload() {
    LoRaWAN theNetwork;
    constexpr uint32_t testPayloadBytesLength{10U};
    const uint8_t testPayloadBytes[testPayloadBytesLength] = {0x02, 0x01, 0xA8, 0xF4, 0x48, 0x64, 0xA4, 0x70, 0x55, 0x40};
    theNetwork.setOffsetsAndLengthsTx(testPayloadBytesLength);
    theNetwork.insertPayload(testPayloadBytes, testPayloadBytesLength);

    for (uint32_t index = 0; index < 10; index++) {
        TEST_ASSERT_EQUAL_UINT32(testPayloadBytes[index], theNetwork.rawMessage[theNetwork.framePayloadOffset + index]);
    }
}

void test_encryptFramePayload() {
    LoRaWAN theNetwork;
    theNetwork.DevAddr.set(0x260B3B92);
    theNetwork.uplinkFrameCount.set(1850);
    theNetwork.applicationKey.setFromASCII("398F459FE521152FD5B014EA44428AC2");
    constexpr uint32_t testPayloadBytesLength{10U};
    const uint8_t testPayloadBytesBeforeEncryption[testPayloadBytesLength] = {0x02, 0x01, 0xA8, 0xF4, 0x48, 0x64, 0xA4, 0x70, 0x55, 0x40};
    const uint8_t testPayloadBytesAfterEncryption[testPayloadBytesLength]  = {0x61, 0x4A, 0xE1, 0x01, 0x75, 0xCC, 0x1B, 0x5A, 0x77, 0xFF};

    theNetwork.setOffsetsAndLengthsTx(testPayloadBytesLength);
    theNetwork.insertPayload(testPayloadBytesBeforeEncryption, testPayloadBytesLength);
    theNetwork.encryptPayload(theNetwork.applicationKey);

    for (uint32_t index = 0; index < 10; index++) {
        TEST_ASSERT_EQUAL_UINT32(testPayloadBytesAfterEncryption[index], theNetwork.rawMessage[theNetwork.framePayloadOffset + index]);
    }
}

void test_insertHeaders() {
    constexpr uint32_t testPort = 16;
    const uint8_t testHeader[]  = {0x40, 0x92, 0x3B, 0x0B, 0x26, 0x00, 0x3A, 0x07, 0x10};
    LoRaWAN theNetwork;
    theNetwork.DevAddr.set(0x260B3B92);
    theNetwork.uplinkFrameCount.set(1850);
    theNetwork.insertHeaders(testPort);

    for (uint32_t index = 0; index < 9; index++) {
        TEST_ASSERT_EQUAL_UINT32(testHeader[index], theNetwork.rawMessage[theNetwork.b0BlockLength + index]);
    }
}

void test_insertBlockB0() {
    const uint8_t testBlockB0[16] = {0x49, 0x00, 0x00, 0x00, 0x00, 0x00, 0x92, 0x3B, 0x0B, 0x26, 0x3A, 0x07, 0x00, 0x00, 0x00, 0x13};
    LoRaWAN theNetwork;
    theNetwork.setOffsetsAndLengthsTx(10U);
    deviceAddress testAddress;
    testAddress.set(0x260B3B92);
    frameCount testFrameCount;
    testFrameCount.set(1850);
    theNetwork.insertBlockB0(linkDirection::uplink, testAddress, testFrameCount, (theNetwork.macHeaderLength + theNetwork.macPayloadLength));

    for (uint32_t index = 0; index < 16; index++) {
        TEST_ASSERT_EQUAL_UINT32(testBlockB0[index], theNetwork.rawMessage[index]);
    }
}

void test_insertMic() {
    LoRaWAN theNetwork;
    theNetwork.DevAddr.set(0x260B3B92);
    theNetwork.uplinkFrameCount.set(1850);
    theNetwork.applicationKey.setFromASCII("398F459FE521152FD5B014EA44428AC2");
    theNetwork.networkKey.setFromASCII("680AB79064FD273E52FBBF4FC6349B13");
    constexpr uint32_t testPayloadBytesLength{10U};
    const uint8_t testPayloadBytesBeforeEncryption[testPayloadBytesLength] = {0x02, 0x01, 0xA8, 0xF4, 0x48, 0x64, 0xA4, 0x70, 0x55, 0x40};
    const uint8_t testMic[4] = {0xB5, 0x51, 0xEF, 0xDF};

    theNetwork.setOffsetsAndLengthsTx(testPayloadBytesLength);
    theNetwork.insertPayload(testPayloadBytesBeforeEncryption, testPayloadBytesLength);
    theNetwork.encryptPayload(theNetwork.applicationKey);
    theNetwork.insertHeaders(16U);
    theNetwork.insertBlockB0(linkDirection::uplink, theNetwork.DevAddr, theNetwork.uplinkFrameCount, (theNetwork.macHeaderLength + theNetwork.macPayloadLength));
    theNetwork.insertMic();

    for (uint32_t index = 0; index < 4; index++) {
        TEST_ASSERT_EQUAL_UINT32(testMic[index], theNetwork.rawMessage[theNetwork.micOffset + index]);
    }
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_offsetsTx);
    RUN_TEST(test_insertFramePayload);
    RUN_TEST(test_encryptFramePayload);
    RUN_TEST(test_insertHeaders);
    RUN_TEST(test_insertBlockB0);
    RUN_TEST(test_insertMic);
    UNITY_END();
}

// rawMessage BE =
// 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
// 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
// 0x02 0x01 0xA8 0xF4 0x48 0x64 0xA4 0x70 0x55 0x40
// 0x00 0x00 0x00 0x00

// rawMessage AE =
// 0x49 0x00 0x00 0x00 0x00 0x00 0x92 0x3B 0x0B 0x26 0x3A 0x07 0x00 0x00 0x00 0x13
// 0x40 0x92 0x3B 0x0B 0x26 0x00 0x3A 0x07 0x10
// 0x61 0x4A 0xE1 0x01 0x75 0xCC 0x1B 0x5A 0x77 0xFF
// 0xB5 0x51 0xEF 0xDF
