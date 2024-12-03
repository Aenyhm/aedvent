#pragma once
#include "aelibc/base.h"
#include "aelibc/arena.h"
#include "aelibc/array.h"
#include "aelibc/string.h"

#define MAX_LINES 1000

Array_Template(String, Lines);

static Lines parse_file(Arena * arena, const char * file_path) {
  Lines lines;
  array_init(arena, lines, MAX_LINES);

  String s = read_whole_file(arena, file_path);
  while (s.count) array_append(&lines, str_chop_by_delim(&s, "\n"));

  return lines;
}
