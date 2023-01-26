# Welcome to STM32WLE-LoRa

This is a driver for the SX126X LoRa radio inside the STM32WLE.
I found the Semtech driver, as well as the ST-variant not really easy to use:
* not optimal as this software tries to be generic, but as a result does not use the HW resources of the MCU, such as CRC calculation, etc.
* hard to read, as there are many preprocessor parts, making the code compile for different variants, eg single core, dual core..
* contains more code than needed because it also supports Lora class B and C

A much simpler driver is possible when only Lora class A, on a single core STM32WLE.

Finally the semtech code is C, whereas a basic C++ implementation is way more readible.
