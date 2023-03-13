// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

// template class for event buffers. Each layer needs it's own buffer, and the events are different types
// instantiate as
//      eventBuffer<sx126xEvent, static_cast<size_t>(16)> radioBuffer;

template <typename eventType, size_t bufferLength>
class eventBuffer {
  public:
    static constexpr size_t length = bufferLength;

    eventBuffer() {
        initialize();
    };

    void initialize() {
        head     = 0;
        level    = 0;
        levelMax = 0;
    };

    void push(eventType event){};

    eventType pop() {
        return eventType{};
    };

    bool isEmpty() const {
        return level == 0;
    };

    bool hasEvents() const {
        return level > 0;
    };

    size_t getLevel() const {
        return level;
    };

    size_t getLevelMax() { return levelMax; };        // TODO, this should reset the levelMax after reading it

  private:
    size_t head{0};
    size_t level{0};
    size_t levelMax{0};
    eventType theBuffer[bufferLength];
};
