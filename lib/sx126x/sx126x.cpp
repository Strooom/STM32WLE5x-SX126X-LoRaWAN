// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include "sx126x.h"

peripheralRegister SPI_SX126X_CR1(reinterpret_cast<volatile uint32_t *const>(SUBGHZSPI_BASE + 0x00));
peripheralRegister SPI_SX126X_CR2(reinterpret_cast<volatile uint32_t *const>(SUBGHZSPI_BASE + 0x04));
peripheralRegister SPI_SX126X_SR(reinterpret_cast<volatile uint32_t *const>(SUBGHZSPI_BASE + 0x08));
peripheralRegister SPI_SX126X_DR(reinterpret_cast<volatile uint32_t *const>(SUBGHZSPI_BASE + 0x0C));
peripheralRegister SPI_SX126X_CRCPR(reinterpret_cast<volatile uint32_t *const>(SUBGHZSPI_BASE + 0x10));
peripheralRegister SPI_SX126X_RXCRCR(reinterpret_cast<volatile uint32_t *const>(SUBGHZSPI_BASE + 0x14));
peripheralRegister SPI_SX126X_CTXCRCR(reinterpret_cast<volatile uint32_t *const>(SUBGHZSPI_BASE + 0x18));

extern peripheralRegister RCC_APB3ENR;
extern peripheralRegister RCC_CSR;
extern peripheralRegister PWR_SR2;

// Notes on the SX126x to STM32WLE interface
// The SX126x RESET is connected to RCC_CSR:15 (RFRST) for driving it, and RCC_CSR:14 (RFRSTF) for reading it
// The SX126x BUSY is connected to PWR_SR2:1 (RFBUSY)
// The SX126x SPI NSS
// * the functionality is enabled by PWR_CR1:3 (SUBGHZSPINSSSEL)
// * the NSS is driven by PWR_SUBGHZSPICR:14 (NSS)

void sx126x::handleEvents() {
}

void sx126x::setPacketType(packetType thePacketType) {
    constexpr uint8_t nmbrExtraBytes{1};                                                        //
    uint8_t parametersIn[nmbrExtraBytes], dataOut[nmbrExtraBytes];                              //
    parametersIn[0] = static_cast<uint8_t>(thePacketType);                                      //
    executeCommand(sx126xCommand::setPacketType, parametersIn, dataOut, nmbrExtraBytes);        //
}

void sx126x::waitOnBusy() {
    uint32_t waitCounter = maxWaitCounter;
    while (waitCounter > 0) {
        if (PWR_SR2 & RFBUSYS) {
            return;
        }
        waitCounter--;
    }
    // TODO : log Busy timeout error
    return;
}

bool sx126x::isStandby() {
    theState = getStateFromHW();
    return ((theState == sx126xState::standbyRc) || (theState == sx126xState::standbyXosc));
}

void sx126x::executeCommand(sx126xCommand opcode, uint8_t *parametersIn, uint8_t *dataOut, uint8_t nmbrExtraBytes, bool waitForBusy = false) {
    LL_PWR_SelectSUBGHZSPI_NSS();                      // drive NSS low = begin of SPI transaction
    SPI_Transmit(static_cast<uint8_t>(opcode));        // send command byte

    for (uint16_t extraBytesIndex = 0U; extraBytesIndex < nmbrExtraBytes; extraBytesIndex++) {
        SPI_Transmit(parametersIn[extraBytesIndex]);
        SPI_Reveive(dataOut[extraBytesIndex]);
    }

    LL_PWR_UnselectSUBGHZSPI_NSS();        // drive NSS high = end of SPI transaction

    if (waitForBusy) {
        (void)SUBGHZ_WaitOnBusy(hsubghz);
    }
}

sx126xState sx126x::getStateFromHW() {
    constexpr uint8_t nmbrExtraBytes{1};
    uint8_t parametersIn[nmbrExtraBytes], dataOut[nmbrExtraBytes];
    executeCommand(sx126xCommand::getStatus, parametersIn, dataOut, nmbrExtraBytes);

    switch (dataOut[1]) {
        default:
            return sx126xState::boot;
    }
}

