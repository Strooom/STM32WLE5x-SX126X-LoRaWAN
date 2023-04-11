// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class cliCommand : uint32_t {
    help = (0x68 << 24) + (0x65 << 16) + (0x6C << 8) + 0x70,        // show Help
    stop = (0x73 << 24) + (0x74 << 16) + (0x6F << 8) + 0x70,        // stop LoRa to prepare for firmware update

    ver = (0x76 << 24) + (0x65 << 16) + (0x72 << 8) + 0x73,        // show Version
    hid = (0x68 << 24) + (0x69 << 16) + (0x64 << 8) + 0x73,        // show Hardeware IDs
    lac = (0x6C << 24) + (0x61 << 16) + (0x63 << 8) + 0x73,        // List Available Channels
    bat = (0x62 << 24) + (0x61 << 16) + (0x74 << 8) + 0x73,        // show Battery Status
    net = (0x6E << 24) + (0x65 << 16) + (0x74 << 8) + 0x73,        // show Network Status
    mem = (0x6D << 24) + (0x65 << 16) + (0x6D << 8) + 0x73,        // show Memory Status
    res = (0x72 << 24) + (0x65 << 16) + (0x73 << 8) + 0x73,        // restart software

    snk = (0x73 << 24) + (0x6E << 16) + (0x6B << 8) + 0x73,        // set Network Key
    sak = (0x73 << 24) + (0x61 << 16) + (0x6B << 8) + 0x73,        // set Application Key
    sda = (0x73 << 24) + (0x64 << 16) + (0x61 << 8) + 0x73,        // set Device Address
    sam = (0x73 << 24) + (0x61 << 16) + (0x6D << 8) + 0x73,        // set Administator Mode
    cam = (0x63 << 24) + (0x61 << 16) + (0x6D << 8) + 0x73,        // clear Administator Mode

};
