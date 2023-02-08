#pragma once
#include <stdint.h>

class loraMessage {};        // aka PHYPayload

class macHeader {
  public:
    static constexpr uint32_t length{1};        // [bytes]
  private:
    frameType theFrameType;                                    // bits [7:5]
    static constexpr uint32_t majorDataFrameVersion{0};        // bits [1:0]
};

class macPayload {
  public:
    uint32_t maximumLength{0};        // this is region and data-rate dependent - see regional parameters
  private:
};

class messageIntegrityCode {
  public:
    static constexpr uint32_t length{4};        // [bytes]
  private:
};

class frameHeader {};

class framePort {};

class framePayload {};

class deviceAddress {};

class frameControl {};

class frameCount {};

class frameOptions {};

enum class frameType : uint8_t {
    joinRequest             = 0x00,
    joinAccept              = 0x01,
    unconfirmedDataUplink   = 0x02,
    unconfirmedDataDownlink = 0x03,
    confirmedDataUplink     = 0x04,
    confirmedDataDownlink   = 0x05
};