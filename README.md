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
* Implicit Header Mode - only Explicit Header Mode is supported
* firmware Update Over The Air - FUOTA - Although I think this is a very powerfull feature, and mandatory to have when deploying a large number of nodes, I have the impression that it is not mature and not really working well. Demo applications from the suppliers were revoked because they were not working well. So I'll postpone FUOTA for a next version
* As a result we do not really need multicast, clock-sync etc.
* things I don't think this SW should do : 
 - measure battery level, measure other sensors (all this belongs in the application above), 
 - handle the non-volatile storage of data (this is needed in the application above anyway, so let them do this)
 - logging, tracing : you need this at application level anyway, don't do anything yourself in the LoraWAN layer
* 
