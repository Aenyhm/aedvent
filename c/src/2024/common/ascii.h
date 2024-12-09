#pragma once
#include "../../aelibc/all.h"

typedef struct {
  u8 * chars;
  int width;
  int height;
} Ascii_Grid;

Ascii_Grid parse_grid(Arena * arena, String s) {
  Ascii_Grid grid = {0};
  grid.chars = arena_push_array(arena, u8, s.count);

  size_t chars_index = 0;
  for (size_t i = 0; i < s.count; ++i) {
    u8 c = s.data[i];
    if (c == '\n') {
      if (grid.height == 0) grid.width = i;
      grid.height++;
    } else {
      grid.chars[chars_index++] = c;
    }
  }

  return grid;
}

static bool is_cell_in_grid(Ascii_Grid grid, int row, int col) {
  return row >= 0 && row < grid.width && col >= 0 && col < grid.height;
}

static int get_cell_index(Ascii_Grid grid, int row, int col) {
  return is_cell_in_grid(grid, row, col) ? col*grid.width + row : -1;
}

static u8 find_char_at(Ascii_Grid grid, int row, int col) {
  u8 result = 0;

  int cell_index = get_cell_index(grid, row, col);
  if (cell_index != -1) {
    result = grid.chars[cell_index];
  }

  return result;
}
