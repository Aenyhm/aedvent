#include "common/ascii.h"
#include "common/result.h"

Arena arena;

#define WORD1 "XMAS"
#define WORD2 "MAS"

typedef u8 (*Lookup_Function)(Ascii_Grid grid, int row, int col);

static u8 part1_lookup(Ascii_Grid grid, int row, int col) {
  u8 result = 0;

  // 8 directions
  for (s8 x = -1; x <= 1; ++x) {
    for (s8 y = -1; y <= 1; ++y) {
      if (x == 0 && y == 0) continue;

      bool success = true;
      for (u8 i = 0; i < strlen(WORD1); ++i) {
        int delta_x = row + x*i;
        int delta_y = col + y*i;
        if (find_char_at(grid, delta_x, delta_y) != WORD1[i]) {
          success = false;
          break;
        }
      }

      if (success) result++;
    }
  }

  return result;
}

static u8 part2_lookup(Ascii_Grid grid, int row, int col) {
  u8 result = 0;

  // start at center
  bool success = find_char_at(grid, row, col) == WORD2[1];

  if (success) {
    u8 upper_left  = find_char_at(grid, row - 1, col - 1);
    u8 upper_right = find_char_at(grid, row + 1, col - 1);
    u8 lower_left  = find_char_at(grid, row - 1, col + 1);
    u8 lower_right = find_char_at(grid, row + 1, col + 1);

    bool word1_ok = (
      (upper_left == WORD2[0] && lower_right == WORD2[2]) ||
      (upper_left == WORD2[2] && lower_right == WORD2[0])
    );
    bool word2_ok = (
      (upper_right == WORD2[0] && lower_left == WORD2[2]) ||
      (upper_right == WORD2[2] && lower_left == WORD2[0])
    );

    if (word1_ok && word2_ok) result = 1;
  }

  return result;
}

static u32 count_words(Ascii_Grid grid, Lookup_Function lookup_fn) {
  u32 result = 0;

  for (int col = 0; col < grid.height; ++col) {
    for (int row = 0; row < grid.width; ++row) {
      result += lookup_fn(grid, row, col);
    }
  }

  return result;
}

static s64 part1(String s) { return count_words(parse_grid(&arena, s), part1_lookup); }
static s64 part2(String s) { return count_words(parse_grid(&arena, s), part2_lookup); }

int main() {
  arena = arena_alloc(KiB(100));

    String example = read_whole_file(&arena, "data/2024/04/example.txt");
    String input   = read_whole_file(&arena, "data/2024/04/input.txt");

    run(part1, example, 18);
    run(part1, input, 2591);

    run(part2, example, 9);
    run(part2, input, 1880);

  arena_release(&arena);

  return 0;
}
