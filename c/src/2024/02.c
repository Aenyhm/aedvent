#include "common/result.h"

Arena arena;

Array_Template(Array_s64, Reports);

static Reports parse_reports(String s) {
  Reports reports = {0};

  Array_String lines = str_split(s, "\n");

  for (size_t i = 0; i < lines.count; ++i) {
    String line = lines.items[i];
    Array_String levels_as_strings = str_split(line, " ");
    Array_s64 levels = {0};
    for (size_t j = 0; j < levels_as_strings.count; ++j) {
      array_append(&levels, str_to_s64(levels_as_strings.items[j]));
    }
    array_append(&reports, levels);
  }

  return reports;
}

static bool check_delta(s64 prev_delta, s64 delta) {
  if (prev_delta) {
    if ((prev_delta > 0) != (delta > 0)) return false;
  }

  return llabs(delta) >= 1 && llabs(delta) <= 3;
}

static bool is_safe_report(Array_s64 levels) {
  bool result = true;

  s64 delta = 0;
  for (size_t i = 0; i < levels.count - 1; ++i) {
    s64 prev_delta = delta;
    s64 l1 = levels.items[i];
    s64 l2 = levels.items[i + 1];
    delta = l2 - l1;
    if (!check_delta(prev_delta, delta)) {
      result = false;
      break;
    }
  }

  return result;
}

static bool is_safe_report_with_tolerance(Array_s64 levels) {
  bool result = false;

  for (u8 j = 0; j < levels.count; ++j) {
    Array_s64 sub_levels = {0};

    for (size_t k = 0; k < levels.count; ++k) {
      if (j != k) {
        array_append(&sub_levels, levels.items[k]);
      }
    }

    if (is_safe_report(sub_levels)) {
      result = true;
      break;
    }
  }

  return result;
}

static s64 count_safe_reports(Reports reports, bool tolerance) {
  s64 result = 0;

  for (size_t i = 0; i < reports.count; ++i) {
    Array_s64 levels = reports.items[i];

    if (is_safe_report(levels) || (tolerance && is_safe_report_with_tolerance(levels))) {
      result++;
    }
  }

  return result;
}

static s64 part1(String s) { return count_safe_reports(parse_reports(s), false); }
static s64 part2(String s) { return count_safe_reports(parse_reports(s), true); }

int main() {
  arena = arena_alloc(KiB(20));

    String example = read_whole_file(&arena, "data/2024/02/example.txt");
    String input   = read_whole_file(&arena, "data/2024/02/input.txt");

    run(part1, example, 2, input, 572);
    run(part2, example, 4, input, 612);

  arena_release(&arena);

  return 0;
}
