#include "sx126xstatus.h"

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