sx126xError sx126x::getLastError() {
    sx126xError result = lastError;
    lastError          = sx126xError::none;
    return result;
}

bool sx126x::isBusy() {
    // PWR_SR2 register, bit 1
}

void sx126x::setRegulatorMode() {
    // Check if in Standby Mode - you cannot change the regulator in other state
    // Also check OverCurrentProtection - default at 60 mA
    // EU 868 band also limits power to 16 dBm !! so maybe the DCDC is not needed in this case
    // Datasheet pg 76 : default the LDO is selected. This works but is not the most power efficient solution -> using the DCDC can be postponed for a next phase where we optimize power

    constexpr uint8_t nmbrExtraBytes{1};                                                  //
    uint8_t parametersIn[nmbrExtraBytes], dataOut[nmbrExtraBytes];                        //
    parametersIn[0] = 0x00;                                                               // LDO-only mode
    executeCommand(sx126xCommand::setRegulatorMode, parametersIn, nmbrExtraBytes);        //
}

void sx126x::reset(bool waitForNoBusy = false) {
    constexpr uint32_t RFRST{15};
    constexpr uint32_t RFRSTF{14};
    constexpr uint32_t RFBUSYS{1};

    RCC_CSR.setBit(RFRSTF);          // activate reset
                                     // How long should we wait ? 1 ms
    RCC_CSR.clearBit(RFRSTF);        // deactivate reset line

    if (waitForNoBusy) {        //      optionally, wait until the SX126x is no longer busy
        while (PWR_SR2.readBit(RFBUSYS)) {
        }
    }
}

void sx126x::initialize() {
    // 1. Clocks
    // The SPI between STM32WLE and SX126x runs on the PCLK3 clock, divided by 2.
    // The SX126x-SPI interface has a maximum of 16 MhZ
    // As we are running the whole STM32WLE on 16 MHz (or lower), we can send the SYSCLK straight to the PCLK3 which means this SPI will run on 8 MHz
    RCC_APB3ENR.setBit(0);
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
    constexpr uint32_t MSTR{2};
    SPI_SX126X_CR1.setBit(MSTR);        // All other bits are 0 and this is what we need
    /*----------------------- SPI CR2 Configuration ----------------------------*
     *            Data Size: 8bits                                              *
     *              TI Mode: Disable                                            *
     *            NSS Pulse: Disable                                            *
     *    Rx FIFO Threshold: 8bits                                              *
     *--------------------------------------------------------------------------*/
    constexpr uint32_t FRXTH{12};
    SPI_SX126X_CR2.setBit(FRXTH);
    // SPI_SX126X_CR2.writeBits(0xF << 8, 0x7 << 8); // 8 bits data size is already the default

    constexpr uint32_t SPE{6};
    SPI_SX126X_CR1.setBit(SPE); /* Enable SUBGHZSPI Peripheral */

    // 3. Configure the SX126x
    setRegulatorMode();
    setPowerAmplifierConfig(uint8_t paDutyCycle, uint8_t hpMax, uint8_t deviceSel, uint8_t paLut);
    setPacketType(packetType::lora);

    // uint8_t modReg;
    //     modReg= SUBGRF_ReadRegister(SUBGHZ_SMPSC2R);
    //     modReg&= (~SMPS_DRV_MASK);
    //     SUBGRF_WriteRegister(SUBGHZ_SMPSC2R, modReg | level);

    CalibrateImage ? ? if (freq > 900000000) {
        calFreq[0] = 0xE1;
        calFreq[1] = 0xE9;
    }
    else if (freq > 850000000) {
        calFreq[0] = 0xD7;
        calFreq[1] = 0xDB;
    }
    else if (freq > 770000000) {
        calFreq[0] = 0xC1;
        calFreq[1] = 0xC5;
    }
    else if (freq > 460000000) {
        calFreq[0] = 0x75;
        calFreq[1] = 0x81;
    }
    else if (freq > 425000000) {
        calFreq[0] = 0x6B;
        calFreq[1] = 0x6F;
    }
    SUBGRF_WriteCommand(RADIO_CALIBRATEIMAGE, calFreq, 2);
}

