#include "ring_buffer.h"

void rb_init(ring_buffer_s *rb, uint8_t *mem, uint32_t size, rb_mode_e mode) {
    rb->size = size;
    rb->count = 0;
    rb->buffer = mem;
    rb->head = 0;
    rb->tail = 0;
    rb->mode = mode;
}

bool rb_push(ring_buffer_s *rb, uint8_t data) {
    if (rb->mode == RB_BLOCK_IF_FULL) {
        if (rb->count < rb->size) {
            rb->buffer[rb->head] = data;
            __disable_irq();
            rb->count++;
            __enable_irq();
            rb->head = (rb->head + 1) % rb->size;
            return true;
        }
        return false;
    }
    else {
        rb->buffer[rb->head] = data;
        rb->head = (rb->head + 1) % rb->size;
        if (rb->head == rb->tail) {
            rb->tail = (rb->tail + 1) % rb->size;
        }
        if (rb->count < rb->size) {
            __disable_irq();
            rb->count++;
            __enable_irq();
        }
        return true;
    }
}

bool rb_pop(ring_buffer_s *rb, uint8_t *data) {
    if (rb->count == 0) {
        return false;
    }
    else {
        *data = rb->buffer[rb->tail];
        rb->tail = (rb->tail + 1) % rb->size;
        __disable_irq();
        rb->count--;
        __enable_irq();
        return true;
    }
}