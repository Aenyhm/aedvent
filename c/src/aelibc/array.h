#pragma once

#define Array_Template(type, name) \
  typedef struct {   \
    type * items;    \
    size_t count;    \
    size_t capacity; \
  } name

#define array_init(arena, array, cap) \
  (array).items = arena_push_array((arena), *(array).items, cap); \
  (array).count = 0; \
  (array).capacity = cap;

#define array_append(array, item) \
  do { \
    if ((array)->count + 1 > (array)->capacity) { \
      panic("Cannot append an item: array capacity = %zu\n", (array)->capacity); \
    } \
    (array)->items[(array)->count++] = (item); \
  } while (0)
