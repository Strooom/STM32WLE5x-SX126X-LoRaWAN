// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include "sx126x.h"
#include "logging.h"

void sx126x::initialize() {
    initializeInterface();
    initializeRadio();
    goSleep();
}

void sx126x::configForTransmit(spreadingFactor theSpreadingFactor, uint32_t frequency, uint8_t* payload, uint32_t payloadLength) {
    logging::snprintf(loggingSource::sx126xControl, "Configuring for Transmit : [%s], [%u] Hz [%u] bytes\n", toString(theSpreadingFactor), frequency, payloadLength);
    goStandby();
    setRfSwitch(rfSwitchState::tx);
    setRfFrequency(frequency);
    setModulationParameters(theSpreadingFactor, bandwidth::b125kHz);
    setPacketParametersTransmit(payloadLength);
    writeBuffer(payload, payloadLength);
    uint8_t commandParameters[8];
    commandParameters[0] = 0x02;
    commandParameters[1] = 0x01;
    commandParameters[2] = 0x02;
    commandParameters[3] = 0x01;
    commandParameters[4] = 0x00;
    commandParameters[5] = 0x00;
    commandParameters[6] = 0x00;
    commandParameters[7] = 0x00;
    executeSetCommand(sx126xCommand::setDioIrqParams, commandParameters, 8);
}

void sx126x::configForReceive(spreadingFactor theSpreadingFactor, uint32_t frequency) {
    logging::snprintf(loggingSource::sx126xControl, "Configuring for Receive : [%s], [%u] Hz\n", toString(theSpreadingFactor), frequency);
    goStandby();
    setRfSwitch(rfSwitchState::rx);
    setRfFrequency(frequency);
    setModulationParameters(theSpreadingFactor, bandwidth::b125kHz);
    setPacketParametersReceive();
    uint8_t commandParameters[8];
    commandParameters[0] = 0x02;
    commandParameters[1] = 0x02;
    commandParameters[2] = 0x02;
    commandParameters[3] = 0x02;
    commandParameters[4] = 0x00;
    commandParameters[5] = 0x00;
    commandParameters[6] = 0x00;
    commandParameters[7] = 0x00;
    executeSetCommand(sx126xCommand::setDioIrqParams, commandParameters, 8);
}

void sx126x::goSleep(sleepMode theSleepMode) {
    logging::snprintf(loggingSource::sx126xControl, "goSleep [%u]\n", static_cast<uint8_t>(theSleepMode));
    setRfSwitch(rfSwitchState::off);
    constexpr uint8_t nmbrCommandParameters{1};
    uint8_t commandParameters[nmbrCommandParameters]{static_cast<uint8_t>(theSleepMode)};
    executeSetCommand(sx126xCommand::setSleep, commandParameters, nmbrCommandParameters);
}

void sx126x::goStandby(standbyMode theStandbyMode) {
    logging::snprintf(loggingSource::sx126xControl, "goStandby [%u]\n", static_cast<uint8_t>(theStandbyMode));
    constexpr uint8_t nmbrCommandParameters{1};
    uint8_t commandParameters[nmbrCommandParameters]{static_cast<uint8_t>(theStandbyMode)};
    executeSetCommand(sx126xCommand::setStandby, commandParameters, nmbrCommandParameters);
}

void sx126x::startTransmit(uint32_t timeOut) {
    logging::snprintf(loggingSource::sx126xControl, "startTx [%u]\n", timeOut);
    constexpr uint8_t nmbrCommandParameters{3};
    uint8_t commandParameters[nmbrCommandParameters];
    commandParameters[0] = static_cast<uint8_t>((timeOut >> 16) & 0xFF);
    commandParameters[1] = static_cast<uint8_t>((timeOut >> 8) & 0xFF);
    commandParameters[2] = static_cast<uint8_t>(timeOut & 0xFF);
    executeSetCommand(sx126xCommand::setTx, commandParameters, nmbrCommandParameters);
    // HAL_GPIO_WritePin(GPIOA, loraTiming_Pin, GPIO_PIN_SET); // Set pin high to monitor timing
}

