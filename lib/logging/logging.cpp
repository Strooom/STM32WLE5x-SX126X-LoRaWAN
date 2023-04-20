// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "logging.h"        //

// #include <stdint.h>        // required for uint8_t and similar type definitions
// #include <string.h>        // required for strncpy()

bool logging::isDebugProbePresent(){
    return debugProbePresent;
}
