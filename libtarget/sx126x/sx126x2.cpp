// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include "sx126x.h"
#include "main.h"        // this allows us to use the ST-HAL
// #include "stm32wle5.h"        // later we then factor it out by doing the HW access ourselves
#include "logging.h"
#include "powermodes.h"
#include "codingrate.h"
#include "spreadingfactor.h"
#include "bandwidth.h"

extern SUBGHZ_HandleTypeDef hsubghz;
extern logging theLog;

// peripheralRegister SPI_SX126X_CR1(reinterpret_cast<volatile uint32_t *const>(SUBGHZSPI_BASE + 0x00));
// peripheralRegister SPI_SX126X_CR2(reinterpret_cast<volatile uint32_t *const>(SUBGHZSPI_BASE + 0x04));
// peripheralRegister SPI_SX126X_SR(reinterpret_cast<volatile uint32_t *const>(SUBGHZSPI_BASE + 0x08));
// peripheralRegister SPI_SX126X_DR(reinterpret_cast<volatile uint32_t *const>(SUBGHZSPI_BASE + 0x0C));
// peripheralRegister SPI_SX126X_CRCPR(reinterpret_cast<volatile uint32_t *const>(SUBGHZSPI_BASE + 0x10));
// peripheralRegister SPI_SX126X_RXCRCR(reinterpret_cast<volatile uint32_t *const>(SUBGHZSPI_BASE + 0x14));
// peripheralRegister SPI_SX126X_CTXCRCR(reinterpret_cast<volatile uint32_t *const>(SUBGHZSPI_BASE + 0x18));
//
// extern peripheralRegister RCC_APB3ENR;
// extern peripheralRegister RCC_CSR;
// extern peripheralRegister PWR_SR2;

// Notes on the SX126x to STM32WLE interface
// The SX126x RESET is connected to RCC_CSR:15 (RFRST) for driving it, and RCC_CSR:14 (RFRSTF) for reading it
// The SX126x BUSY is connected to PWR_SR2:1 (RFBUSY)
// The SX126x SPI NSS
// * the functionality is enabled by PWR_CR1:3 (SUBGHZSPINSSSEL)
// * the NSS is driven by PWR_SUBGHZSPICR:14 (NSS)

// void sx126x::setPacketType(packetType thePacketType) {
//     constexpr uint8_t nmbrCommandParameters{1};                                                        //
//     uint8_t parametersIn[nmbrCommandParameters], dataOut[nmbrCommandParameters];                              //
//     parametersIn[0] = static_cast<uint8_t>(thePacketType);                                      //
//     executeCommand(sx126xCommand::setPacketType, parametersIn, dataOut, nmbrCommandParameters);        //
// }

// void sx126x::waitOnBusy() {
//  uint32_t waitCounter = maxWaitCounter;
//  while (waitCounter > 0) {
//      if (PWR_SR2 & RFBUSYS) {
//          return;
//      }
//      waitCounter--;
//  }
//  // TODO : log Busy timeout error
//  return;
//}

// bool sx126x::isStandby() {
//     theState = getStateFromHW();
//     return ((theState == sx126xState::standbyRc) || (theState == sx126xState::standbyXosc));
// }

void sx126x::executeCommand(sx126xCommand command, uint8_t *commandParameters, uint8_t commandParametersLength) {
    HAL_SUBGHZ_ExecSetCmd(&hsubghz, static_cast<SUBGHZ_RadioSetCmd_t>(command), commandParameters, 1);
}

void sx126x::writeRegisters(sx126xRegister theRegister, uint8_t *data, uint8_t dataLength) {
    HAL_SUBGHZ_WriteRegisters(&hsubghz, static_cast<uint16_t>(theRegister), data, dataLength);
}

// sx126xState sx126x::getStateFromHW() {
//     constexpr uint8_t nmbrCommandParameters{1};
//     uint8_t parametersIn[nmbrCommandParameters], dataOut[nmbrCommandParameters];
//     executeCommand(sx126xCommand::getStatus, parametersIn, dataOut, nmbrCommandParameters);

//     switch (dataOut[1]) {
//         default:
//             return sx126xState::boot;
//     }
// }

// sx126xError sx126x::getLastError() {
//     sx126xError result = lastError;
//     lastError          = sx126xError::none;
//     return result;
// }

bool sx126x::isBusy() {
    // PWR_SR2 register, bit 1
    return false; // TODO
}

