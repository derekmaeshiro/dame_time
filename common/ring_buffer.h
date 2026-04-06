#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t *buffer;
    uint32_t head;
    uint32_t tail;
    uint32_t size; 
    volatile uint32_t count; // interrupt safety --> use volatile
    rb_mode_e mode;
} ring_buffer_s;

typedef enum {
    RB_BLOCK_IF_FULL,
    RB_OVERRIDE_IF_FULL,
} rb_mode_e;

// methods
void rb_init(ring_buffer_s *rb, uint8_t *mem, uint32_t size, rb_mode_e mode);
bool rb_push(ring_buffer_s *rb, uint8_t data);
bool rb_pop(ring_buffer_s *rb, uint8_t *data);

#endif