#pragma once
#include "base.h"

void panic(const char *fmt, ...) {
  fprintf(stderr, "[ERROR] ");

  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  fprintf(stderr, "\n");

  exit(1);
}
