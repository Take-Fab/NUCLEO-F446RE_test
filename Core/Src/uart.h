/*
 * @file uart.h
 * @author Takeo Kikuchi
 */

#ifndef SRC_UART_H_
#define SRC_UART_H_

#include "main.h"
#include "stdbool.h"

int16_t uart_init(UART_HandleTypeDef *huart);
int16_t uart_transmit(uint8_t *pData, uint16_t Size);
int16_t uart_receive(uint8_t *pData, uint16_t Size);

int16_t uart_transmit_nonblocking(uint8_t *pData, uint16_t Size);

int16_t uart_receive_start();
bool uart_is_received();
int16_t uart_receive_nonblocking(uint8_t *pData, uint16_t Size);

int16_t uart_receive_nonblocking_start();
uint16_t uart_get_receive_data(uint8_t *p_buff, uint16_t size);

#endif /* SRC_UART_H_ */
