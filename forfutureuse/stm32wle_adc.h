// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include "main.h"
#include <stdint.h>

#include "adcchannel.h"
#include "stm32wle_peripheral.h"

class stm32wleAdc : public stm32wlePeripheral {
  public:
    void initialize();
    void prepareForSleep();
    void restoreAfterSleep();
    uint32_t convert();
    bool isReady() const;

  private:
    void selectChannel(adcChannel aChannel);

    static constexpr uint32_t ADCRDY{0};            // bit index into ADC_ISR
    static constexpr uint32_t EOC{2};               // bit index into ADC_ISR
    static constexpr uint32_t ADCAL{31};            // bit index into ADC_CR
    static constexpr uint32_t ADVREGEN{28};         // bit index into ADC_CR
    static constexpr uint32_t ADSTART{2};           // bit index into ADC_CR
    static constexpr uint32_t ADEN{0};              // bit index into ADC_CR
    static constexpr uint32_t AUTOFF{15};           // bit index into ADC_CFGR1
    static constexpr uint32_t CHSELRMOD{21};        // bit index into ADC_CFGR1

    static constexpr uint32_t VREFINT_CHANNEL{13};        // index of the internal voltage reference channel
    static constexpr uint32_t VREFEN{22};                 // bit index into ADC_CCR
};
