# Welcome to LoRa-E5-SX126X

This is a driver for the SX126X LoRa radio inside the (Seeed / ST) LoRa-E5 module.

The LoRa-E5 module comes with a factory-installed firmware which allows to control it from an external MCU over a UART as an AT modem.
However, the LoRa-E5 also contains an STM32 MCU. Running your application on the STM32 achieves:
* lower power : no extra external MCU to be powered
* smaller footprint
* lower cost

Disadvantage is a higher complexity, as the AT modem application hides the LoRa and LoRaWAN complexity for you. The purpose of this library, however, is to limit this extra complexity so you can build a LoRaWAN end node directly on the embedded STM32

This is currently a work-in-progress:
* should have a working first release before 2023-03-31
* sw-framework : independent or to be decided (Arduino or STMCube)