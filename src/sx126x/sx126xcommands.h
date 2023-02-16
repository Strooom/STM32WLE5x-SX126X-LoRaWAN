// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class sx126xCommand : uint32_t {
    // Commands selecting the operating mode of the radio - SX1261/2 Datasheet Rev 2.1 - Table 11-1
    setSleep              = 0x84,
    setStandby            = 0x80,
    setFrequencySynthesis = 0xC1,
    setTx                 = 0x83,
    setRx                 = 0x82,

    setSTOPRXTIMERONPREAMBLE = 0x9FU,
    setRXDUTYCYCLE           = 0x94U,
    setCAD                   = 0xC5U,
    setTXCONTINUOUSWAVE      = 0xD1U,
    setTXCONTINUOUSPREAMBLE  = 0xD2U,
    setRegulatorMode         = 0x96U,
    RADIO_CALIBRATE          = 0x89U,
    RADIO_CALIBRATEIMAGE     = 0x98U,
    setPowerAmplifierConfig  = 0x95U,
    setRxTXFALLBACKMODE      = 0x93U,

    // Commands accessing the radio registers and FIFO - SX1261/2 Datasheet Rev 2.1 - Table 11-2

    writeRegister = 0x0D,
    readRegister  = 0x1D,
    writeBuffer   = 0x0E,
    readBuffer    = 0x1E,

    // Commands controlling the radio IRQ and Digital IO (DIO) - SX1261/2 Datasheet Rev 2.1 - Table 11-3

    RADIO_CFG_DIOIRQ      = 0x08U,
    getIRQSTATUS          = 0x12U,
    RADIO_CLR_IRQSTATUS   = 0x02U,
    SetDIO2AsRfSwitchCtrl = 0x9D,
    SetDIO3AsTcxoCtrl     = 0x97,

    // Commands Controlling the RF and Packets Settings - SX1261/2 Datasheet Rev 2.1 - Table 11-4

    setRfFRequency       = 0x86,
    setPacketType        = 0x8AU,
    getPACKETTYPE        = 0x11U,
    setTXPARAMS          = 0x8EU,
    setMODULATIONPARAMS  = 0x8BU,
    setPacketParameters  = 0x8C,
    setCADPARAMS         = 0x88U,
    setBUFFERBASEADDRESS = 0x8FU,
    setLORASYMBTIMEOUT   = 0xA0U,

    // Commands Returning the Radio Status -  - SX1261/2 Datasheet Rev 2.1 - Table 11-5

    getStatus         = 0x0C,
    getRSSIINST       = 0x15U,
    getRXBUFFERSTATUS = 0x13U,
    getPACKETSTATUS   = 0x14U,
    getERROR          = 0x17U,
    RADIO_CLR_ERROR   = 0x07U,
    getSTATS          = 0x10U,
    RADIO_RESET_STATS = 0x00U

    // setTCXOMODE          = 0x97U,
    // setRFSWITCHMODE      = 0x9DU,

};
