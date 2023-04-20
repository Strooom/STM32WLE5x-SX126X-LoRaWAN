## RTC takes a 30 second tick to wake up the MCU
* whenever a sample is needed, we initialize the peripherals for it.
* each peripheral remembers its initialized state, so we don't initialize when this is already done
* only at the end, we de-initialize the peripherals, as many sensors may need the same I2C bus

## when we use uplink time randomization, we randomize over 15 seconds = 1/2 of the RTC tick


## for the display
* we update at least once every 24 hours, even if there is no new data : this is to keep the image fresh
* then we update (optionally) when the data has changed



## sensors
* battery voltage: 
    - running on battery : sample every 10 minutes, average * 8 -> new value every 80 minutes
    - running on USB power (= charging) : sample every 30 seconds, average * 8 -> new value every 4 minutes

* temperature
* humidity
* light
* barometer
* VOCs