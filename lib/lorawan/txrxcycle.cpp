#include "txrxcycle.h"

const char* toString(txRxCycleState theState) {
    switch (theState) {
        case txRxCycleState::idle:
            return "idle";
            break;
        case txRxCycleState::waitForCadEnd:
            return "waitForCadEnd";
            break;
        case txRxCycleState::waitForRandomTimeBeforeTransmit:
            return "waitForRandomTimeBeforeTransmit";
            break;
        case txRxCycleState::waitForTxComplete:
            return "waitForTxComplete";
            break;
        case txRxCycleState::waitForRx1Start:
            return "waitForRx1Start";
            break;
        case txRxCycleState::waitForRx1CompleteOrTimeout:
            return "waitForRx1CompleteOrTimeout";
            break;
        case txRxCycleState::waitForRx2Start:
            return "waitForRx2Start";
            break;
        case txRxCycleState::waitForRx2CompleteOrTimeout:
            return "waitForRx2CompleteOrTimeout";
            break;
        case txRxCycleState::waitForRxMessageReadout:
            return "waitForRxMessageReadout";
            break;
        default:
            return "unknown state";
            break;
    }
}

const char* toString(loRaWanEvent anEvent) {
    switch (anEvent) {
        case loRaWanEvent::none:
            return "none";
            break;
        case loRaWanEvent::sx126xCadEnd:
            return "sx126x Cad End";
            break;
        case loRaWanEvent::sx126xTxComplete:
            return "sx126x Tx Complete";
            break;
        case loRaWanEvent::sx126xRxComplete:
            return "sx126x Rx Complete";
            break;
        case loRaWanEvent::sx126xTimeout:
            return "sx126x Timeout";
            break;
        case loRaWanEvent::timeOut:
            return "timeOut";
            break;
        default:
            return "unknown event";
            break;
    }
}