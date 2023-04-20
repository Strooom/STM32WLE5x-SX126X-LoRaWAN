# TODO's LoRaWAN

* unit tests sensor - oversampling/prescaler 0 : disabled
* after booting, add an event into the measurements, with immediate priority so we send something immediately and we can monitor that at the livedata from the gateway


# General
* make logging class completely static
* check other classes to become static as well

test EPOCH stuff https://www.epochconverter.com/programming/c

# Performance Improvements
* For all channels, I could precalculate the frequency register value and store it in the channel object.

