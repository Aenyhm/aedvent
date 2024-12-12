#pragma once

#define Array_Template(type, name) \
  typedef struct {   \
    type * items;    \
    size_t count;    \
    size_t capacity; \
    bool   ordered;  \
  } name

#define ARRAY_INIT_CAP 256

#define array_append(array, item) \
  do { \
    if ((array)->count >= (array)->capacity) { \
      if ((array)->capacity == 0) { \
        (array)->capacity = ARRAY_INIT_CAP; \
        (array)->items = malloc((array)->capacity*sizeof(*(array)->items)); \
      } else { \
        (array)->capacity = (array)->capacity*2; \
        (array)->items = realloc((array)->items, (array)->capacity*sizeof(*(array)->items)); \
        if ((array)->capacity > (1<<3)*ARRAY_INIT_CAP) { \
          printf("[DEBUG] Realloc array: new max items = %zu\n", (array)->capacity); \
        } \
      } \
      if ((array)->items == NULL) { \
        panic("Insufficient memory: array capacity = %zu\n", (array)->capacity); \
      } \
    } \
    (array)->items[(array)->count++] = (item); \
  } while (0)

// Named `counter_` to not interfere with outer code.
#define array_remove_at_index(array, index) \
  do { \
    if ((index) >= (array)->count) { \
      panic("Cannot remove index `%zu`: array count = %zu\n", (index), (array)->count); \
    } \
    if ((array)->ordered) { \
      for (size_t counter_ = (index); counter_ < (array)->count - 1; ++counter_) { \
        (array)->items[counter_] = (array)->items[counter_ + 1]; \
      } \
    } else { \
      (array)->items[(index)] = (array)->items[(array)->count - 1]; \
    } \
    --(array)->count; \
  } while (0)

#define array_map(input_array, fn, output_array) \
  for (size_t counter_ = 0; counter_ < (input_array).count; ++counter_) { \
    array_append((output_array), (fn)((input_array).items[counter_])); \
  }

#define array_contains(array, item, result) \
  (result) = false; \
  for (size_t counter_ = 0; counter_ < (array).count; ++counter_) { \
    if ((array).items[counter_] == (item)) { \
      (result) = true; \
      break; \
    } \
  }
