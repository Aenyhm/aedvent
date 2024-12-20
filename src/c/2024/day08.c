#include "common/ascii.h"
#include "common/result.h"
#include "common/vector.h"

Arena arena;

Array_Template(V2, Array_V2);
Table_Template(u8, Array_V2, Table_Frequency);

#define ANTINODE_CHAR '#'

static s8 try_create_antinode(Ascii_Grid * grid, V2 position) {
  s8 result;

  int cell_index = get_cell_index(*grid, position.x, position.y);
  if (cell_index != -1) {
    u8 c1 = grid->chars[cell_index];
    if (c1 != ANTINODE_CHAR) {
      result = 1;
      grid->chars[cell_index] = ANTINODE_CHAR;
    } else {
      result = 0;
    }
  } else {
    result = -1;
  }

  return result;
}

static s64 compute_antinodes(Ascii_Grid * grid, Table_Frequency freqs, size_t min_dist, size_t max_dist) {
  s64 result = 0;

  for (size_t i = 0; i < freqs.count; ++i) {
    for (size_t j = 0; j < freqs.values[i].count; ++j) {
      V2 pos1 = freqs.values[i].items[j];

      for (size_t k = j; k < freqs.values[i].count; ++k) {
        if (j == k) continue;

        V2 pos2 = freqs.values[i].items[k];

        V2 base_distance = {pos2.x - pos1.x, pos2.y - pos1.y};

        for (size_t dist_factor = min_dist; dist_factor <= max_dist; dist_factor++) {
          V2 distance = v2_scale(base_distance, dist_factor);
          V2 antinode1 = v2_sub(pos1, distance);
          V2 antinode2 = v2_add(pos2, distance);

          s8 result1 = try_create_antinode(grid, antinode1);
          if (result1 == 1) result++;

          s8 result2 = try_create_antinode(grid, antinode2);
          if (result2 == 1) result++;

          if (result1 == -1 && result2 == -1) break;
        }
      }
    }
  }

  return result;
}

static s64 process(String s, int min_dist, int max_dist) {
  Ascii_Grid grid = string_to_grid(&arena, s);

  if (max_dist == -1) max_dist = grid.height;

  Table_Frequency freqs = {0};
  for (int col = 0; col < grid.height; ++col) {
    for (int row = 0; row < grid.width; ++row) {
      char c = find_char_at(grid, row, col);
      if (c != '.') {
        V2 pos = {row, col};
        Array_V2 * existing_positions;
        table_get(freqs, c, existing_positions);
        if (!existing_positions) {
          Array_V2 positions = {0};
          array_append(&positions, pos);
          table_append(&freqs, c, positions);
        } else {
          array_append(existing_positions, pos);
        }
      }
    }
  }

  return compute_antinodes(&grid, freqs, min_dist, max_dist);
}

static s64 part1(String s) { return process(s, 1,  1); }
static s64 part2(String s) { return process(s, 0, -1); }

int main() {
  arena = arena_alloc(KiB(10));

    String example  = read_whole_file(&arena, "data/2024/08/example.txt");
    String input    = read_whole_file(&arena, "data/2024/08/input.txt");

    run(part1, example, 14, input, 413);
    run(part2, example, 34, input, 1417);

  arena_release(&arena);

  return 0;
}
