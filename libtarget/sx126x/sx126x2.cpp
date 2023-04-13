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

void sx126x::initializeRadio() {
    setRfSwitch(rfSwitchState::off);

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

void sx126x::writeBuffer(uint8_t* payload, uint32_t payloadLength) {
    theLog.snprintf("Write Buffer : length = [%u], data = [", payloadLength);
    for (uint32_t index = 0; index < payloadLength; index++) {
        theLog.snprintf("%02X ", payload[index]);
    }
    theLog.snprintf("]\n");
    HAL_SUBGHZ_WriteBuffer(&hsubghz, 0U, payload, payloadLength);        // copy the raw LoRa message into the transmitBuffer of the SX126
}

void sx126x::readBuffer(uint8_t* payload, uint32_t payloadLength) {
    HAL_SUBGHZ_ReadBuffer(&hsubghz, 0U, payload, payloadLength);        // read the raw LoRa message which has been received from the SX126 into the receiveBuffer of the LoRaWAN stack
    theLog.snprintf("Read Buffer : length = [%u], data = [", payloadLength);
    for (uint32_t index = 0; index < payloadLength; index++) {
        theLog.snprintf("%02X ", payload[index]);
    }
    theLog.snprintf("]\n");
}

void sx126x::setRfSwitch(rfSwitchState newState) {
    theLog.snprintf("Set RF Switch [%s] \n", toString(newState));
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

void sx126x::executeCommand(sx126xCommand command, uint8_t* commandParameters, uint8_t commandParametersLength) {
    theLog.snprintf("Command [%02X] %s", static_cast<uint8_t>(command), toString(command));
    for (uint32_t index = 0; index < commandParametersLength; index++) {
        theLog.snprintf(" %02X", commandParameters[index]);
    }
    theLog.snprintf("\n");
    HAL_SUBGHZ_ExecSetCmd(&hsubghz, static_cast<SUBGHZ_RadioSetCmd_t>(command), commandParameters, commandParametersLength);
}

void sx126x::writeRegisters(sx126xRegister theRegister, uint8_t* data, uint8_t dataLength) {
    theLog.snprintf("Write Registers [%04X] %s", static_cast<uint16_t>(theRegister), toString(theRegister));
    for (uint32_t index = 0; index < dataLength; index++) {
        theLog.snprintf(" %02X", data[index]);
    }
    theLog.snprintf("\n");
    HAL_SUBGHZ_WriteRegisters(&hsubghz, static_cast<uint16_t>(theRegister), data, dataLength);
}
