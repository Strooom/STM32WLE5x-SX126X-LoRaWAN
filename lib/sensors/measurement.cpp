#include "measurement.h"

#ifndef environment_desktop

#include "main.h"

extern RTC_HandleTypeDef hrtc;

uint32_t measurement::getTimeStamp() {
    RTC_TimeTypeDef currTime;
    RTC_DateTypeDef currDate;

    HAL_RTC_GetTime(&hrtc, &currTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &currDate, RTC_FORMAT_BIN);

    uint32_t days    = ((36525 * currDate.Year) / 100) + ((306001 * (currDate.Month + 1)) / 10000) + currDate.Date - 719606;
    uint32_t seconds = currTime.Seconds + (currTime.Minutes * 60) + (currTime.Hours * 3600);
    return ((days * 86400) + seconds);

    //((36525 * Year) / 100) + ((306001 * (Month + 1)) / 10000) + Day - 719606;
    // Second += (Minute * 60) + (Hour * 3600);
}

#else

uint32_t measurement::getTimeStamp() {
    return 0;
}

#endif
