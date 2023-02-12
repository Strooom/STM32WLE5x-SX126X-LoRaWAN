# Welcome to STM32WLE5-SX126X-LoRa

This is a driver for the **SX126X LoRa radio** inside the **STM32WLE5x**.

As part of the development for this Lora End-Node project (https://github.com/Strooom/MuMo-V2-Node-PCB (HW) https://github.com/Strooom/MuMo-v2-Node-SW (SW)) I was looking for a software library for the SX126x and LoRaWAN. Unfortunately, I could not find anything that met my needs.
* Semtech's driver is not easy to reuse, mainly because it tries to implement all their radio chips, all LoraWAN versions and all 3 classes (A,B & C) in a single software repository.
* Looking into the code, I found that ST had added several workarounds... Why are they needed ? How do they work ?
* Inside the code I found numerous code smells, eg. negative values being assigned into unsigned int, etc..
* Initially their code had FUOTA examples, but they were revoked, with the message that they didn't work..
* In demo's from their code I saw people disable ADR, because it didn't work..
* Everything is written in C io. C++

Investigating the existing code, I realised that writing my own driver would take a few months. But trying te reuse the existing code would take much more effort in the long term, en probably never reach the same quality.

# Requirements - Scope

In order to limit the amount of work, and get something working soon, I decided to limit the scope and postpone some non-essential things for later:
* only do LoRa - no (G)FSK, long packets, etc
* only do LoRaWAN Class A
* only do LoRaWAN V1.0.4
* only Activation By Personalization - ABP
* only support region EU-868

# Things for later
* Over-The-Air Activation - OTAA
* Class C when having external power
* firmware Update Over The Air - FUOTA
* other regions ??

# Application Functionality
Some things are needed by the application anyway, so we don't need to repeat them in the SX126 / LoRaWAN driver:
* handling of non-volatile storage
* logging / tracing