void sx126x::goSleep() {
    sx126xState currentState = getStateFromHW();
    if (isStandby()) {
        constexpr uint8_t nmbrExtraBytes{1};
        uint8_t parametersIn[nmbrExtraBytes], dataOut[nmbrExtraBytes];
        executeCommand(sx126xCommand::setSleep, parametersIn, dataOut, nmbrExtraBytes);
    } else {
        lastError = sx126xError::goSleepWhenNotInStandby;
    }

    // TODO : change the state

    /* switch the antenna OFF by SW */
    // RBI_ConfigRFSwitch(RBI_SWITCH_OFF);

    // Radio_SMPS_Set(SMPS_DRIVE_SETTING_DEFAULT);

    // uint8_t value = ( ( ( uint8_t )sleepConfig.Fields.WarmStart << 2 ) |
    //                   ( ( uint8_t )sleepConfig.Fields.Reset << 1 ) |
    //                   ( ( uint8_t )sleepConfig.Fields.WakeUpRTC ) );
    // SUBGRF_WriteCommand( RADIO_SET_SLEEP, &value, 1 );
    // OperatingMode = MODE_SLEEP;
}

void sx126x::goStandby(sx126xStandbyMode theStandbyMode = sx126xStandbyMode::rc) {
    // normally the SX126x goes to standby automatically after transmission / receive completed, or timed out

    constexpr uint8_t nmbrExtraBytes{1};
    uint8_t parametersIn[nmbrExtraBytes], dataOut[nmbrExtraBytes];
    parametersIn[0] = static_cast<uint8_t>(theStandbyMode);
    executeCommand(sx126xCommand::setStandby, parametersIn, dataOut, nmbrExtraBytes);
}

void sx126x::setRfFrequency(uint32_t frequencyInHz) {
    uint64_t tmpFrequencyRegisterValue = ((static_cast<uint64_t>(frequencyInHz) << 25) / static_cast<uint64_t>(crystalFrequency));        // 64 bit math needed to avoid overflow, result is only 32 bits wide
    uint32_t frequencyRegisterValue    = (static_cast<uint32_t>(tmpFrequencyRegisterValue && 0x00000000FFFFFFFF));                        // take only lower 32-bits, upper bits should be zero

    constexpr uint8_t nmbrExtraBytes{4};
    uint8_t parametersIn[nmbrExtraBytes]{0}, dataOut[nmbrExtraBytes];
    parametersIn[0] = static_cast<uint8_t>((frequencyRegisterValue >> 24) & 0xFF);        //
    parametersIn[1] = static_cast<uint8_t>((frequencyRegisterValue >> 16) & 0xFF);        //
    parametersIn[2] = static_cast<uint8_t>((frequencyRegisterValue >> 8) & 0xFF);         //
    parametersIn[3] = static_cast<uint8_t>((frequencyRegisterValue)&0xFF);                //

    executeCommand(sx126xCommand::setRfFRequency, parametersIn, dataOut, nmbrExtraBytes);
}

void sx126x::setTxParameters(int8_t transmitPowerdBm, rampTime theRamptime) {        // caution - signed int8, negative dBm values are in two's complement
    constexpr uint8_t nmbrExtraBytes{2};
    uint8_t parametersIn[nmbrExtraBytes]{0}, dataOut[nmbrExtraBytes];

    if (transmitPowerdBm < 0) {
        parametersIn[0] = 255 - static_cast<uint8_t>(-(transmitPowerdBm + 1));        // magic to turn negative signed value into a raw byte in two's complement
    } else {
        parametersIn[0] = static_cast<uint8_t>(transmitPowerdBm);        // if value is not negative, casting it is safe
    }
    parametersIn[1] = theRamptime;        // rampTime 200 uS - no info why this value, but this was in the demo application from ST / Semtech
                                          // the remaining 4 bytes are empty 0x00 for LoRa
    executeCommand(sx126xCommand::setTXPARAMS, parametersIn, dataOut, nmbrExtraBytes);
}