void sx126x::setRegulatorMode() {
    // Check if in Standby Mode - you cannot change the regulator in other state
    // Also check OverCurrentProtection - default at 60 mA
    // EU 868 band also limits power to 16 dBm !! so maybe the DCDC is not needed in this case
    // Datasheet pg 76 : default the LDO is selected. This works but is not the most power efficient solution -> using the DCDC can be postponed for a next phase where we optimize power

    // constexpr uint8_t nmbrCommandParameters{1};                                                  //
    // uint8_t parametersIn[nmbrCommandParameters], dataOut[nmbrCommandParameters];                        //
    // parametersIn[0] = 0x00;                                                               // LDO-only mode
    // executeCommand(sx126xCommand::setRegulatorMode, parametersIn, nmbrCommandParameters);        //
}

void sx126x::reset() {


    // constexpr uint32_t RFRST{15};
    // constexpr uint32_t RFRSTF{14};
    // constexpr uint32_t RFBUSYS{1};

    // RCC_CSR.setBit(RFRSTF);          // activate reset
    //                                  // How long should we wait ? 1 ms
    // RCC_CSR.clearBit(RFRSTF);        // deactivate reset line

    // (void)SUBGHZ_WaitOnBusy(hsubghz);
}

void sx126x::initializeRadio() {
    // 1. Clocks
    // The SPI between STM32WLE and SX126x runs on the PCLK3 clock, divided by 2.
    // The SX126x-SPI interface has a maximum of 16 MhZ
    // As we are running the whole STM32WLE on 16 MHz (or lower), we can send the SYSCLK straight to the PCLK3 which means this SPI will run on 8 MHz
    //RCC_APB3ENR.setBit(0);
    // Wait a few clocks before accessing the peripheral

    // 2. Configure SPI1
    /*----------------------- SPI CR1 Configuration ----------------------------*
     *             SPI Mode: Master                                             *
     *   Communication Mode: 2 lines (Full-Duplex)                              *
     *       Clock polarity: Low                                                *
     *                phase: 1st Edge                                           *
     *       NSS management: Internal (Done with External bit inside PWR        *
     *  Communication speed: BaudratePrescaler                             *
     *            First bit: MSB                                                *
     *      CRC calculation: Disable                                            *
     *--------------------------------------------------------------------------*/
    //constexpr uint32_t MSTR{2};
    //SPI_SX126X_CR1.setBit(MSTR);        // All other bits are 0 and this is what we need
    /*----------------------- SPI CR2 Configuration ----------------------------*
     *            Data Size: 8bits                                              *
     *              TI Mode: Disable                                            *
     *            NSS Pulse: Disable                                            *
     *    Rx FIFO Threshold: 8bits                                              *
     *--------------------------------------------------------------------------*/
    //constexpr uint32_t FRXTH{12};
    //SPI_SX126X_CR2.setBit(FRXTH);
    // SPI_SX126X_CR2.writeBits(0xF << 8, 0x7 << 8); // 8 bits data size is already the default

    //constexpr uint32_t SPE{6};
    //SPI_SX126X_CR1.setBit(SPE); /* Enable SUBGHZSPI Peripheral */

    // 3. Configure the SX126x

    uint8_t commandParameters[8]{0};        // contains parameters when sending a command to the SX126x

    commandParameters[0] = static_cast<uint8_t>(sx126xPowerMode::DCDC);
    executeCommand(sx126xCommand::setRegulatorMode, commandParameters, 1);

    commandParameters[0] = 0x1E;
    writeRegisters(sx126xRegister::TxClampConfig, commandParameters, 1);

    commandParameters[0] = 0x38;
    writeRegisters(sx126xRegister::OCPConfiguration, commandParameters, 1);

    commandParameters[0] = 0x00;
    writeRegisters(sx126xRegister::XTAtrim, commandParameters, 1);

    commandParameters[0] = 0x01;
    commandParameters[1] = 0x00;
    commandParameters[2] = 0x00;
    commandParameters[3] = 0x40;
    executeCommand(sx126xCommand::setDIO3AsTcxoCtrl, commandParameters, 4);

    commandParameters[0] = 0x7F;
    executeCommand(sx126xCommand::calibrate, commandParameters, 1);

    commandParameters[0] = 0xD7;
    commandParameters[1] = 0xDB;
    executeCommand(sx126xCommand::calibrateImage, commandParameters, 2);

    commandParameters[0] = static_cast<uint8_t>(packetType::LoRa);
    executeCommand(sx126xCommand::setPacketType, commandParameters, 1);

    commandParameters[0] = 0x36;
    commandParameters[1] = 0x41;
    commandParameters[2] = 0x99;
    commandParameters[3] = 0x99;
    executeCommand(sx126xCommand::setRfFRequency, commandParameters, 4);

    commandParameters[0] = 0x02;
    commandParameters[1] = 0x03;
    commandParameters[2] = 0x00;
    commandParameters[3] = 0x01;
    executeCommand(sx126xCommand::setPaConfig, commandParameters, 4);

    commandParameters[0] = 0x0D;        // PA optimal setting and operating modes: 14 dBm HighPower mode
    commandParameters[1] = 0x02;        // RampTime
    executeCommand(sx126xCommand::setTxParams, commandParameters, 2);

    commandParameters[0] = 0x34;        // LoRa Syncword MSB
    commandParameters[1] = 0x44;        // LoRa Syncword LSB
    writeRegisters(sx126xRegister::LoRaSyncWordMSB, commandParameters, 2);
}

