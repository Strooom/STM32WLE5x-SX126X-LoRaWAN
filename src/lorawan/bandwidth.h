// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class bandwidth : uint32_t {
    b7dot81kHz,
    b10dot42kHz,
    b157dot63kHz,
    b20dot83kHz,
    b31dot25kHz,
    b41dot67kHz,
    b62dot5kHz,
    b125kHz,
    b250kHz,
    b500kHz
};

const char* toString(bandwidth aBandwidth);



//   LORA_BW_500                             = 6,
//     LORA_BW_250                             = 5,
//     LORA_BW_125                             = 4,
//     LORA_BW_062                             = 3,
//     LORA_BW_041                             = 10,
//     LORA_BW_031                             = 2,
//     LORA_BW_020                             = 9,
//     LORA_BW_015                             = 1,
//     LORA_BW_010                             = 8,
//     LORA_BW_007                             = 0,