#include "sx126xregisters.h"

const char* toString(sx126xRegister aRegister) {
    switch (aRegister) {
        case sx126xRegister::LoRaSyncWordMSB:
            return "LoRaSyncWordMSB";
            break;
        case sx126xRegister::LoRaSyncWordLSB:
            return "LoRaSyncWordLSB";
            break;
        case sx126xRegister::TxClampConfig:
            return "TxClampConfig";
            break;
        case sx126xRegister::OCPConfiguration:
            return "OCPConfiguration";
            break;
        case sx126xRegister::XTAtrim:
            return "XTAtrim";
            break;
        case sx126xRegister::DIO3outputVoltageControl:
            return "DIO3outputVoltageControl";
            break;

        default:
            return "unknown register";
    }
}
