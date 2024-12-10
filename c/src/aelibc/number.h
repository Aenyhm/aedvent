#pragma once
#include <limits.h>

#include "base.h"
#include "array.h"
#include "string.h"

Array_Template(s64, Array_s64);

int compare(const void * a, const void * b) {
  int int_a = *((int *)a);
  int int_b = *((int *)b);

  if (int_a == int_b) return 0;
  else if (int_a < int_b) return -1;
  else return 1;
}

int count_digits(int n) {
  if (n < 0)  return count_digits(n == INT_MIN ? INT_MAX: -n);
  if (n < 10) return 1;
  return 1 + count_digits(n/10);
}

s64 str_to_s64(String s) {
  char * endptr = s.data + s.count;

  s64 result = strtoll(s.data, &endptr, 10);

  if (endptr == s.data) {
    printf("Failed to cast str " str_fmt " to s64: invalid characters.\n", str_arg(s));
    return -1;
  }

  if (errno == ERANGE) {
    printf("Failed to cast str " str_fmt " to s64: out of range.\n", str_arg(s));
    return -1;
  }

  return result;
}
