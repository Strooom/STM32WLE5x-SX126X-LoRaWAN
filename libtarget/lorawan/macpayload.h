// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>
#include "../general/bytebuffer.h"
#include "frameheader.h"
#include "frameport.h"

class macPayload {
  public:
    void set(frameHeader theFrameHeader, framePort theFramePort, byteBuffer applicationPayload);
    uint8_t *get();
    uint32_t getLength();

  private:
    frameHeader FHDR;
    framePort FPort;
    byteBuffer FRMPayload;
};
