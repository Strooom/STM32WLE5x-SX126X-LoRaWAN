#include "adaptivedatarate.h"

void AdaptiveDataRate::setDataRate(dataRate newDataRate) {
    currentDataRate = newDataRate;
}

void AdaptiveDataRate::decreaseDataRate() {
switch (currentDataRate)
{
case dataRate::DR7:
    currentDataRate = dataRate::DR6;
    break;

case dataRate::DR6:
    currentDataRate = dataRate::DR5;
    break;

case dataRate::DR5:
    currentDataRate = dataRate::DR4;
    break;

case dataRate::DR4:
    currentDataRate = dataRate::DR3;
    break;

case dataRate::DR3:
    currentDataRate = dataRate::DR2;
    break;

case dataRate::DR2:
    currentDataRate = dataRate::DR1;
    break;

case dataRate::DR1:
    currentDataRate = dataRate::DR0;
    break;

default:
    break;
}
}