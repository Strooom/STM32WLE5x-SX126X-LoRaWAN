#include "cli.h"

void cli::handleRxEvent() {

// using STM-HAL, check for data in the UART RX buffer
// if there is data, append it to the receive buffer
// if the latest character received is a carriage return, process the command

    // if (rxBuffer.hasData()) {
    // append to commandBuffer
    // if (commandBuffer[0] == 0x0D) {
    // process command
}