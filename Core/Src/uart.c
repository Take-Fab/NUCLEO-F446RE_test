/*
 * uart.c
 *
 *  Created on: Mar 9, 2025
 *      Author: goota
 */

#include "uart.h"


static UART_HandleTypeDef huart_id;

/**
 * @brief  UART送信
 * @param[in] pData 送信データ
 * @param[in] Size 送信データサイズ
 */
int16_t uart_transmit(uint8_t *pData, uint16_t Size)
{
  HAL_StatusTypeDef status;

  status = HAL_UART_Transmit(&huart_id, pData, Size, HAL_MAX_DELAY);

  return (int16_t)status;
}

/**
 * @brief  UART受信
 * @param[out] pData 受信データ
 * @param[in] Size 受信データサイズ
 * @attention pData は呼び出し側で十分なサイズを確保すること
 * @note  受信データがSizeに満たない場合は、HAL_MAX_DELAYで待機する
 */
int16_t uart_receive(uint8_t *pData, uint16_t Size)
{
  HAL_StatusTypeDef status;

  status = HAL_UART_Receive(&huart_id, pData, Size, HAL_MAX_DELAY);

  return (int16_t)status;
}

/**
 * @brief  UART送信
 * @param[in] pData 送信データ
 * @param[in] Size 送信データサイズ
 * @note  送信完了割り込みで送信完了を検知する
 */
int16_t uart_transmit_nonblocking(uint8_t *pData, uint16_t Size)
{
  HAL_StatusTypeDef status;

  status = HAL_UART_Transmit_IT(&huart_id, pData, Size);

  return (int16_t)status;
}

/**
 * @brief  UART送信完了割り込み
 * @param huart UARTハンドル
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  static uint16_t count2;
  static uint16_t count3;

  if (huart->Instance == USART2)
  {
      ++count2;
  } else if (huart->Instance == USART3) {
      ++count3;
  }
}

#define RX_BUFFER_SIZE 1
static uint8_t rxBuffer[RX_BUFFER_SIZE];


int16_t uart_receive_nonblocking(uint8_t *pData, uint16_t Size)
{
  HAL_StatusTypeDef status;

  status = HAL_UART_Receive_IT(&huart_id, pData, Size);

  return (int16_t)status;
}


static bool received = false;

bool uart_is_received()
{
  if (received) {
      received = false;
      return true;
  }

  return received;
}

/**
 * @brief  UART受信完了割り込み
 * @param huart UARTハンドル
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  static uint16_t count2;
  static uint16_t count3;

  if (huart->Instance == USART2)
  {
      ++count2;

      received = true;
    } else if (huart->Instance == USART3) {
      ++count3;

      received = true;
    }
}



/**
 * @brief  UART受信
 * @param[out] pData 受信データ
 * @param[in] Size 受信データサイズ
 * @note  受信完了割り込みで受信完了を検知する
 */
int16_t uart_receive_nonblocking_start()
{
  HAL_StatusTypeDef status;

  status = HAL_UART_Receive_IT(&huart_id, rxBuffer, RX_BUFFER_SIZE);

  return (uint16_t)status;
}

/**
 * @brief  UARTの初期化
 */
int16_t uart_init(UART_HandleTypeDef huart)
{
    huart_id = huart;

    return 0;
}



static uint16_t rxLength;

uint16_t get_receive_data(uint8_t *p_buff, uint16_t size)
{
  uint16_t rcv_size = rxLength;

  if (rcv_size > size) {
      rcv_size = size;
  }

  for (uint16_t u = 0; u < rcv_size; ++u) {
      *(p_buff + u) = rxBuffer[u];
  }

  return rcv_size;
}

/**
 * @brief  UART受信完了割り込み
 * @param huart UARTハンドル
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if (huart->Instance == USART2)
  {
    rxLength = Size;

    // 受信データの処理
    HAL_UARTEx_ReceiveToIdle_IT(&huart_id, rxBuffer,  RX_BUFFER_SIZE);
  }
}