void sx126x::initializeModem() {
    constexpr uint8_t nmbrCommandParameters{4};
    uint8_t commandParameters[nmbrCommandParameters]{0};
    commandParameters[0] = static_cast<uint8_t>(spreadingFactor::SF7);
    commandParameters[1] = static_cast<uint8_t>(bandwidth::b125kHz);
    commandParameters[2] = static_cast<uint8_t>(codingRate::cr4_5);
    commandParameters[3] = 0x00;        // Low DataRate Optimization Off
    executeCommand(sx126xCommand::setModulationParams, commandParameters, nmbrCommandParameters);
}

void sx126x::goSleep(sleepMode theSleepMode) {
    constexpr uint8_t nmbrCommandParameters{1};
    uint8_t commandParameters[nmbrCommandParameters]{static_cast<uint8_t>(theSleepMode)};
    executeCommand(sx126xCommand::setSleep, commandParameters, nmbrCommandParameters);
}

void sx126x::goStandby(standbyMode theStandbyMode) {
    constexpr uint8_t nmbrCommandParameters{1};
    uint8_t commandParameters[nmbrCommandParameters]{static_cast<uint8_t>(theStandbyMode)};
    executeCommand(sx126xCommand::setStandby, commandParameters, nmbrCommandParameters);
}

void sx126x::setRfFrequency(uint32_t frequencyInHz) {
    constexpr uint8_t nmbrCommandParameters{4};
    uint32_t frequencyRegisterValue = calculateFrequencyRegisterValue(frequencyInHz);
    uint8_t commandParameters[nmbrCommandParameters];
    commandParameters[0] = static_cast<uint8_t>((frequencyRegisterValue >> 24) & 0xFF);        //
    commandParameters[1] = static_cast<uint8_t>((frequencyRegisterValue >> 16) & 0xFF);        //
    commandParameters[2] = static_cast<uint8_t>((frequencyRegisterValue >> 8) & 0xFF);         //
    commandParameters[3] = static_cast<uint8_t>((frequencyRegisterValue)&0xFF);                //
    executeCommand(sx126xCommand::setRfFRequency, commandParameters, nmbrCommandParameters);
}

void sx126x::setTxParameters(int8_t transmitPowerdBm, rampTime theRamptime) {        // caution - signed int8, negative dBm values are in two's complement
    constexpr uint8_t nmbrCommandParameters{2};
    uint8_t commandParameters[nmbrCommandParameters];

    if (transmitPowerdBm < 0) {
        commandParameters[0] = 255 - static_cast<uint8_t>(-(transmitPowerdBm + 1));        // magic to turn negative signed value into a raw byte in two's complement
    } else {
        commandParameters[0] = static_cast<uint8_t>(transmitPowerdBm);        // if value is not negative, casting it is safe
    }
    commandParameters[1] = static_cast<uint8_t>(theRamptime);        // rampTime 200 uS - no info why this value, but this was in the demo application from ST / Semtech
                                                                     // the remaining 4 bytes are empty 0x00 for LoRa
    executeCommand(sx126xCommand::setTxParams, commandParameters, nmbrCommandParameters);
}

