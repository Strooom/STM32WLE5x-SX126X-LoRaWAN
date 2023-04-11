# TODO's LoRaWAN
* get LowPower Timer working for timing the TxRx1Rx2Cycle
* finalize TxRex stateMachine



# General
* detect if a debug-probe is present : The debugger can be detected by examining the DBGMCU_CR, the control register for the debug peripheral within the MCU.  The bottom 3 bits default to 0 after a power-on reset, but the debugging session will set some or all of them high when it attaches (except in very specific circumstances where you’re changing the debugger settings).  So we can just examine these bits to infer if a debugger is attached:

// Set this appropriate to your stm32 processor. 
#include "stm32fxxx.h" 
#define IS_DEBUGGER_ATTACHED() (DBGMCU-&gt;CR &amp; 0x07)

* then use this presence in the logging class..
* logging can be seen in CubeProgrammer under SWV
