# Welcome to STM32WLE5-SX126X-LoRa

This is a driver for the **SX126X LoRa radio** inside the **STM32WLE5x**.

As part of the development for this Lora End-Node project (https://github.com/Strooom/MuMo-V2-Node-PCB (HW) https://github.com/Strooom/MuMo-v2-Node-SW (SW)) I was looking for a software library for the SX126x and LoRaWAN. Unfortunately, I could not find anything that met my needs.
* Semtech's driver is not easy to reuse, mainly because it tries to implement all their radio chips, all LoraWAN versions and all 3 classes (A,B & C) in a single software repository.
* Looking into the code, I found that ST had added several workarounds... Why are they needed ? How do they work ?
* Inside the code I found numerous code smells, eg. negative values being assigned into unsigned int, etc..
* Initially their code had FUOTA examples, but they were revoked, with the message that they didn't work..
* In demo's from their code I saw people disable ADR, because it didn't work..
* Everything is written in C io. C++

Investigating the existing code, I realised that writing my own driver would take a few months. But trying to reuse the existing code would take much more effort in the long term, and probably never reach the same quality, readability & maintainability.

# Requirements - Scope

In order to limit the amount of work, and get something working soon, I decided to limit the scope and postpone some non-essential things for later:
* only do LoRa - no (G)FSK, long packets, etc
* only do LoRaWAN Class A
* only do LoRaWAN V1.0.4
* only Activation By Personalization - ABP : DevEUI is taken from the UID64 inside the STM32WLE. DeviceAddress (DevAddr) AppSKey and NwkSKey are generated on TTN 'register end device' console and then programmed via the Node's CLI into the device.
* only support region EU-868
* LoRa sync word is fixed to 0x34 and preamble length is fixed to 8 symbols
* Support only the minimum set of DR0 - DR5, as a consequence, the bandwidth is always 125 kHz
* in the uplink, we don't combine MAC messages with application payload.
    - MAC messages are sent in the payload on framePort 0
    - Application messages are sent on another port
* restrict to settings needed by The Things Network, wherever applicable, as this will be the LoRaWAN network provider we will be using.

# Things for later
* Adaptive Data Rate - ADR : as our gateways are really close (50m), I can probably live with just using SF9
* Over-The-Air Activation - OTAA
* Class C when having external power
* firmware Update Over The Air - FUOTA : our devices are recharged via a USB-C port. During recharging, we could upgrade the firmware over the USB port
* other regions ?? Currently we are field trialling in Belgium, Europe. However one day we may want this sensor-node to travel with the art collection item, and then we must look into supporting other regions.

# Application Functionality
Some things are needed by the application anyway, so we don't need to repeat them in the SX126 / LoRaWAN driver:
* handling of non-volatile storage : there is 128Kbytes of I2C EEPROM on our board. We use it for storing all context that needs to persist beyond a reset and/or battery removal. This storage is also used to store measurements data.
* logging / tracing : during development, logging goes to SWD/TRACE port. Afterwards it can be redirected to UART over USB
