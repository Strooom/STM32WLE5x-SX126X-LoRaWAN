// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class sx126xCommand : uint8_t {
    // Commands accessing the radio registers and FIFO - SX1261/2 Datasheet Rev 2.1 - Table 11-2
    writeRegister        = 0x0D,
    readRegister         = 0x1D,
    setBufferBaseAddress = 0x8F,
    writeBuffer          = 0x0E,
    readBuffer           = 0x1E,

    // Commands selecting the operating mode of the radio - SX1261/2 Datasheet Rev 2.1 - Table 11-1
    setSleep              = 0x84,
    setStandby            = 0x80,
    setFs                 = 0xC1,
    setTx                 = 0x83,
    setRx                 = 0x82,
    stopTimerOnPreamble   = 0x9F,
    setRxdutycycle        = 0x94,
    setCad                = 0xC5,
    setTxContinuousWave   = 0xD1,
    setTxInfinitePreamble = 0xD2,
    setRegulatorMode      = 0x96,
    calibrate             = 0x89,
    calibrateImage        = 0x98,
    setPaConfig           = 0x95,
    setRxTxFallbackMode   = 0x93,

    // Commands Controlling the RF and Packets Settings - SX1261/2 Datasheet Rev 2.1 - Table 11-4
    setRfFRequency        = 0x86,
    setPacketType         = 0x8A,
    getPacketType         = 0x11,
    setTxParams           = 0x8E,
    setModulationParams   = 0x8B,
    setPacketParameters   = 0x8C,
    setCadParams          = 0x88,
    setLoRaSymbNumTimeout = 0xA0,

    // Commands controlling the radio IRQ and Digital IO (DIO) - SX1261/2 Datasheet Rev 2.1 - Table 11-3
    setDioIrqParams       = 0x08,
    getIrqStatus          = 0x12,
    clearIrqStatus        = 0x02,
    setDIO2AsRfSwitchCtrl = 0x9D,
    setDIO3AsTcxoCtrl     = 0x97,

    // Commands Returning the Radio Status - Errors - Statistics SX1261/2 Datasheet Rev 2.1 - Table 11-5
    getStatus         = 0x0C,
    getRssiInst       = 0x15,
    getRxBufferStatus = 0x13,
    getPacketStatus   = 0x14,
    getDeviceErrors   = 0x17,
    clearDeviceErrors = 0x07,
    getStats          = 0x10,
    resetStats        = 0x00,

};

const char* toString(sx126xCommand command);