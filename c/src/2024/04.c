#include "../aelibc/all.h"

#define WORD1 "XMAS"
#define WORD2 "MAS"

typedef struct {
  u8 * letters;
  u32 width;
  u32 height;
} Grid;

typedef u8 (*Lookup_Function)(Grid grid, int row, int col);

static Grid parse_grid(Arena * arena, String s) {
  Grid grid = {0};
  grid.letters = arena_push_array(arena, u8, s.count);

  size_t letter_index = 0;
  for (size_t i = 0; i < s.count; ++i) {
    u8 c = s.data[i];
    if (c == '\n') {
      if (grid.height == 0) grid.width = i;
      grid.height++;
    } else {
      grid.letters[letter_index++] = c;
    }
  }

  return grid;
}

static bool is_cell_in_grid(Grid grid, int row, int col) {
  return row >= 0 && (u32)row < grid.width && col >= 0 && (u32)col < grid.height;
}

static u8 find_letter_at(Grid grid, int row, int col) {
  u8 result = '.';

  if (is_cell_in_grid(grid, row, col)) {
    size_t index = col*grid.width + row;
    result = grid.letters[index];
  }

  return result;
}

static u8 part1_lookup(Grid grid, int row, int col) {
  u8 result = 0;

  // 8 directions
  for (s8 x = -1; x <= 1; ++x) {
    for (s8 y = -1; y <= 1; ++y) {
      if (x == 0 && y == 0) continue;

      bool success = true;
      for (u8 i = 0; i < strlen(WORD1); ++i) {
        int delta_x = row + x*i;
        int delta_y = col + y*i;
        if (find_letter_at(grid, delta_x, delta_y) != WORD1[i]) {
          success = false;
          break;
        }
      }

      if (success) result++;
    }
  }

  return result;
}

static u8 part2_lookup(Grid grid, int row, int col) {
  u8 result = 0;

  // start at center
  bool success = find_letter_at(grid, row, col) == WORD2[1];

  if (success) {
    u8 upper_left  = find_letter_at(grid, row - 1, col - 1);
    u8 upper_right = find_letter_at(grid, row + 1, col - 1);
    u8 lower_left  = find_letter_at(grid, row - 1, col + 1);
    u8 lower_right = find_letter_at(grid, row + 1, col + 1);

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

static u32 count_words(Grid grid, Lookup_Function lookup_fn) {
  u32 result = 0;

  for (size_t col = 0; col < grid.height; ++col) {
    for (size_t row = 0; row < grid.width; ++row) {
      result += lookup_fn(grid, row, col);
    }
  }

  return result;
}

int main() {
  Arena arena = arena_alloc(KiB(100));

    String example = read_whole_file(&arena, "data/2024/04/example.txt");
    String input   = read_whole_file(&arena, "data/2024/04/input.txt");

    Grid example_grid = parse_grid(&arena, example);
    Grid input_grid   = parse_grid(&arena, input);

    assert(count_words(example_grid, part1_lookup) == 18);
    assert(count_words(input_grid, part1_lookup) == 2591);

    assert(count_words(example_grid, part2_lookup) == 9);
    assert(count_words(input_grid, part2_lookup) == 1880);

  arena_release(&arena);

  return 0;
}
