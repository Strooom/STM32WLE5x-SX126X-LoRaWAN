#include "collisionprevention.h"

const char* toString(collisionPreventionMode aMode) {
    switch (aMode) {
        case collisionPreventionMode::none:
            return "none";
            break;
        case collisionPreventionMode::randomTxTime:
            return "randomTxTime";
            break;
        case collisionPreventionMode::useCAD:
            return "useCAD";
            break;
        default:
            return "unknown mode";
            break;
    }
}