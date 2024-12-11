#include "common/result.h"
#include <math.h>

Arena arena;

typedef struct {
  s64 value;
  Array_s64 operands;
} Equation;
Array_Template(Equation, Equations);

typedef s64 (*Operation)(s64, s64);
static s64 sum(s64, s64);
static s64 mul(s64, s64);
static s64 concat(s64, s64);
Operation operations[] = {sum, mul, concat};

static Equation line_to_equation(String s) {
  Equation result = {0};

  result.value = str_to_s64(str_chop_by_delim(&s, ": "));

  Array_String operands_as_string = str_split(s, " ");
  array_map(operands_as_string, str_to_s64, &result.operands);

  return result;
}

static s64 sum(s64 a, s64 b) { return a + b; }
static s64 mul(s64 a, s64 b) { return a*b; }
static s64 concat(s64 a, s64 b) {
  s64 pow = 10;
  while (b >= pow) pow *= 10;

  return a*pow + b;
}

static bool check_combination(Equation equation, size_t combination_index, u8 operation_count) {
  s64 value = equation.operands.items[0];

  size_t operators_count = equation.operands.count - 1;
  for (size_t i = 0; i < operators_count; ++i) {
    size_t operator_type = (combination_index/(size_t)pow(operation_count, i)) % operation_count;
    Operation op = operations[operator_type];

    value = op(value, equation.operands.items[i + 1]);

    if (value > equation.value) break;
  }

  return value == equation.value;
}

static bool is_valid_equation(Equation equation, u8 operation_count) {
  bool result = false;

  size_t operators_count = equation.operands.count - 1;
  size_t total_combinations = pow(operation_count, operators_count);

  for (size_t i = 0; i < total_combinations; ++i) {
    if (check_combination(equation, i, operation_count)) {
      result = true;
      break;
    }
  }

  return result;
}

static Equations parse_input(String s) {
  Equations equations = {0};

  Array_String lines = str_split(s, "\n");
  array_map(lines, line_to_equation, &equations);

  return equations;
}

static s64 count_total_calibrations(Equations equations, u8 operation_count) {
  s64 result = 0;

  for (size_t i = 0; i < equations.count; ++i) {
    Equation equation = equations.items[i];
    if (is_valid_equation(equation, operation_count)) {
      result += equation.value;
    }
  }

  return result;
}

static s64 part1(String s) { return count_total_calibrations(parse_input(s), 2); }
static s64 part2(String s) { return count_total_calibrations(parse_input(s), 3); }

int main() {
  arena = arena_alloc(KiB(100));

    String example  = read_whole_file(&arena, "data/2024/07/example.txt");
    String input    = read_whole_file(&arena, "data/2024/07/input.txt");

    run(part1, example,  3749, input, 1298300076754);
    run(part2, example, 11387, input, 248427118972289);

  arena_release(&arena);

  return 0;
}
