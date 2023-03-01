// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>

class loRaChannel{
    public:
        loRaChannel();
        void set(uint32_t frequency, uint8_t dataRate, uint8_t maxEIRP, bool enabled);
        uint32_t getFrequency() const;
        uint8_t getDataRate() const;
        uint8_t getMaxEIRP() const;
        bool isEnabled() const;
        static constexpr uint32_t length{7};        // [bytes]
    private:
        uint32_t frequency{0};          // [Hz]
        uint8_t dataRate{0};            // [0..15]
        uint8_t maxEIRP{0};             // [dBm]
        bool enabled{false};            // [true/false]
};