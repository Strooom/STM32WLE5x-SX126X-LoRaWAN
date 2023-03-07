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


template<typename eventType, size_t bufferLength> 
class eventBuffer {
public:
    static constexpr size_t length = bufferLength;

    eventBuffer() {} // constructor

    void push(eventType event) {
        if (static_cast<size_t>(event) % 2 == 0 && size_ < Capacity) {
            buffer_[size_++] = event;
        }
    }

    eventType pop() {
        if (size_ > 0) {
            return buffer_[--size_];
        }
        return eventType{};
    }

    bool isEmpty() const {
        return level == 0;
    }

    size_t getLevel() const {
        return level;
    }

private:
    eventType buffer_[Capacity];
    size_t level = 0;
};

