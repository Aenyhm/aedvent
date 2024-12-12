#pragma once
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef char       s8;          // ±127
typedef short      s16;         // ±32 767
typedef long       s32;         // ±2 147 483 647
typedef long long  s64;         // ±9 223 372 036 854 775 807

typedef unsigned char      u8;  // 255
typedef unsigned short     u16; // 65 535
typedef unsigned long      u32; // 4 294 967 295
typedef unsigned long long u64; // 18 446 744 073 709 551 615

#define KiB(n) 1024*(n)
#define MiB(n) 1024*KiB(n)

#define UNREACHABLE assert(false && "unreachable")
