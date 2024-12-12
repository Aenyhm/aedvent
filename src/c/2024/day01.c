#include "common/result.h"

typedef s64 (*Lookup)(s64 * left_col, s64 * right_col, size_t col_size);

static s64 lookup_part1(s64 * left_col, s64 * right_col, size_t col_size) {
  s64 result = 0;

  for (size_t i = 0; i < col_size; ++i) {
    s64 distance = llabs(left_col[i] - right_col[i]);
    result += distance;
  }

  return result;
}

static s64 lookup_part2(s64 * left_col, s64 * right_col, size_t col_size) {
  s64 result = 0;

  s64 left_occurences = 1;
  size_t right_index = 0;
  for (size_t left_index = 0; left_index < col_size; left_index++) {
    s64 lhs = left_col[left_index];
    if (lhs == left_col[left_index + 1]) {
      left_occurences++;
    } else {
      size_t right_occurences = 0;
      while (right_index < col_size) {
        s64 rhs = right_col[right_index];
        if (rhs == lhs) right_occurences++;
        if (rhs <= lhs) {
          right_index++;
        } else {
          break;
        }
      }
      result += lhs*left_occurences*right_occurences;
      left_occurences = 1;
    }
  }

  return result;
}

static s64 process(String s, Lookup lookup) {
  Array_String lines = str_split(s, "\n");

  s64 left_col[lines.count];
  s64 right_col[lines.count];

  for (size_t i = 0; i < lines.count; ++i) {
    String line = lines.items[i];
    String lhs = str_chop_by_delim(&line, "   ");
    String rhs = line;
    left_col[i]  = str_to_s64(lhs);
    right_col[i] = str_to_s64(rhs);
  }

  qsort(left_col, lines.count, sizeof(s64), compare_int);
  qsort(right_col, lines.count, sizeof(s64), compare_int);

  return lookup(left_col, right_col, lines.count);
}

static s64 part1(String s) { return process(s, lookup_part1); }
static s64 part2(String s) { return process(s, lookup_part2); }

int main() {
  Arena arena = arena_alloc(KiB(20));

    String example = read_whole_file(&arena, "data/2024/01/example.txt");
    String input   = read_whole_file(&arena, "data/2024/01/input.txt");

    run(part1, example, 11, input, 1506483);
    run(part2, example, 31, input, 23126924);

  arena_release(&arena);

  return 0;
}
