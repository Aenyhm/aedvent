#pragma once
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include "arena.h"

typedef struct {
  char * data;
  size_t count;
} String;

#define str_fmt "%.*s"
#define str_arg(s) (int)(s).count, (s).data

String read_whole_file(Arena * arena, const char * file_path) {
  String result;

  FILE * file = fopen(file_path, "r");
  if (!file) {
    panic("Cannot read file %s: %s", file_path, strerror(errno));
  }

  fseek(file, 0, SEEK_END);
  result.count = ftell(file);
  fseek(file, 0, SEEK_SET);
  result.data = arena_push(arena, result.count + 1);
  fread(result.data, result.count, 1, file);
  result.data[result.count] = 0;

  fclose(file);

  return result;
}

void str_advance(String * s, size_t amount) {
  assert(amount <= s->count);
  s->data  += amount;
  s->count -= amount;
}

char * str_to_cstr(Arena * arena, String s) {
  size_t count = s.count + 1; // +1 for null-terminator
  char * value = (char *) arena_push(arena, sizeof(char *)*count);
  memcpy(value, s.data, s.count);
  value[count - 1] = '\0';
  return value;
}

String str_chop_by_delim(String * s, const char * delim) {
  size_t delim_length = strlen(delim);
  for (size_t i = 0; i < s->count; ++i) {
    if (memcmp(s->data + i, delim, delim_length) == 0) {
      String new_s = {
        .data = s->data,
        .count = i
      };
      str_advance(s, i + delim_length);
      return new_s;
    }
  }

  String new_s = *s;
  str_advance(s, s->count);
  return new_s;
}
