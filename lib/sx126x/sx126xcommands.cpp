#include "sx126xcommands.h"

const char* toString(sx126xCommand command) {
    switch (command) {
        case sx126xCommand::setSleep:
            return "setSleep";
            break;
        case sx126xCommand::setStandby:
            return "setStandby";
            break;
        case sx126xCommand::setFs:
            return "setFs";
            break;
        case sx126xCommand::setTx:
            return "setTx";
            break;
        case sx126xCommand::setRx:
            return "setRx";
            break;
        case sx126xCommand::calibrate:
            return "calibrate";
            break;
        case sx126xCommand::getDeviceErrors:
            return "GetDeviceErrors";
            break;
        case sx126xCommand::clearDeviceErrors:
            return "clearDeviceErrors";
            break;
        case sx126xCommand::calibrateImage:
            return "calibrateImage";
            break;
        case sx126xCommand::setRegulatorMode:
            return "SetRegulatorMode";
            break;
        case sx126xCommand::setPaConfig:
            return "SetPaConfig";
            break;
        case sx126xCommand::writeRegister:
            return "writeRegister";
            break;
        case sx126xCommand::readRegister:
            return "readRegister";
            break;
        case sx126xCommand::writeBuffer:
            return "writeBuffer";
            break;
        case sx126xCommand::readBuffer:
            return "readBuffer";
            break;
        case sx126xCommand::getStatus:
            return "getStatus";
            break;
        case sx126xCommand::setRfFRequency:
            return "setRfFRequency";
            break;
        case sx126xCommand::setPacketType:
            return "setPacketType";
            break;
        case sx126xCommand::getPacketType:
            return "getPacketType";
            break;
        case sx126xCommand::setTxParams:
            return "setTxParams";
            break;
        case sx126xCommand::setModulationParams:
            return "setModulationParams";
            break;
        case sx126xCommand::setPacketParameters:
            return "setPacketParameters";
            break;
        case sx126xCommand::setCadParams:
            return "setCadParams";
            break;
        case sx126xCommand::setBufferBaseAddress:
            return "setBufferBaseAddress";
            break;
        default:
            return "unknown command";
            break;
    }
}
