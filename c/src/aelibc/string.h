#pragma once
#include "base.h"
#include "arena.h"
#include "array.h"
#include "number.h"

typedef struct {
  char * data;
  size_t count;
} String;

Array_Template(String, Array_String);

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
  result.data = arena_push_array(arena, char, result.count + 1);
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

bool str_eq(String s1, String s2) {
  return s1.count == s2.count && memcmp(s1.data, s2.data, s1.count) == 0;
}

String cstr_to_str(char * cstr) {
  String s = {0};
  s.data = cstr;
  s.count = strlen(cstr);
  return s;
}

char * str_to_cstr(Arena * arena, String s) {
  size_t count = s.count + 1; // +1 for null-terminator
  char * value = arena_push_array(arena, char, count);
  memcpy(value, s.data, s.count);
  value[count - 1] = '\0';
  return value;
}

bool str_starts_with(String s, const char * cstr) {
  return memcmp(s.data, cstr, strlen(cstr)) == 0;
}

String str_sub(Arena * arena, String s, size_t start, size_t end) {
  assert(start < end && end <= s.count);

  String result = {0};
  result.count = end - start;
  result.data = arena_push_array(arena, char, result.count);
  memcpy(result.data, s.data + start, result.count);
  return result;
}

String str_chop_by_delim(String * s, const char * delim) {
  u32 delim_length = strlen(delim);
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

Array_String str_split(String s, const char * delim) {
  Array_String strings = {0};
  while (s.count) {
    array_append(&strings, str_chop_by_delim(&s, delim));
  }

  return strings;
}

int str_to_int(String s) {
  int result;

  int scan_result = sscanf(s.data, "%d", &result);
  if (scan_result == 0 || (size_t)count_digits(result) != s.count) result = -1;

  return result;
}
