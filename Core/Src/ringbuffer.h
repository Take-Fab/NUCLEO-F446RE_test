/*
 * @file ringbuffer.h
 * @brief リングバッファ
 * @author Takeo Kikuchi
 */

#ifndef SRC_RINGBUFFER_H_
#define SRC_RINGBUFFER_H_

#include <stdbool.h>

#include "main.h"

#define RING_BUFFER_SIZE 256

typedef struct {
    uint8_t buffer[RING_BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
} RingBuffer;

void RingBuffer_Init(RingBuffer *rb);
bool RingBuffer_Put(RingBuffer *rb, uint8_t data);
bool RingBuffer_Get(RingBuffer *rb, uint8_t *data);
bool RingBuffer_IsEmpty(RingBuffer *rb);

#endif /* SRC_RINGBUFFER_H_ */
