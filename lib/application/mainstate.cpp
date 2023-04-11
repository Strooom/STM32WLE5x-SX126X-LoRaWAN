// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "mainstate.h"

const char *toString(mainState aState) {
    switch (aState) {
        case mainState::boot:
            return "boot";
            break;
            
        default:
            return "unknown";
            break;
    }
}
