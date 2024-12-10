#pragma once
#include "../../aelibc/all.h"

typedef s64 (*Process_Function)(String);

void run(Process_Function process_fn, String data, s64 expected) {
  s64 result = process_fn(data);

  if (result == expected)  printf("✓");
  else if (expected == -1) printf("?");
  else printf("✗");

  printf(" Result found: %lli", result);

  if (result != expected && expected != -1) {
    printf(" ; Expected: %lli\n", expected);
    exit(1);
  } else {
    printf("\n");
  }
}
