#ifndef _MATH_H
#define _MATH_H 

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t div_ceil(uint32_t, uint32_t);
uint32_t maxu(uint32_t, uint32_t);
uint32_t min(uint32_t, uint32_t);

#ifdef __cplusplus
}
#endif

#endif/* _MATH_H */
