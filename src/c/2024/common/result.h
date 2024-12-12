#pragma once
#include <time.h>
#include "../../aelibc/all.h"

typedef s64 (*Process_Function)(String);

static void run_part(const char * label, Process_Function process_fn, String data, s64 expected, bool profile) {
  struct timespec start, end;

  clock_gettime(CLOCK_MONOTONIC, &start);
  s64 result = process_fn(data);
  clock_gettime(CLOCK_MONOTONIC, &end);

  u64 elapsed_ns = (end.tv_sec - start.tv_sec)*1000000000 + (end.tv_nsec - start.tv_nsec);

  if (result == expected)  printf("✓");
  else if (expected == -1) printf("?");
  else printf("✗");

  printf(" [%s] Result found: %lli", label, result);
  if (result != expected && expected != -1) {
    printf(" ; Expected: %lli\n", expected);
    exit(1);
  }

  if (profile) {
    u8 spacing = 20;
    for (u8 i = 0; i < spacing - count_digits(result); ++i) printf(" ");
    printf("(in %.3f ms)", elapsed_ns/1000000.);
  }

  printf("\n");
}

void run(
  Process_Function process_fn,
  String example,
  s64 example_expected,
  String input,
  s64 input_expected
) {
  run_part("Example", process_fn, example, example_expected, false);
  run_part(" Input ", process_fn, input, input_expected, true);
  printf("\n");
}
