#include "rfswitch.h"

const char* toString(rfSwitchState aState){
    switch (aState)
    {
    case rfSwitchState::off:
        return "off";
        break;
    case rfSwitchState::tx:
        return "tx";
        break;
    case rfSwitchState::rx:
        return "rx";
        break;
    default:
        return "unknown";
        break;
    }
}