void sx126x::startReceive(uint32_t timeOut) {
    timeOut = 3840;        // TODO : make this a funtion of the DataRate.. currently it is fixed to 60 ms, which should be ok for DR5
    logging::snprintf(loggingSource::sx126xControl, "startRx [%u]\n", timeOut);
    constexpr uint8_t nmbrCommandParameters{3};
    uint8_t commandParameters[nmbrCommandParameters];
    commandParameters[0] = static_cast<uint8_t>((timeOut >> 16) & 0xFF);
    commandParameters[1] = static_cast<uint8_t>((timeOut >> 8) & 0xFF);
    commandParameters[2] = static_cast<uint8_t>(timeOut & 0xFF);
    executeSetCommand(sx126xCommand::setRx, commandParameters, nmbrCommandParameters);
    // HAL_GPIO_WritePin(GPIOA, loraTiming_Pin, GPIO_PIN_SET); // Set pin high to monitor timing
}

void sx126x::setRfFrequency(uint32_t frequencyInHz) {
    logging::snprintf(loggingSource::sx126xControl, "setRfFrequency [%u]\n", frequencyInHz);
    constexpr uint8_t nmbrCommandParameters{4};
    uint32_t frequencyRegisterValue = calculateFrequencyRegisterValue(frequencyInHz);
    uint8_t commandParameters[nmbrCommandParameters];
    commandParameters[0] = static_cast<uint8_t>((frequencyRegisterValue >> 24) & 0xFF);        //
    commandParameters[1] = static_cast<uint8_t>((frequencyRegisterValue >> 16) & 0xFF);        //
    commandParameters[2] = static_cast<uint8_t>((frequencyRegisterValue >> 8) & 0xFF);         //
    commandParameters[3] = static_cast<uint8_t>((frequencyRegisterValue)&0xFF);                //
    executeSetCommand(sx126xCommand::setRfFRequency, commandParameters, nmbrCommandParameters);
}

void sx126x::setModulationParameters(spreadingFactor theSpreadingFactor, bandwidth theBandwidth) {
    logging::snprintf(loggingSource::sx126xControl, "setModulationParameters [%s], [%s]\n", toString(theSpreadingFactor), toString(theBandwidth));
    constexpr uint8_t nmbrCommandParameters{8};
    uint8_t commandParameters[nmbrCommandParameters]{0};
    commandParameters[0] = static_cast<uint8_t>(theSpreadingFactor);        //
    commandParameters[1] = static_cast<uint8_t>(theBandwidth);              //
    commandParameters[2] = 0x01;                                            // LoRaWAN is fixed to coding rate 4/5
    commandParameters[3] = 0x00;                                            // TODO ?? LowDatarateOptimize // why would you NOT want to do this ??
                                                                            // the remaining 4 bytes are empty 0x00 for LoRa modulation
    executeSetCommand(sx126xCommand::setModulationParams, commandParameters, nmbrCommandParameters);
}

void sx126x::setPacketParametersTransmit(uint8_t payloadLength) {
    logging::snprintf(loggingSource::sx126xControl, "setPacketParametersTransmit [%u]\n", payloadLength);
    constexpr uint8_t nmbrCommandParameters{9};
    uint8_t commandParameters[nmbrCommandParameters]{0};
    commandParameters[0] = 0x00;                 // MSB for PreambleLength
    commandParameters[1] = 0x08;                 // LSB for PreambleLength LoRaWAN is fixed to 8 symbols
    commandParameters[2] = 0x00;                 // Variable length packet (explicit header)
    commandParameters[3] = payloadLength;        // Payload Length
    commandParameters[4] = 0x01;                 // CRC On
    commandParameters[5] = 0x00;                 // Standard IQ for uplink
                                                 // the remaining 3 bytes are empty 0x00 for LoRa
    executeSetCommand(sx126xCommand::setPacketParameters, commandParameters, nmbrCommandParameters);
}

void sx126x::setPacketParametersReceive() {
    logging::snprintf(loggingSource::sx126xControl, "setPacketParametersReceive\n");
    constexpr uint8_t nmbrCommandParameters{9};
    uint8_t commandParameters[nmbrCommandParameters]{0};
    commandParameters[0] = 0x00;        // MSB for PreambleLength
    commandParameters[1] = 0x08;        // LSB for PreambleLength LoRaWAN is fixed to 8 symbols
    commandParameters[2] = 0x00;        // Variable length packet (explicit header)
    commandParameters[3] = 0xFF;        // Maximum Payload Length
    commandParameters[4] = 0x00;        // CRC Off
    commandParameters[5] = 0x01;        // Downlink requires inverted IQ
                                        // the remaining 3 bytes are empty 0x00 for LoRa
    executeSetCommand(sx126xCommand::setPacketParameters, commandParameters, nmbrCommandParameters);
}

