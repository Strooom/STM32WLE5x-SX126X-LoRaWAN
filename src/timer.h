#include <stdint.h>

class timer {

    public:
        timer();
        void start(uint32_t duration);
        bool expired() const;
        void stop();
};