# Welcome to STM32WLE5-SX126X-LoRa

This is a driver for the SX126X LoRa radio inside the STM32WLE5x.

I found the Semtech driver, as well as the ST-variant not really easy to use:
* not optimal as this software tries to be generic, but as a result does not benefit from the HW resources of the MCU, such as CRC calculation, etc.
* hard to read, as there are many preprocessor parts, making the code compile for different variants, eg single core, dual core..
* contains more code than needed because it also supports Lora class B and C

A much simpler driver is possible when only Lora class A, on a single core STM32WLE.

Finally the semtech code is C, whereas a basic C++ implementation is way more readible.


This is currently a work-in-progress:
* should have a working first release before 2023-03-31



# Things this library does NOT do
* (G)FSK modulation. This library only supports the LoRa modulation
* initially I only support LoraWAN Class A, so no support for Class B and C - but the framework is ready for it, and it could be added later when needed
* 


