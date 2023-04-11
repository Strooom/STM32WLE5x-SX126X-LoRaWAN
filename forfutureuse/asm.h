#include <stdint.h>

__attribute__((always_inline)) static inline uint32_t getPRIMASK(void) {
    uint32_t result;

    __asm volatile("MRS %0, primask"
                   : "=r"(result)::"memory");
    return (result);
}

__attribute__((always_inline)) static inline void setPRIMASK(uint32_t priMask) {
    __asm volatile("MSR primask, %0"
                   :
                   : "r"(priMask)
                   : "memory");
}

__attribute__((always_inline)) static inline void enableIrq(void) {
    __asm volatile("cpsie i"
                   :
                   :
                   : "memory");
}

__attribute__((always_inline)) static inline void disableIrq(void) {
    __asm volatile("cpsid i"
                   :
                   :
                   : "memory");
}
