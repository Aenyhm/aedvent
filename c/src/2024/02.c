#include "../aelibc/all.h"

Array_Template(u8, Levels);
Array_Template(Levels, Reports);

static Reports parse_reports(String s) {
  Reports reports;

  Array_String lines = str_split(s, "\n");

  for (size_t i = 0; i < lines.count; ++i) {
    String line = lines.items[i];
    Array_String levels_as_strings = str_split(line, " ");
    Levels levels = {0};
    for (size_t j = 0; j < levels_as_strings.count; ++j) {
      array_append(&levels, str_to_int(levels_as_strings.items[j]));
    }
    array_append(&reports, levels);
  }

  return reports;
}

static bool check_delta(s8 prev_delta, s8 delta) {
  if (prev_delta) {
    if ((prev_delta > 0) != (delta > 0)) return false;
  }

  return abs(delta) >= 1 && abs(delta) <= 3;
}

static bool is_safe_report(Levels levels) {
  bool result = true;

  s8 delta = 0;
  for (u8 i = 0; i < levels.count - 1; ++i) {
    s8 prev_delta = delta;
    u8 l1 = levels.items[i];
    u8 l2 = levels.items[i + 1];
    delta = l2 - l1;
    if (!check_delta(prev_delta, delta)) {
      result = false;
      break;
    }
  }

  return result;
}

static bool is_safe_report_with_tolerance(Levels levels) {
  bool result = false;

  for (u8 j = 0; j < levels.count; ++j) {
    Levels sub_levels = {0};

    for (u8 k = 0; k < levels.count; ++k) {
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

static u32 count_safe_reports(Reports reports, bool tolerance) {
  u32 result = 0;

  for (size_t i = 0; i < reports.count; ++i) {
    Levels levels = reports.items[i];

    if (is_safe_report(levels) || (tolerance && is_safe_report_with_tolerance(levels))) {
      result++;
    }
  }

  return result;
}

int main() {
  Arena arena = arena_alloc(KiB(20));

    String example = read_whole_file(&arena, "data/2024/02/example.txt");
    String input   = read_whole_file(&arena, "data/2024/02/input.txt");

    Reports example_reports = parse_reports(example);
    Reports input_reports   = parse_reports(input);

    assert(count_safe_reports(example_reports, false) == 2);
    assert(count_safe_reports(input_reports, false) == 572);

    assert(count_safe_reports(example_reports, true) == 4);
    assert(count_safe_reports(input_reports, true) == 612);

  arena_release(&arena);

  return 0;
}
