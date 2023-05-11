# LoRaWAN layer TODO's

1. implement more details of the execution of the MAC commands and answers
2. reconsider frameOptions upstream, because they are required for acknowledging certain commands..
3. when using frameOptions upstream, the amount of available payload in framePayload changes..
4. channels seem to have an INDEX, so I cannot assume a contiguous list of channels in channelCollection, and we need an 'enable' for each individual channel. This has an impact on selecting a random channel
5. provide channelSelection over ALL active channels + over the 3 DEFAULT channels..(so we can send MAC stuff over those 3 only)
6. implementation of dutyCycle
7. monitor the timing of the txRxCycle on the scope and optimize the timings - they are very coarse for the time being


# LoRaWAN unitTests

* unitTest for each MAC command
  - set context
  - execute request / answer
  - check context

* test different signatures for insertPayload