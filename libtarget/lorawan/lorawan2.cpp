#include "lorawan.h"
#include "main.h"

extern RNG_HandleTypeDef hrng;
extern LPTIM_HandleTypeDef hlptim1;

uint32_t LoRaWAN::getRandomNumber() {
    uint32_t result{0};
    HAL_RNG_GenerateRandomNumber(&hrng, &result);
    return result;
}

void LoRaWAN::startTimer(uint32_t timeOut) {
    HAL_LPTIM_TimeOut_Start_IT(&hlptim1, timeOut, timeOut);
}

void LoRaWAN::stopTimer() {
    HAL_LPTIM_Counter_Stop(&hlptim1);
}