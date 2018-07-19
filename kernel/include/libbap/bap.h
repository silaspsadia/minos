/* **********************************************************************
 * ### "Box and Pointer" Base Data Type 
 * 
 * - The most non-trivial trivial data structure.
 * - Inspired by Lisp implementations.
 * - Intended for simplifying interfaces and implementations
 * - Cache-friendly 
 * - Algorithm-friendly
 *
 ************************************************************************/


#ifndef LIBBAP_H
#define LIBBAP_H

#include <stdint.h>

// "Classic flavor" bap

typedef struct bap {
	int first;
	int second;
} bap_t;

// Width-specified baps

typedef struct bap16 { 
	uint8_t first;	
	uint8_t second;	
} bap16_t;

typedef struct bap32 {
	uint16_t first;	
	uint16_t second;	
} bap32_t;

typedef struct bap64 {
	uint32_t first;	
	uint32_t second;	
} bap64_t;

typedef struct bap128 {
	uint64_t first;	
	uint64_t second;	
} bap128_t;

#endif /* LIBBAP_H */
