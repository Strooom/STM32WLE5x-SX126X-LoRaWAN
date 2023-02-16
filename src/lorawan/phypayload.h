// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class frameType : uint8_t {
    joinRequest             = 0x00,
    joinAccept              = 0x01,
    unconfirmedDataUplink   = 0x02,
    unconfirmedDataDownlink = 0x03,
    confirmedDataUplink     = 0x04,
    confirmedDataDownlink   = 0x05
};


class phyPayload {
  public:
  static constexpr uint32_t maxPayloadLength{256};
  private:
    frameType macHeader;
    uint8_t payload[maxPayloadLength]{0};
    uint8_t fCtrl;
}