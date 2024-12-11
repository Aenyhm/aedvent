#include <pthread.h>
#include "common/result.h"

typedef struct {
  Array_s64 left;
  Array_s64 right;
} Columns;

static void * sort_array(void * arg) {
  Array_s64 * column = (Array_s64 *)arg;
  qsort(column->items, column->count, sizeof(*column->items), compare);
  pthread_exit(NULL);
}

static Columns parse_columns(String s) {
  Columns columns = {0};

  Array_String lines = str_split(s, "\n");

  for (size_t i = 0; i < lines.count; ++i) {
    String line = lines.items[i];
    String lhs = str_chop_by_delim(&line, "   ");
    String rhs = line;
    array_append(&columns.left, str_to_s64(lhs));
    array_append(&columns.right, str_to_s64(rhs));
  }

  pthread_t thread1;
  pthread_t thread2;
  pthread_create(&thread1, NULL, sort_array, &columns.left);
  pthread_create(&thread2, NULL, sort_array, &columns.right);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  return columns;
}

static s64 find_total_distance(Columns columns) {
  s64 result = 0;
  for (size_t i = 0; i < columns.left.count; ++i) {
    s64 distance = llabs(columns.left.items[i] - columns.right.items[i]);
    result += distance;
  }
  return result;
}

static s64 find_similarity_score(Columns columns) {
  s64 result = 0;

  u8 left_occurences = 1;
  size_t right_index = 0;
  for (size_t left_index = 0; left_index < columns.left.count; left_index++) {
    s64 lhs = columns.left.items[left_index];
    if (lhs == columns.left.items[left_index + 1]) {
      left_occurences++;
    } else {
      u8 right_occurences = 0;
      while (right_index < columns.right.count) {
        s64 rhs = columns.right.items[right_index];
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

static s64 part1(String s) { return find_total_distance(parse_columns(s)); }
static s64 part2(String s) { return find_similarity_score(parse_columns(s)); }

int main() {
  Arena arena = arena_alloc(KiB(20));

    String example = read_whole_file(&arena, "data/2024/01/example.txt");
    String input   = read_whole_file(&arena, "data/2024/01/input.txt");

    run(part1, example, 11, input, 1506483);
    run(part2, example, 31, input, 23126924);

  arena_release(&arena);

  return 0;
}
