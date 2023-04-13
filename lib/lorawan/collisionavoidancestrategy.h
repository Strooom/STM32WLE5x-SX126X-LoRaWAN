// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class collisionAvoidanceStrategy : uint8_t {
    none,        // when we want to transmit, we just do it immediately
    cad,         // when we want to transmit, we first do a channel activity detection, and if no activity, we transmit. If activity, we wait a random time and do channel activity detection again, until we can transmit.
    aloha        // when we want to transmit, we first wait a random time, and then transmit.
    };