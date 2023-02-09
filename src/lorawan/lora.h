// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

// All kinds of definitions for LoRa (not LoRaWAN) terminology

typedef enum {
    LORA_PACKET_VARIABLE_LENGTH = 0x00,        //!< The packet is on variable size, header included
    LORA_PACKET_FIXED_LENGTH    = 0x01,        //!< The packet is known on both sides, no header included in the packet
    LORA_PACKET_EXPLICIT        = LORA_PACKET_VARIABLE_LENGTH,
    LORA_PACKET_IMPLICIT        = LORA_PACKET_FIXED_LENGTH,
} RadioLoRaPacketLengthsMode_t;

typedef enum {
    LORA_IQ_NORMAL   = 0x00,
    LORA_IQ_INVERTED = 0x01,

} RadioLoRaIQModes_t;

typedef struct
{
    RadioPacketTypes_t PacketType;        //!< Packet to which the modulation parameters are referring to.
    struct
    {
        RadioLoRaSpreadingFactors_t SpreadingFactor;        //!< Spreading Factor for the LoRa modulation
        RadioLoRaBandwidths_t Bandwidth;                    //!< Bandwidth for the LoRa modulation
        RadioLoRaCodingRates_t CodingRate;                  //!< Coding rate for the LoRa modulation
        uint8_t LowDatarateOptimize;                        //!< Indicates if the modem uses the low datarate optimization
    } LoRa;

} ModulationParams_t;

typedef struct
{
    RadioPacketTypes_t PacketType;        //!< Packet to which the packet parameters are referring to.
    struct
    {
        uint16_t PreambleLength;                        //!< The preamble length is the number of LoRa symbols in the preamble
        RadioLoRaPacketLengthsMode_t HeaderType;        //!< If the header is explicit, it will be transmitted in the LoRa packet. If the header is implicit, it will not be transmitted
        uint8_t PayloadLength;                          //!< Size of the payload in the LoRa packet
        RadioLoRaCrcModes_t CrcMode;                    //!< Size of CRC block in LoRa packet
        RadioLoRaIQModes_t InvertIQ;                    //!< Allows to swap IQ for LoRa packet
    } LoRa;
} PacketParams_t;

typedef struct
{
    RadioPacketTypes_t packetType;        //!< Packet to which the packet status are referring to.
    struct
    {
        int8_t RssiPkt;        //!< The RSSI of the last packet
        int8_t SnrPkt;         //!< The SNR of the last packet
        int8_t SignalRssiPkt;
        uint32_t FreqError;
    } LoRa;
} PacketStatus_t;