void sx126x::setModulationParameters(spreadingFactor theSpreadingFactor, bandwidth theBandwidth, codingRate theCodingRate) {
    constexpr uint8_t nmbrCommandParameters{8};
    uint8_t commandParameters[nmbrCommandParameters]{0};
    commandParameters[0] = static_cast<uint8_t>(theSpreadingFactor);        //
    commandParameters[1] = static_cast<uint8_t>(theBandwidth);              //
    commandParameters[2] = static_cast<uint8_t>(theCodingRate);             //
    commandParameters[3] = 0x01;                                            // TODO ?? LowDatarateOptimize // why would you NOT want to do this ??
                                                                            // the remaining 4 bytes are empty 0x00 for LoRa
    executeCommand(sx126xCommand::setPacketParameters, commandParameters, nmbrCommandParameters);
}

void sx126x::setPacketParameters(uint8_t payloadLength) {
    constexpr uint8_t nmbrCommandParameters{9};
    uint8_t commandParameters[nmbrCommandParameters]{0};
    commandParameters[0] = 0x00;                 // MSB for PreambleLength
    commandParameters[1] = 0x08;                 // LSB for PreambleLength LoRaWAN is fixed to 8 symbols
    commandParameters[2] = 0x00;                 // Variable length packet (explicit header)
    commandParameters[3] = payloadLength;        // Payload Length
    commandParameters[4] = 0x01;                 // CRC On ??
    commandParameters[5] = 0x00;                 // Standard IQ : for uplink. Downlink requires inverted IQ
                                                 // the remaining 3 bytes are empty 0x00 for LoRa
    executeCommand(sx126xCommand::setPacketParameters, commandParameters, nmbrCommandParameters);
}

void sx126x::setPowerAmplifierConfig(uint8_t paDutyCycle, uint8_t hpMax, uint8_t deviceSel, uint8_t paLut) {
    // Guessing from the STM32WLE datasheet, there is an SX1262 inside, ie. the high power version, with up to 22 dBm transmit power.
    // However, in Europe, ETSI limits the power to 16 dBm anyway, so it seems like useing the extra power of the SX1262 vs the SX1261 is not allowed...

    constexpr uint8_t nmbrCommandParameters{4};
    uint8_t commandParameters[nmbrCommandParameters];
    commandParameters[0] = 0x06;        // paDutyCycle - +15 dBm
    commandParameters[1] = 0x00;        // hpMax - no effect in case of SX1261 mode, only for SX1262
    commandParameters[2] = 0x01;        // 0x01 = SX1261, 0x00 = SX1262 mode
    commandParameters[3] = 0x01;        // reserved and always 0x01

    executeCommand(sx126xCommand::setPaConfig, commandParameters, nmbrCommandParameters);
}

// void sx126x::writeBuffer(uint8_t *data, uint8_t dataLength, uint8_t startAddressOffset = 0) {
//  Problem : we need to insert the offset byte into the stream of data... maybe we need to make a local copy, but this buffer can be 256 bytes long..
//  executeCommand(sx126xCommand::writeBuffer, parametersIn, dataOut, nmbrCommandParameters);
//}

void sx126x::writeBuffer(byteBuffer rawMessage) {
    // copy the raw LoRa message into the transmitBuffer
}

// uint32_t sx126x::readBuffer(uint8_t *data, uint8_t dataLength, uint8_t startAddressOffset = 0) {
//  Problem : we need to insert the offset byte into the stream of data... maybe we need to make a local copy, but this buffer can be 256 bytes long..
//  executeCommand(sx126xCommand::writeBuffer, parametersIn, dataOut, nmbrCommandParameters);
//}

void sx126x::readBuffer(byteBuffer rawMessage) {
    // read the raw LoRa message which has been received
}

void sx126x::setRfSwitch(rfSwitchState newState) {
    switch (newState) {
        case rfSwitchState::off:
        default:
            HAL_GPIO_WritePin(GPIOA, rfControl1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, rfControl2_Pin, GPIO_PIN_RESET);
            break;

        case rfSwitchState::tx:
            HAL_GPIO_WritePin(GPIOA, rfControl1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, rfControl2_Pin, GPIO_PIN_SET);
            break;

        case rfSwitchState::rx:
            HAL_GPIO_WritePin(GPIOA, rfControl1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, rfControl2_Pin, GPIO_PIN_RESET);
            break;
    }
}
