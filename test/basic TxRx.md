# Circuit Configuration for Basic Tx Operation

After power up (battery insertion or hard reset) the chip runs automatically a calibration procedure and goes to STDBY_RC
mode. 
This is indicated by a low state on BUSY pin. 

From this state the steps are:
1. If not in STDBY_RC mode, then go to this mode with the command SetStandby(...)
2. Define the protocol (LoRa® or FSK) with the command SetPacketType(...)
3. Define the RF frequency with the command SetRfFrequency(...)
4. Define the Power Amplifier configuration with the command SetPaConfig(...)
5. Define output power and ramping time with the command SetTxParams(...)
6. Define where the data payload will be stored with the command SetBufferBaseAddress(...)
7. Send the payload to the data buffer with the command WriteBuffer(...)
8. Define the modulation parameter according to the chosen protocol with the command SetModulationParams(...)1
9. Define the frame format to be used with the command SetPacketParams(...)2
10. Configure DIO and IRQ: use the command SetDioIrqParams(...) to select TxDone IRQ and map this IRQ to a DIO (DIO1, DIO2 or DIO3)
11. Define Sync Word value: use the command WriteReg(...) to write the value of the register via direct register access
12. Set the circuit in transmitter mode to start transmission with the command SetTx(). Use the parameter to enable Timeout
13. Wait for the IRQ TxDone or Timeout: once the packet has been sent the chip goes automatically to STDBY_RC mode
14. Clear the IRQ TxDone flag


# Circuit Configuration for Basic Rx Operation
This chapter describes the sequence of operations needed to receive a frame starting from a power up. This sequence is
valid for all protocols.
After power up (battery insertion or hard reset) the chip run automatically a calibration procedure and goes to STDBY_RC
mode. 
This is indicated by a low state on BUSY pin. 
From this state the steps are:
1. If not in STDBY_RC mode, then set the circuit in this mode with the command SetStandby()
2. Define the protocol (LoRa® or FSK) with the command SetPacketType(...)
3. Define the RF frequency with the command SetRfFrequency(...)
4. Define where the data will be stored inside the data buffer in Rx with the command SetBufferBaseAddress(...)
5. Define the modulation parameter according to the chosen protocol with the command SetModulationParams(...)1
6. Define the frame format to be used with the command SetPacketParams(...)
7. Configure DIO and irq: use the command SetDioIrqParams(...) to select the IRQ RxDone and map this IRQ to a DIO (DIO1 or DIO2 or DIO3), set IRQ Timeout as well.
8. Define Sync Word value: use the command WriteReg(...) to write the value of the register via direct register access.
9. Set the circuit in reception mode: use the command SetRx(). Set the parameter to enable timeout or continuous mode
10. Wait for IRQ RxDone2 or Timeout: the chip will stay in Rx and look for a new packet if the continuous mode is selected otherwise it will goes to STDBY_RC mode.
11. In case of the IRQ RxDone, check the status to ensure CRC is correct: use the command GetIrqStatus()
Note: 
The IRQ RxDone means that a packet has been received but the CRC could be wrong: the user must check the CRC before validating the packet.
12. Clear IRQ flag RxDone or Timeout: use the command ClearIrqStatus(). In case of a valid packet (CRC OK), get the packet length and address of the first byte of the eceived payload by using the command GetRxBufferStatus(...)
13. In case of a valid packet (CRC OK), start reading the packet