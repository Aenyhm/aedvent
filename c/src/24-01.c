#include <pthread.h>

#include "common.h"

Array_Template(u32, Array_u32);

typedef struct {
  Array_u32 left;
  Array_u32 right;
} Columns;

static void * sort_array(void * arg) {
  Array_u32 * column = (Array_u32 *)arg;
  qsort(column->items, column->count, sizeof(*column->items), compare);
  pthread_exit(NULL);
}

static Columns file_to_sorted_columns(Arena * arena, const char * file_path) {
  Columns columns;

  Lines lines = parse_file(arena, file_path);

  array_init(arena, columns.left, lines.count);
  array_init(arena, columns.right, lines.count);

  for (size_t i = 0; i < lines.count; ++i) {
    String line = lines.items[i];
    String lhs = str_chop_by_delim(&line, "   ");
    String rhs = line;
    array_append(&columns.left, atoi(str_to_cstr(arena, lhs)));
    array_append(&columns.right, atoi(str_to_cstr(arena, rhs)));
  }

  pthread_t thread1;
  pthread_t thread2;
  pthread_create(&thread1, NULL, sort_array, &columns.left);
  pthread_create(&thread2, NULL, sort_array, &columns.right);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  return columns;
}

static u32 find_total_distance(Columns columns) {
  u32 result = 0;
  for (size_t i = 0; i < columns.left.count; ++i) {
    u32 distance = labs((s32)(columns.left.items[i] - columns.right.items[i]));
    result += distance;
  }
  return result;
}

static u32 find_similarity_score(Columns columns) {
  u32 result = 0;

  u8 left_occurences = 1;
  size_t right_index = 0;
  for (size_t left_index = 0; left_index < columns.left.count; left_index++) {
    u32 lhs = columns.left.items[left_index];
    if (lhs == columns.left.items[left_index + 1]) {
      left_occurences++;
    } else {
      u8 right_occurences = 0;
      while (right_index < columns.right.count) {
        u32 rhs = columns.right.items[right_index];
        if (rhs == lhs) right_occurences++;
        if (rhs <= lhs) {
          right_index++;
        } else {
          break;
        }
      }
      result += lhs*left_occurences*right_occurences;
      left_occurences = 1;
    }
  }

  return result;
}

int main() {
  Arena arena = arena_alloc(KiB(200));

    Columns example_columns = file_to_sorted_columns(&arena, "data/2024/01/example.txt");
    Columns input_columns   = file_to_sorted_columns(&arena, "data/2024/01/input.txt");

    assert(find_total_distance(example_columns) == 11);
    assert(find_total_distance(input_columns) == 1506483);
    assert(find_similarity_score(example_columns) == 31);
    assert(find_similarity_score(input_columns) == 23126924);

  arena_release(&arena);

  return 0;
}
