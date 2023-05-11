#include "macpayload.h"

void macPayload::set(frameHeader theFrameHeader, framePort theFramePort, byteBuffer applicationPayload) {
    FHDR = theFrameHeader;
    FPort = theFramePort;
    FRMPayload = applicationPayload;
}
