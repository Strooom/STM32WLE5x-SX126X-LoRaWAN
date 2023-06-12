1. SX126x TX : peak of 60mA, but typically shown as 25 mA
2. SX126 Rx - much lower
3. SX126 in standby : adds ~2mA -> enable the radio only when Tx or Rx

4. MCU @ 16 MHz : with IOs enabled, 2.3 mA
5. Using LowPowerAutoPowerOff on ADC has no measurable improvement...

6. Put RTC in economic mode by clocking it differently

7. running without debug probe, lowers 0.1 mA