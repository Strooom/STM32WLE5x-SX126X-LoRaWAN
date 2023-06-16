# Goals
1. Faster / More efficient Development
2. More robust implementation
3. Lower Power : runs longer on a battery charge
4. More functionality -> extra features

# TODO's 
* why are all the std:: stuff linked in... (robust)
* make a github action build for unit testing (robust)
* check other classes if they could be implemented as static (robust)
* Let the application piggyback a linkStatusRequest on a certain interval, eg. once per day = once every 2880 rtcTicks (features)
* after booting, add an event into the measurements, with immediate priority so we send something immediately and we can monitor that at the livedata from the gateway
* reduce power consumption : put MCU into sleep as much as possible
* For all channels, I could precalculate the frequency register value and store it in the channel object. (power)
* AES implementation in HW (power)
* implement clockSync application package (feature)
* test EPOCH stuff https://www.epochconverter.com/programming/c
* firmware update over USB
* CLI to provision a device
* show status and measurements on the ePaper display
* report batteryPower to LoRaWAN : [0..254]
* report network strength to LoRaWAN
* after having the clock synced, use it to store real timestamps with measurements
* CLI


# DONOT's
* FUOTA : complex implementation, not only on the end node but also on the network server. We can do firmware updates via USB as an alternative


# TODO's C++ / CodingStyle
* rename eventBuffer template class to circularBuffer : it can be used for any type of content, not only events.. (I intend to use if for UART buffer as well)
* rename byteBuffer2 template class to linearBuffer
* maybe get rid of byteBuffer1

# TODO's LowPower

* do we need __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI); ??
* check SystemClock_Config : compare with STs demo app

Notes from Experiments :
MCU after reset (4 MHz) ~ 1.5 mA
MCU after clockConfig (16 MHz) ~ 2.2 mA
ADC Initialized vs DeInitialized : 
    * 25 uA when CPU is running
    * no effect when in stop mode
RNG Initialized vs DeInitialized : 
    * 120 uA when CPU is running
    * no effect when in stop mode
I2C Initialized vs DeInitialized : 
    * 60 uA when CPU is running
    * no effect when in stop mode
SC126x Sleep vs Standby : (DebugStopModeEnabled)
    * standby 2.277 mA
    * sleep 1.914 mA
    * delta = 0.363 mA
SC126x Sleep vs Standby : (DebugStopModeDisabled)
    * standby 1.13 mA
    * sleep 0.383 mA
    * delta = 0.747 mA

* investigate how to put the STM32/ SX126 in sleep during the waitForRxStart and waitForRx2Start


# TODO's LoRaWAN
* solution for when a device boots with no settings in EEPROM : when reading settings from NVS, they should all be checked to see if they are (still) valid
* solution for storing updates to active channels in EEPROM
* adaptive datarate

# TODO's USB / CLI
* add standard flashing from USB during 10 seconds after reboot, when USB is present..
* make the trace output select SWD/Trace and/or UART2 depending on when they are present


# TODO's Sensors
* add TSL2591 - this needs a change from blocking to nonblocking sensor reading, as this sensor may need 600 ms to do a reading..