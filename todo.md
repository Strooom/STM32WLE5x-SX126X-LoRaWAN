# TODO's before we can transmit LoRaWAN Messages

1. implement LoRaWAN::encryptApplicationPayload();
2. implement frameControl(linkDirection::uplink); constructor
3. implement frameHeader(devAddr, uplinkFrameControl, uplinkFrameCount); constructor
4. implement loraMessage::constructUplinkMessage(frameHeader theFrameHeader, framePort theFramePort, byteBuffer& theApplicationPayload, aesKey theKey)
5. implement bool loraMessage::transmit();
