// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include "stm32wle_peripheral.h"

class stm32wleUart : public stm32wlePeripheral {
  public:
	void initialize();
	void prepareForSleep();
	void restoreAfterSleep();
    static constexpr uint32_t rxBufferLength = 64;          //
    static constexpr uint32_t txBufferLength = 1024;        //

  private:
    uint8_t rxBuffer[rxBufferLength]{};        // Receive buffer
    volatile uint32_t rxHead{0};               // Position of oldest byte received
    volatile uint32_t rxLevel{0};              // Number of bytes in the receive buffer

    uint8_t txBuffer[txBufferLength]{};        // Transmit buffer
    volatile uint32_t txHead{0};               // Position of next byte to be transmitted
    volatile uint32_t txLevel{0};              // Number of bytes in the transmit buffer

    //friend int main(void);             // TODO assign the ISR routine here, so it can call the txReady and rxReady functions
};
