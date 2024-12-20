#include "common/result.h"

Arena arena;

static Array_s64 diskmap_to_ids(String s) {
  Array_s64 result = {0};

  size_t current_id = 0;
  for (size_t i = 0; i < s.count; ++i) {
    u8 c = s.data[i];
    if (c == '\n') continue;

    // character to number
    u8 digit = c - '0';

    if (i % 2 == 0) {
      for (u8 j = 0; j < digit; ++j) {
        array_append(&result, current_id);
      }
      current_id++;
    } else {
      for (u8 j = 0; j < digit; ++j) {
        array_append(&result, -1);
      }
    }
  }

  return result;
}

static void compress(Array_s64 * ids) {
  size_t begin_cursor = 0;
  size_t end_cursor = ids->count;

  for (size_t i = ids->count - 1; i > 0; --i) {
    s64 end_digit = ids->items[i];

    if (end_digit != -1) {
      for (size_t j = begin_cursor; j < ids->count; ++j) {
        s64 begin_digit = ids->items[j];

        if (begin_digit == -1) {
          ids->items[j] = end_digit;
          end_cursor--;
          break;
        }
      }
    }
  }

  ids->count = end_cursor;
}

static s64 checksum(Array_s64 ids) {
  s64 result = 0;

  for (size_t i = 0; i < ids.count; ++i) {
    result += i*ids.items[i];
  }

  return result;
}

static s64 process(String s) {
  Array_s64 ids = diskmap_to_ids(s);

  compress(&ids);

  return checksum(ids);
}

static s64 part1(String s) { return process(s); }

int main() {
  arena = arena_alloc(KiB(100));

    String example  = read_whole_file(&arena, "data/2024/09/example.txt");
    String input    = read_whole_file(&arena, "data/2024/09/input.txt");

    run(part1, example, 1928, input, 6415184586041);
    // TODO: part 2

  arena_release(&arena);

  return 0;
}
