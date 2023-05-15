// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>
#include "datarate.h"

class loRaChannel {
  public:
    loRaChannel(bool enabled, uint32_t frequency, uint32_t minimumDataRateIndex, uint32_t maximumDataRateIndex);

//#ifndef unitTesting
//  private:
//#endif
    
    bool enabled{false};
    uint32_t frequency{0};
    uint32_t minimumDataRateIndex{0};
    uint32_t maximumDataRateIndex{0};

    friend class loRaChannelCollection;
};

