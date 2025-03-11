/*
 * @file ringbuffer.c
 * @brief リングバッファ
 * @author Takeo Kikuchi
 */


#include "ringbuffer.h"

/**
 * @brief Initializes the ring buffer.
 * @param[in] rb Pointer to the ring buffer.
 */
 void RingBuffer_Init(RingBuffer *rb) {
     rb->head = 0;
     rb->tail = 0;
 }
 
 /**
  * @brief Puts a byte into the ring buffer.
  * @param[in] rb Pointer to the ring buffer.
  * @param[in] data The byte to put into the buffer.
  * @return true if the byte was put into the buffer, false if the buffer is full.
  */
 bool RingBuffer_Put(RingBuffer *rb, uint8_t data) {
     uint16_t next = (rb->head + 1) % RING_BUFFER_SIZE;
     if (next == rb->tail) {
         // バッファが満杯
         return false;
     }
     rb->buffer[rb->head] = data;
     rb->head = next;
     return true;
 }
 
 /**
  * @brief Gets a byte from the ring buffer.
  * @param[in] rb Pointer to the ring buffer.
  * @param[out] data Pointer to the variable that will receive the byte.
  * @return true if a byte was retrieved, false if the buffer is empty.
  */
 bool RingBuffer_Get(RingBuffer *rb, uint8_t *data) {
     if (rb->head == rb->tail) {
         // バッファが空
         return false;
     }
     *data = rb->buffer[rb->tail];
     rb->tail = (rb->tail + 1) % RING_BUFFER_SIZE;
     return true;
 }

/**
 * @brief Checks if the ring buffer is empty.
 * @param[in] rb Pointer to the ring buffer.
 * @return true if the buffer is empty, false otherwise.
 */
bool RingBuffer_IsEmpty(RingBuffer *rb) {
    return rb->head == rb->tail;
}

/**
 * @brief Gets the number of bytes in the ring buffer.
 * @param[in] rb Pointer to the ring buffer.
 * @return The number of bytes in the buffer.
 */
uint16_t RingBuffer_GetLength(RingBuffer *rb) {
    return (rb->head - rb->tail + RING_BUFFER_SIZE) % RING_BUFFER_SIZE;
}

