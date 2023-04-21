// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "nvs.h"
#include "nvs2.h"
#include <stdio.h>
#include <string.h>

some syntax error

extern uint8_t memory[60 * 1024];

bool nonVolatileStorage::isReady() {
    return true;
}

void nonVolatileStorage::read(uint32_t startAddress, uint8_t *data, uint32_t dataLength) {
    memcpy(data, memory + startAddress, dataLength);
}
void nonVolatileStorage::write(uint32_t startAddress, uint8_t *data, uint32_t dataLength) {
    memcpy(memory + startAddress, data, dataLength);
}