uint32_t sx126x::calculateFrequencyRegisterValue(uint32_t rfFrequency) {
    uint64_t tmpResult;
    tmpResult = static_cast<uint64_t>(rfFrequency) << 25;
    tmpResult = tmpResult / crystalFrequency;
    return static_cast<uint32_t>(tmpResult);
}

void sx126x::setTxParameters(int8_t transmitPowerdBm, rampTime theRamptime) {        // caution - signed int8, negative dBm values are in two's complement
    logging::snprintf(loggingSource::sx126xControl, "setTxParameters [%d], [%s]\n", transmitPowerdBm, toString(theRamptime));
    constexpr uint8_t nmbrCommandParameters{2};
    uint8_t commandParameters[nmbrCommandParameters];

    union {
        int8_t asInt8;
        uint8_t asUint8;
    } txPower;

    txPower.asInt8 = transmitPowerdBm;

    // if (transmitPowerdBm < 0) {
    //     commandParameters[0] = 255 - static_cast<uint8_t>(-(transmitPowerdBm + 1));        // magic to turn negative signed value into a raw byte in two's complement
    // } else {
    //     commandParameters[0] = static_cast<uint8_t>(transmitPowerdBm);                     // if value is not negative, casting it is safe
    // }

    commandParameters[0] = txPower.asUint8;
    commandParameters[1] = static_cast<uint8_t>(theRamptime);        // rampTime 200 uS - no info why this value, but this was in the demo application from ST / Semtech
                                                                     // the remaining 4 bytes are empty 0x00 for LoRa
    executeSetCommand(sx126xCommand::setTxParams, commandParameters, nmbrCommandParameters);
}

void sx126x::initializeRadio() {
    setRfSwitch(rfSwitchState::off);
    uint8_t commandParameters[8]{0};        // contains parameters when sending a command to the SX126x
    uint8_t response[8]{0};                 // contains response for a get command

    executeGetCommand(sx126xCommand::getStatus, commandParameters, 1);

    commandParameters[0] = 0x01;        // DCDC powermode
    executeSetCommand(sx126xCommand::setRegulatorMode, commandParameters, 1);

    // TODO : add SMPS maximum drive capability 60 mA (io 100mA default)
    //    commandParameters[0] = 0x02;
    //    writeRegisters(sx126xRegister::smpsMaximumDrive, commandParameters, 1);

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
    executeSetCommand(sx126xCommand::setDIO3AsTcxoCtrl, commandParameters, 4);

    commandParameters[0] = 0x7F;
    executeSetCommand(sx126xCommand::calibrate, commandParameters, 1);

    commandParameters[0] = 0xD7;
    commandParameters[1] = 0xDB;
    executeSetCommand(sx126xCommand::calibrateImage, commandParameters, 2);

    commandParameters[0] = 0x01;        // packetType::LoRa;
    executeSetCommand(sx126xCommand::setPacketType, commandParameters, 1);

    commandParameters[0] = 0x36;
    commandParameters[1] = 0x41;
    commandParameters[2] = 0x99;
    commandParameters[3] = 0x99;
    executeSetCommand(sx126xCommand::setRfFRequency, commandParameters, 4);

    commandParameters[0] = 0x02;
    commandParameters[1] = 0x03;
    commandParameters[2] = 0x00;
    commandParameters[3] = 0x01;
    executeSetCommand(sx126xCommand::setPaConfig, commandParameters, 4);

    commandParameters[0] = 0x0D;        // PA optimal setting and operating modes: 14 dBm HighPower mode
    commandParameters[1] = 0x02;        // RampTime
    executeSetCommand(sx126xCommand::setTxParams, commandParameters, 2);

    commandParameters[0] = 0x34;        // LoRa Syncword MSB
    commandParameters[1] = 0x44;        // LoRa Syncword LSB
    writeRegisters(sx126xRegister::LoRaSyncWordMSB, commandParameters, 2);

    //goSleep();

    //executeGetCommand(sx126xCommand::getStatus, commandParameters, 1);
}

