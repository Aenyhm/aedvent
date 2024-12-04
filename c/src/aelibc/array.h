#pragma once

#define Array_Template(type, name) \
  typedef struct {   \
    type * items;    \
    size_t count;    \
    size_t capacity; \
    bool   ordered;  \
  } name

// Unordered arrays by default: to remove efficiently an item.
#define array_init(arena, array, cap) \
  (array).items = arena_push_array((arena), *(array).items, cap); \
  (array).count = 0; \
  (array).capacity = cap; \
  (array).ordered = false;

#define array_append(array, item) \
  do { \
    if ((array)->count + 1 > (array)->capacity) { \
      panic("Cannot append an item: array capacity = %zu\n", (array)->capacity); \
    } \
    (array)->items[(array)->count++] = (item); \
  } while (0)

// Named `counter_` to not interfere with outer code.
#define array_remove_at_index(array, index) \
  do { \
    if ((index) >= (array)->capacity) { \
      panic("Cannot remove index `%zu`: array capacity = %zu\n", (index), (array)->capacity); \
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
