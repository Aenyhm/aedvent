#pragma once

#define Table_Template(key_type, value_type, name) \
  typedef struct {   \
    key_type * keys; \
    value_type * values; \
    size_t count;    \
    size_t capacity; \
  } name

#define table_append(table, key, value) \
  do { \
    if ((table)->count >= (table)->capacity) { \
      if ((table)->capacity == 0) { \
        (table)->capacity = ARRAY_INIT_CAP; \
        (table)->keys = malloc((table)->capacity*sizeof(*(table)->keys)); \
        (table)->values = malloc((table)->capacity*sizeof(*(table)->values)); \
      } else { \
        (table)->capacity = (table)->capacity*2; \
        (table)->keys = realloc((table)->keys, (table)->capacity*sizeof(*(table)->keys)); \
        (table)->values = realloc((table)->values, (table)->capacity*sizeof(*(table)->values)); \
        if ((table)->capacity > (1<<3)*ARRAY_INIT_CAP) { \
          printf("[DEBUG] Realloc table: new max items = %zu\n", (table)->capacity); \
        } \
      } \
      if ((table)->keys == NULL || (table)->values == NULL) { \
        panic("Insufficient memory: table capacity = %zu\n", (table)->capacity); \
      } \
    } \
    (table)->keys[(table)->count] = (key); \
    (table)->values[(table)->count] = (value); \
    (table)->count++; \
  } while (0)

#define table_get(table, key, value) \
  (value) = NULL; \
  for (size_t counter_ = 0; counter_ < (table).count; ++counter_) { \
    if ((table).keys[counter_] == (key)) { \
      (value) = (table).values + counter_; \
      break; \
    } \
  }
