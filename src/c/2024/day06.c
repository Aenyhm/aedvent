#include "common/ascii.h"
#include "common/result.h"
#include "common/vector.h"

Arena arena;

typedef struct {
  V2 position;
  V2 direction_vector;
  int rotation_count;
} Guard;

static Guard find_guard(Ascii_Grid grid) {
  Guard guard = {0};

  for (int col = 0; col < grid.height; ++col) {
    for (int row = 0; row < grid.width; ++row) {
      u8 c = find_char_at(grid, row, col);
      if (c != '.' && c != '#') {
        guard.position = (V2){row, col};
        switch (c) {
          case '<': guard.direction_vector.x = -1; break;
          case '>': guard.direction_vector.x = +1; break;
          case '^': guard.direction_vector.y = -1; break;
          case 'v': guard.direction_vector.y = +1; break;
          default: UNREACHABLE;
        }
        goto end;
      }
    }
  }

  end:
  return guard;
}

static s64 process(String s) {
  int result = 0;

  Ascii_Grid grid = string_to_grid(&arena, s);

  Guard guard = find_guard(grid);
  grid.chars[get_cell_index(grid, guard.position.x, guard.position.y)] = 'X';
  result++;

  while (true) {
    V2 next_position = v2_add(guard.position, guard.direction_vector);
    int cell_index = get_cell_index(grid, next_position.x, next_position.y);
    if (cell_index == -1) break;

    u8 c = grid.chars[cell_index];
    if (c == '#') {
      guard.direction_vector = v2_rotate(guard.direction_vector, true);
      guard.rotation_count++;
    } else {
      guard.position = next_position;
      if (c != 'X') result++;
      grid.chars[cell_index] = 'X';
    }
  }

  return result;
}

static s64 part1(String s) { return process(s); }

int main() {
  arena = arena_alloc(KiB(100));

    String example = read_whole_file(&arena, "data/2024/06/example.txt");
    String input   = read_whole_file(&arena, "data/2024/06/input.txt");

    run(part1, example, 41, input, 5101);
    // TODO: part 2

  arena_release(&arena);

  return 0;
}
