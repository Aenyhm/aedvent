#include "../aelibc/all.h"
#include "common/result.h"

Arena arena;

#define MUL_INSTR  cstr_to_str("mul(")
#define DO_INSTR   cstr_to_str("do()")
#define DONT_INSTR cstr_to_str("don't()")

static s64 count_next_digits(String s, u8 offset) {
  s64 result = 0;

  for (size_t i = 0; i < 3; ++i) {
    u8 c = s.data[offset + i];
    if (isdigit(c)) result++;
    else break;
  }

  return result;
}

static s64 get_mul_length(String s) {
  s64 result = MUL_INSTR.count;

  // step 1: left digits
  s64 left_digits_count = count_next_digits(s, result);
  bool success = left_digits_count >= 1;
  result += left_digits_count;

  // step 2: comma
  if (success) {
    success = s.data[result] == ',';
    result++;
  }

  // step 3: right digits
  if (success) {
    s64 right_digits_count = count_next_digits(s, result);
    success = right_digits_count >= 1;
    result += right_digits_count;
  }

  // step 4: right parenthesis
  if (success) {
    success = s.data[result] == ')';
    result++;
  }

  return success ? result : 0;
}

static Array_String parse_instructions(String * s) {
  Array_String result = {0};

  while (s->count > DONT_INSTR.count) {
    if (str_starts_with(*s, MUL_INSTR.data)) {
      s64 mul_length = get_mul_length(*s);
      if (mul_length) {
        String mul_string = str_sub(&arena, *s, 0, mul_length);
        array_append(&result, mul_string);
        str_advance(s, mul_length);
      } else {
        str_advance(s, 1);
      }
    } else if (str_starts_with(*s, DO_INSTR.data)) {
      String do_string = str_sub(&arena, *s, 0, DO_INSTR.count);
      array_append(&result, do_string);
      str_advance(s, DO_INSTR.count);
    } else if (str_starts_with(*s, DONT_INSTR.data)) {
      String dont_string = str_sub(&arena, *s, 0, DONT_INSTR.count);
      array_append(&result, dont_string);
      str_advance(s, DONT_INSTR.count);
    } else {
      str_advance(s, 1);
    }
  }

  return result;
}

static s64 compute_mul(String * s) {
  str_advance(s, MUL_INSTR.count);
  String lhs = str_chop_by_delim(s, ",");
  s64 x = str_to_s64(lhs);
  String rhs = str_chop_by_delim(s, ")");
  s64 y = str_to_s64(rhs);

  return x*y;
}

static s64 process_instructions(Array_String instructions, bool with_do_and_dont) {
  s64 result = 0;

  bool enable_mul = true;
  for (size_t i = 0; i < instructions.count; ++i) {
    String instr = instructions.items[i];
    if (str_starts_with(instr, MUL_INSTR.data) && (enable_mul || !with_do_and_dont)) {
      result += compute_mul(&instr);
    } else if (with_do_and_dont) {
      if (str_eq(instr, DO_INSTR)) {
        enable_mul = true;
      } else if (str_eq(instr, DONT_INSTR)) {
        enable_mul = false;
      }
    }
  }

  return result;
}

static s64 part1(String s) { return process_instructions(parse_instructions(&s), false); }
static s64 part2(String s) { return process_instructions(parse_instructions(&s), true); }

int main() {
  arena = arena_alloc(KiB(100));

    String example1 = read_whole_file(&arena, "data/2024/03/example1.txt");
    String input    = read_whole_file(&arena, "data/2024/03/input.txt");
    String example2 = read_whole_file(&arena, "data/2024/03/example2.txt");

    run(part1, example1, 161, input, 171183089);
    run(part2, example2,  48, input, 63866497);

  arena_release(&arena);

  return 0;
}