#ifndef environment_desktop

#include "main.h"        // added only for timing tests
extern RTC_HandleTypeDef hrtc;

// #define showSpiCommunicationControl
#define showSpiCommunicationData

extern SUBGHZ_HandleTypeDef hsubghz;

// Notes on the SX126x to STM32WLE interface
// The SX126x RESET is connected to RCC_CSR:15 (RFRST) for driving it, and RCC_CSR:14 (RFRSTF) for reading it
// The SX126x BUSY is connected to PWR_SR2:1 (RFBUSY)
// The SX126x SPI NSS
// * the functionality is enabled by PWR_CR1:3 (SUBGHZSPINSSSEL)
// * the NSS is driven by PWR_SUBGHZSPICR:14 (NSS)
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
// constexpr uint32_t MSTR{2};
// SPI_SX126X_CR1.setBit(MSTR);        // All other bits are 0 and this is what we need
/*----------------------- SPI CR2 Configuration ----------------------------*
 *            Data Size: 8bits                                              *
 *              TI Mode: Disable                                            *
 *            NSS Pulse: Disable                                            *
 *    Rx FIFO Threshold: 8bits                                              *
 *--------------------------------------------------------------------------*/
// constexpr uint32_t FRXTH{12};
// SPI_SX126X_CR2.setBit(FRXTH);
//  SPI_SX126X_CR2.writeBits(0xF << 8, 0x7 << 8); // 8 bits data size is already the default
// constexpr uint32_t SPE{6};
// SPI_SX126X_CR1.setBit(SPE); /* Enable SUBGHZSPI Peripheral */
// peripheralRegister SPI_SX126X_CR1(reinterpret_cast<volatile uint32_t *const>(SUBGHZSPI_BASE + 0x00));
// peripheralRegister SPI_SX126X_CR2(reinterpret_cast<volatile uint32_t *const>(SUBGHZSPI_BASE + 0x04));
// peripheralRegister SPI_SX126X_SR(reinterpret_cast<volatile uint32_t *const>(SUBGHZSPI_BASE + 0x08));
// peripheralRegister SPI_SX126X_DR(reinterpret_cast<volatile uint32_t *const>(SUBGHZSPI_BASE + 0x0C));
// peripheralRegister SPI_SX126X_CRCPR(reinterpret_cast<volatile uint32_t *const>(SUBGHZSPI_BASE + 0x10));
// peripheralRegister SPI_SX126X_RXCRCR(reinterpret_cast<volatile uint32_t *const>(SUBGHZSPI_BASE + 0x14));
// peripheralRegister SPI_SX126X_CTXCRCR(reinterpret_cast<volatile uint32_t *const>(SUBGHZSPI_BASE + 0x18));
// extern peripheralRegister RCC_APB3ENR;
// extern peripheralRegister RCC_CSR;
// extern peripheralRegister PWR_SR2;

void sx126x::initializeInterface() {
    HAL_SUBGHZ_Init(&hsubghz);
}

// void sx126x::reset() {
// constexpr uint32_t RFRST{15};
// constexpr uint32_t RFRSTF{14};
// constexpr uint32_t RFBUSYS{1};
// RCC_CSR.setBit(RFRSTF);          // activate reset
//                                  // How long should we wait ? 1 ms
// RCC_CSR.clearBit(RFRSTF);        // deactivate reset line
// (void)SUBGHZ_WaitOnBusy(hsubghz);
//}

void sx126x::writeBuffer(uint8_t* payload, uint32_t payloadLength) {
    if (logging::isActive(loggingSource::sx126xBufferData)) {
        RTC_TimeTypeDef currTime = {0};
        RTC_DateTypeDef currDate = {0};

        HAL_RTC_GetTime(&hrtc, &currTime, RTC_FORMAT_BIN);
        HAL_RTC_GetDate(&hrtc, &currDate, RTC_FORMAT_BIN);
        logging::snprintf("%02u:%02u:%02u Tx[%u] ", currTime.Hours, currTime.Minutes, currTime.Seconds, payloadLength);
        for (uint32_t index = 0; index < payloadLength; index++) {
            logging::snprintf("%02X ", payload[index]);
        }
        logging::snprintf("\n");
    }
    HAL_SUBGHZ_WriteBuffer(&hsubghz, 0U, payload, payloadLength);        // copy the raw LoRa message into the transmitBuffer of the SX126
}

