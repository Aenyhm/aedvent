#pragma once
#include <limits.h>

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
