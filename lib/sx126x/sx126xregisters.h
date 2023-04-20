// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

// Note : I think most of these registers are of no use for the standard LoRa case...

enum class sx126xRegister : uint16_t {
    LoRaSyncWordMSB          = 0x740,
    LoRaSyncWordLSB          = 0x741,
    TxClampConfig            = 0x8D8,
    OCPConfiguration         = 0x8E7,
    XTAtrim                  = 0x911,
    DIO3outputVoltageControl = 0x920,
    smpsMaximumDrive         = 0x923,

    // unknown = 0x923
    // hoppingEnable = 0x0385,
    // packetLength  = 0x0386,
    // overcurrentProtection = 0x08E7,
    // footCapacitorConfigA  = 0x0911,
    // footCapacitorConfigB  = 0x0912,
    // REG_LR_CRCSEEDBASEADDR          = 0x06BC,
    // REG_LR_CRCPOLYBASEADDR          = 0x06BE,
    // REG_LR_WHITSEEDBASEADDR_MSB     = 0x06B8,
    // REG_LR_WHITSEEDBASEADDR_LSB     = 0x06B9,
    // REG_LR_PACKETPARAMS             = 0x0704,
    // REG_LR_PAYLOADLENGTH            = 0x0702,
    // REG_LR_SYNCH_TIMEOUT            = 0x0706,
    // REG_LR_SYNCWORDBASEADDRESS      = 0x06C0,
    // REG_LR_SYNCWORD                 = 0x0740,
    // LORA_MAC_PRIVATE_SYNCWORD       = 0x1424,
    // LORA_MAC_PUBLIC_SYNCWORD        = 0x3444,
    // RANDOM_NUMBER_GENERATORBASEADDR = 0x0819,
    // REG_ANA_LNA                     = 0x08E2,
    // REG_ANA_MIXER                   = 0x08E5,
    // REG_RX_GAIN                     = 0x08AC,
    // REG_BIT_SYNC                    = 0x06AC,
    // REG_XTA_TRIM                    = 0x0911,
    // REG_XTB_TRIM                    = 0x0912,
    // REG_OCP                         = 0x08E7,
    // REG_TX_CLAMP                    = 0x08D8,
};

const char* toString(sx126xRegister aRegister);