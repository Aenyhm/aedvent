#include "common/result.h"

Arena arena;

typedef struct {
  s64 value;
  Array_s64 operands;
} Equation;

Array_Template(Equation, Equations);
Array_Template(bool *, Combinations);

static Equation parse_line(String s) {
  Equation result = {0};

  result.value = str_to_s64(str_chop_by_delim(&s, ": "));
  array_map(str_split(s, " "), str_to_s64, &result.operands);

  return result;
}

static s64 compute_combination(Array_s64 operands, size_t combination_index) {
  s64 value = operands.items[0];

  size_t operators_count = operands.count - 1;
  for (size_t i = 0; i < operators_count; ++i) {
    bool operator_bit = (combination_index >> i) & 1;
    if (operator_bit) {
      value *= operands.items[i + 1];
    } else {
      value += operands.items[i + 1];
    }
  }

  return value;
}

static bool is_valid_equation(Equation equation) {
  bool result = false;

  size_t operators_count = equation.operands.count - 1;
  size_t total_combinations = 1 << operators_count;

  for (size_t i = 0; i < total_combinations; ++i) {
    s64 combination_result = compute_combination(equation.operands, i);
    if (combination_result == equation.value) {
      result = true;
      break;
    }
  }

  return result;
}

static s64 count_total_calibrations(String s) {
  s64 result = 0;

  Array_String lines = str_split(s, "\n");
  Equations equations;
  array_map(lines, parse_line, &equations);

  for (size_t i = 0; i < equations.count; ++i) {
    Equation equation = equations.items[i];
    if (is_valid_equation(equation)) {
      result += equation.value;
    }
  }

  return result;
}

int main() {
  arena = arena_alloc(KiB(100));

    String example  = read_whole_file(&arena, "data/2024/07/example.txt");
    String input    = read_whole_file(&arena, "data/2024/07/input.txt");

    run(count_total_calibrations, example, 3749);
    run(count_total_calibrations, input, 1298300076754);

    // TODO:  part 2

  arena_release(&arena);

  return 0;
}