void sx126x::readBuffer(uint8_t* payload, uint32_t payloadLength) {
    HAL_SUBGHZ_ReadBuffer(&hsubghz, 0U, payload, payloadLength);        // read the raw LoRa message which has been received from the SX126 into the receiveBuffer of the LoRaWAN stack
    if (logging::isActive(loggingSource::sx126xBufferData)) {
        RTC_TimeTypeDef currTime = {0};
        RTC_DateTypeDef currDate = {0};

        HAL_RTC_GetTime(&hrtc, &currTime, RTC_FORMAT_BIN);
        HAL_RTC_GetDate(&hrtc, &currDate, RTC_FORMAT_BIN);
        logging::snprintf("%02u:%02u:%02u Rx[%u] ", currTime.Hours, currTime.Minutes, currTime.Seconds, payloadLength);
        for (uint32_t index = 0; index < payloadLength; index++) {
            logging::snprintf("%02X ", payload[index]);
        }
        logging::snprintf("\n");
    }
}

void sx126x::setRfSwitch(rfSwitchState newState) {
    logging::snprintf(loggingSource::sx126xControl, "Set RF Switch [%s] \n", toString(newState));
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

void sx126x::executeSetCommand(sx126xCommand command, uint8_t* commandParameters, uint8_t commandParametersLength) {
    if (logging::isActive(loggingSource::sx126xControl)) {
        logging::snprintf("Command [%02X] %s : ", static_cast<uint8_t>(command), toString(command));
        for (uint32_t index = 0; index < commandParametersLength; index++) {
            logging::snprintf(" %02X", commandParameters[index]);
        }
        logging::snprintf("\n");
    }
    HAL_SUBGHZ_ExecSetCmd(&hsubghz, static_cast<SUBGHZ_RadioSetCmd_t>(command), commandParameters, commandParametersLength);
}

void sx126x::executeGetCommand(sx126xCommand command, uint8_t* responseData, uint8_t responseDataLength) {
    HAL_SUBGHZ_ExecGetCmd(&hsubghz, static_cast<SUBGHZ_RadioGetCmd_t>(command), responseData, responseDataLength);
    if (logging::isActive(loggingSource::sx126xControl)) {
        logging::snprintf("Command [%02X] %s : ", static_cast<uint8_t>(command), toString(command));
        for (uint32_t index = 0; index < responseDataLength; index++) {
            logging::snprintf(" %02X", responseData[index]);
        }
        logging::snprintf("\n");
    }
}

void sx126x::writeRegisters(sx126xRegister theRegister, uint8_t* data, uint8_t dataLength) {
    if (logging::isActive(loggingSource::sx126xControl)) {
        logging::snprintf("Write Registers [%04X] %s : ", static_cast<uint16_t>(theRegister), toString(theRegister));
        for (uint32_t index = 0; index < dataLength; index++) {
            logging::snprintf(" %02X", data[index]);
        }
        logging::snprintf("\n");
    }
    HAL_SUBGHZ_WriteRegisters(&hsubghz, static_cast<uint16_t>(theRegister), data, dataLength);
}

#else
#include <cstring>

uint8_t mockSx126xDataBuffer[256];          // emulates the SX126x data buffer so we can do some unit test on it

void sx126x::initializeInterface() {        // TODO : this is currently not doing anything ???
}

void sx126x::writeBuffer(uint8_t* payload, uint32_t payloadLength) {
    memcpy(mockSx126xDataBuffer, payload, payloadLength);
}

void sx126x::readBuffer(uint8_t* payload, uint32_t payloadLength) {
    memcpy(payload, mockSx126xDataBuffer, payloadLength);
}

void sx126x::setRfSwitch(rfSwitchState newState) {
}

void sx126x::executeSetCommand(sx126xCommand command, uint8_t* commandParameters, uint8_t commandParametersLength) {
}

void sx126x::writeRegisters(sx126xRegister theRegister, uint8_t* data, uint8_t dataLength) {
}

void sx126x::executeGetCommand(sx126xCommand command, uint8_t* responseData, uint8_t responseDataLength) {}

#endif