void sx126x::setModulationParameters(spreadingFactor theSpreadingFactor, bandwidth theBandwidth, codingRate theCodingRate) {
    constexpr uint8_t nmbrExtraBytes{8};
    uint8_t parametersIn[nmbrExtraBytes]{0}, dataOut[nmbrExtraBytes];
    parametersIn[0] = static_cast<uint8_t>(theSpreadingFactor);        //
    parametersIn[1] = static_cast<uint8_t>(theBandwidth);              //
    parametersIn[2] = static_cast<uint8_t>(theCodingRate);             //
    parametersIn[3] = 0x01;                                            // TODO ?? LowDatarateOptimize // why would you NOT want to do this ??
                                                                       // the remaining 4 bytes are empty 0x00 for LoRa
    executeCommand(sx126xCommand::setPacketParameters, parametersIn, dataOut, nmbrExtraBytes);
}

void sx126x::setPacketParameters(uint8_t payloadLength) {
    constexpr uint8_t nmbrExtraBytes{9};
    uint8_t parametersIn[nmbrExtraBytes]{0}, dataOut[nmbrExtraBytes];
    parametersIn[0] = 0x00;                 // MSB for PreambleLength
    parametersIn[1] = 0x08;                 // LSB for PreambleLength LoRaWAN is fixed to 8 symbols
    parametersIn[2] = 0x00;                 // Variable length packet (explicit header)
    parametersIn[3] = payloadLength;        // Payload Length
    parametersIn[4] = 0x01;                 // CRC On ??
    parametersIn[5] = 0x00;                 // Standard IQ : for uplink. Downlink requires inverted IQ
                                            // the remaining 3 bytes are empty 0x00 for LoRa
    executeCommand(sx126xCommand::setPacketParameters, parametersIn, dataOut, nmbrExtraBytes);
}

void sx126x::setPowerAmplifierConfig(uint8_t paDutyCycle, uint8_t hpMax, uint8_t deviceSel, uint8_t paLut) {
    // Guessing from the STM32WLE datasheet, there is an SX1262 inside, ie. the high power version, with up to 22 dBm transmit power.
    // However, in Europe, ETSI limits the power to 16 dBm anyway, so it seems like useing the extra power of the SX1262 vs the SX1261 is not allowed...

    constexpr uint8_t nmbrExtraBytes{4};
    uint8_t parametersIn[nmbrExtraBytes], dataOut[nmbrExtraBytes];
    parametersIn[0] = 0x06;        // paDutyCycle - +15 dBm
    parametersIn[1] = 0x00;        // hpMax - no effect in case of SX1261 mode, only for SX1262
    parametersIn[2] = 0x01;        // 0x01 = SX1261, 0x00 = SX1262 mode
    parametersIn[3] = 0x01;        // reserved and always 0x01

    executeCommand(sx126xCommand::setPowerAmplifierConfig, parametersIn, dataOut, nmbrExtraBytes);
}

// void sx126x::writeBuffer(uint8_t *data, uint8_t dataLength, uint8_t startAddressOffset = 0) {
//  Problem : we need to insert the offset byte into the stream of data... maybe we need to make a local copy, but this buffer can be 256 bytes long..
//  executeCommand(sx126xCommand::writeBuffer, parametersIn, dataOut, nmbrExtraBytes);
//}

void sx126x::writeBuffer(byteBuffer rawMessage) {
    // copy the raw LoRa message into the transmitBuffer
}

// uint32_t sx126x::readBuffer(uint8_t *data, uint8_t dataLength, uint8_t startAddressOffset = 0) {
//  Problem : we need to insert the offset byte into the stream of data... maybe we need to make a local copy, but this buffer can be 256 bytes long..
//  executeCommand(sx126xCommand::writeBuffer, parametersIn, dataOut, nmbrExtraBytes);
//}

void sx126x::readBuffer(byteBuffer rawMessage) {
    // read the raw LoRa message which has been received
}
