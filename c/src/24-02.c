#include "common.h"

#define MAX_LEVELS 20

Array_Template(u8, Levels);
Array_Template(Levels, Reports);

static Levels line_to_levels(Arena * arena, String * line) {
  Levels result;
  array_init(arena, result, MAX_LEVELS);
  while (line->count) {
    String level_string = str_chop_by_delim(line, " ");
    array_append(&result, atoi(str_to_cstr(arena, level_string)));
  }

  return result;
}

static Reports file_to_reports(Arena * arena, const char * file_path) {
  Reports reports;

  Lines lines = parse_file(arena, file_path);

  array_init(arena, reports, lines.count);
  for (size_t i = 0; i < lines.count; ++i) {
    String line = lines.items[i];
    Levels levels = line_to_levels(arena, &line);
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

static bool is_safe_report_with_tolerance(Arena * arena, Levels levels) {
  bool result = false;

  for (u8 j = 0; j < levels.count; ++j) {
    Levels sub_levels;
    array_init(arena, sub_levels, levels.count);
    sub_levels.count = levels.count;
    sub_levels.ordered = true;

    memcpy(sub_levels.items, levels.items, levels.count);
    array_remove_at_index(&sub_levels, j);

    if (is_safe_report(sub_levels)) {
      result = true;
      break;
    }
  }

  return result;
}

static u32 count_safe_reports(Arena * arena, Reports reports, bool tolerance) {
  u32 result = 0;

  for (size_t i = 0; i < reports.count; ++i) {
    Levels levels = reports.items[i];

    if (is_safe_report(levels) || (tolerance && is_safe_report_with_tolerance(arena, levels))) {
      result++;
    }
  }

  return result;
}

int main() {
  Arena arena = arena_alloc(KiB(400));
    Reports example_reports = file_to_reports(&arena, "data/2024/02/example.txt");
    Reports input_reports   = file_to_reports(&arena, "data/2024/02/input.txt");

    assert(count_safe_reports(&arena, example_reports, false) == 2);
    assert(count_safe_reports(&arena, input_reports, false) == 572);
    assert(count_safe_reports(&arena, example_reports, true) == 4);
    assert(count_safe_reports(&arena, input_reports, true) == 612);

  arena_release(&arena);

  return 0;
}
