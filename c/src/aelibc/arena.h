#pragma once
#include "log.h"

typedef struct {
  void * data;
  size_t count;
  size_t offset;
} Arena;

#define ALIGNMENT_OF(type) __alignof__(type)
#define arena_push(arena, count) arena_push_aligned((arena), (count), 1)
#define arena_push_array(arena, type, cap) \
  arena_push_aligned((arena), sizeof(type)*(cap), ALIGNMENT_OF(type))

#define arena_fmt "%zu/%zu (%zu %%)"
#define arena_args(used, count) (count) == 0 ? 0 : (used), (count), (used)*100/(count)

Arena arena_alloc(size_t count) {
  void * data = malloc(count);
  memset(data, 0, count);

  Arena arena = {0};
  arena.data  = data;
  arena.count = count;
  return arena;
}

void * arena_push_aligned(Arena * arena, size_t count, size_t alignment) {
  size_t misalignment = (uintptr_t)(arena->data + arena->offset) % alignment;
  size_t padding = misalignment > 0 ? (alignment - misalignment) : 0;

  if (arena->offset + padding + count > arena->count) {
    panic("Arena out of memory: %zu/%zu", arena->offset + padding + count, arena->count);
  }

  arena->offset += padding;

  void * data = (u8 *)arena->data + arena->offset;
  arena->offset += count;
  return data;
}

void arena_release(Arena * arena) {
  printf("[DEBUG] Arena release: " arena_fmt "\n", arena_args(arena->offset, arena->count));

  free(arena->data);
}
