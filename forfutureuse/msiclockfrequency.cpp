// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include "msiclockfrequency.h"

const char* toString(msiClockFrequency aFrequency) {
    switch (aFrequency) {
        case msiClockFrequency::f100kHz:
            return "100 kHz";
            break;

        case msiClockFrequency::f200kHz:
            return "200 kHz";
            break;

        case msiClockFrequency::f400kHz:
            return "400 kHz";
            break;

        case msiClockFrequency::f800kHz:
            return "800 kHz";
            break;

        case msiClockFrequency::f1MHz:
            return "1 MHz";
            break;

        case msiClockFrequency::f2MHz:
            return "2 MHz";
            break;

        case msiClockFrequency::f4MHz:
            return "4 MHz";
            break;

        case msiClockFrequency::f8MHz:
            return "8 MHz";
            break;

        case msiClockFrequency::f16MHz:
            return "16 MHz";
            break;

        case msiClockFrequency::f24MHz:
            return "24 MHz";
            break;

        case msiClockFrequency::f32MHz:
            return "32 MHz";
            break;

        case msiClockFrequency::f48MHz:
            return "48 MHz";
            break;

         default:
            return "unknown/invalid frequency";
            break;
    }
}
