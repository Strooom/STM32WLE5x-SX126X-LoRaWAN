#include "sx126xstatus.h"

// void sx126xStatus::fromUint8(uint8_t rawStatusByte) {
//     theChipMode      = static_cast<chipMode>((rawStatusByte & 0x70) >> 4);
//     theCommandStatus = static_cast<commandStatus>((rawStatusByte & 0x0E) >> 1);
// }

const char* toString(chipMode aChipMode) {
    switch (aChipMode) {
        case chipMode::standbyRc:
            return "standbyRc";
            break;
        case chipMode::standbyHseOscillator:
            return "standbyHseOscillator";
            break;
        case chipMode::frequencySynthesis:
            return "frequencySynthesis";
            break;
        case chipMode::receiving:
            return "receiving";
            break;
        case chipMode::transmitting:
            return "transmitting";
            break;
        default:
            return "unknown chipMode";
            break;
    }
}


const char* toString(commandStatus aCommandStatus){
    switch (aCommandStatus)
        {
    case commandStatus::commandTransmitDone:
        return "commandTransmitDone";
        break;
    case commandStatus::commandExecutionFailure:
        return "commandExecutionFailure";
        break;
    case commandStatus::commandProcessingError:
        return "commandProcessingError";
        break;
    case commandStatus::commandTimeOut:
        return "commandTimeOut";
        break;
    default:
        return "unknown commandStatus";
        break;
    }
}