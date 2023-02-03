#pragma once

#include "stdint.h"

class loRaBandwidth

{
private:
enum class bandwidth : uint32_t {
loRaBandwidth500,
loRaBandwidth500
} theBandwidth;


public:
    loRaBandwidth
    (/* args */);
    ~loRaBandwidth
    ();
};

loRaBandwidth
::loRaBandwidth
(/* args */)
{
}

loRaBandwidth
::~loRaBandwidth
()
{
}


operationMode
startup,
sleep,
standbyRc,
standbyXOsc,
frequencySynthesisMode,
transmitMode,
receiveMode

