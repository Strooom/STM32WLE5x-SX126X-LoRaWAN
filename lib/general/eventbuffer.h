// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

// template class for event buffers. Each layer needs it's own buffer, and the events are different types
// This is a circular buffer, so it works FIFO
// push() on an already full buffer, overwrites the oldes event.
// pop() on an empty buffer, returns 0x00
// TODO : this needs to be made thread safe with critical sections. Ideally in this class, but if not possible, then in code that calls it

template <typename eventType, uint32_t bufferLength>
class eventBuffer {
  public:
    static constexpr uint32_t length = bufferLength;

    eventBuffer() {
        initialize();
    };

    void initialize() {
        head  = 0;
        level = 0;
        // levelMax = 0;
    };

    void push(eventType event) {
        theBuffer[(head + level) % bufferLength] = event;
        if (level < bufferLength) {
            level++;
        } else {
            head = (head + 1) % bufferLength;
        }
    };

    eventType pop() {
        if (level > 0) {
            eventType result = theBuffer[head];
            head             = (head + 1) % bufferLength;
            level--;
            return result;
        } else {
            return static_cast<eventType>(0x00);
        }
    };

    bool isEmpty() const {
        return level == 0;
    };

    bool hasEvents() const {
        return level > 0;
    };

    uint32_t getLevel() const {
        return level;
    };

        // uint32_t getLevelMax() {
        //     return levelMax;
        // };        // TODO, this should reset the levelMax after reading it

#ifndef unitTesting
//  private:
#endif
    uint32_t head{0};
    uint32_t level{0};
    eventType theBuffer[bufferLength];
    // uint32_t levelMax{0};
};
