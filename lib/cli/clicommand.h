// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class cliCommand : uint32_t {
    HLP = (0x48 << 16) + (0x4C << 8) + 0x50,        // show Help
    VER = (0x56 << 16) + (0x45 << 8) + 0x52,        // show Version
    HID = (0x48 << 16) + (0x49 << 8) + 0x44,        // show Hardeware IDs
    LAC = (0x4C << 16) + (0x41 << 8) + 0x43,        // List Available Channels
    BAT = (0x42 << 16) + (0x41 << 8) + 0x54,        // show Battery Status
    NET = (0x4E << 16) + (0x45 << 8) + 0x54,        // show Network Status
    MEM = (0x4D << 16) + (0x45 << 8) + 0x4D,        // show Memory Status
    RES = (0x52 << 16) + (0x45 << 8) + 0x53,        // restart software
    STP = (0x53 << 16) + (0x54 << 8) + 0x50,        // stop LoRa to prepare for firmware update
};