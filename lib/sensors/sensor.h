// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>

// Base class for a sensor - All sensor classes derive from it to get their common methods
// I would like to make isPresent() virtual as well, so all derived classes have to implement it. But it needs to be static function, as we
// want to call it before allocating an object.. In C++ it seems impossible to combine static and virtual

class sensor {
  protected:
    sensor();

  public:
    virtual bool initialize() = 0;
    bool isPresent();
    virtual bool goSleep() = 0;
};