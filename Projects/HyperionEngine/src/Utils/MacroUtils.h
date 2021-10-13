#pragma once

#define MACRO_ASSERT(condition) extern int macro_assert[1 - 2 * !!(condition)];
#define NUM_ARGS(...) (sizeof((int[]){0, ##__VA_ARGS__}) / sizeof(int) - 1)
#define AS_COUNTABLE(_) char _##_Countable;
#define COUNT(_) sizeof(struct { _(AS_COUNTABLE) } __attribute__((packed)))
