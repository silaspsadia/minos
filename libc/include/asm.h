#ifndef _LIBC_ASM_H_
#define _LIBC_ASM_H_ 1

#include <stdint.h>

inline void invlpg(void* m) {
  asm volatile("invlpg (%0)" : : "b"(m) : "memory");
}

#endif  // _LIBC_ASM_H_
