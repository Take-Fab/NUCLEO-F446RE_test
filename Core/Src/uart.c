/*
 * @file uart.c
 * @brief UART通信
 * @author Takeo Kikuchi
 */

#include "uart.h"
#include "ringbuffer.h"

static UART_HandleTypeDef *huart_id;

static RingBuffer rx_rBuffer;
static RingBuffer tx_rBuffer;

//! @note このバージョンでは、受信データは1バイトごとに処理を行う
#define RX_TMP_BUFFER_SIZE 1
static uint8_t rx_tmp_buff[RX_TMP_BUFFER_SIZE];

/**
 * @brief  UARTの初期化。RingBufferの初期化も行う。
 * *param[in] huart UARTハンドル
 */
int16_t uart_init(UART_HandleTypeDef *huart)
{
  huart_id = huart;

  RingBuffer_Init(&rx_rBuffer);
  RingBuffer_Init(&tx_rBuffer);

  return 0;
}


/**
 * @brief  UART送信
 * @param[in] pData 送信データ
 * @param[in] Size 送信データサイズ
 * @warning 送信完了まで待機する
 */
int16_t uart_transmit(uint8_t *pData, uint16_t Size)
{
  HAL_StatusTypeDef status;

  status = HAL_UART_Transmit(huart_id, pData, Size, HAL_MAX_DELAY);

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

  status = HAL_UART_Receive(huart_id, pData, Size, HAL_MAX_DELAY);

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

  status = HAL_UART_Transmit_IT(huart_id, pData, Size);

  return (int16_t)status;
}

/**
 * @brief  UART送信完了割り込み
 * @param huart UARTハンドル
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  static uint16_t count2;  //! @note デバッグ用
  static uint16_t count3;  //! @note デバッグ用

  if (huart->Instance == USART2)
  {
      ++count2;
  } else if (huart->Instance == USART3) {
      ++count3;
  }
}

#if 0
/**
 * @brief  UART受信
 * @param[out] pData 受信データ
 * @param[in] Size 受信データサイズ
 * @note  受信完了割り込みで受信完了を検知する
 */
int16_t uart_receive_nonblocking(uint8_t *pData, uint16_t Size)
{
  HAL_StatusTypeDef status;

  status = HAL_UART_Receive_IT(huart_id, pData, Size);

  return (int16_t)status;
}
#endif

/**
 * @brief  UART受信開始
 * @note  受信完了割り込みで受信完了を検知する
 */
int16_t uart_receive_start()
{
  HAL_StatusTypeDef status;

  status = HAL_UART_Receive_IT(huart_id, rx_tmp_buff, RX_TMP_BUFFER_SIZE);

  return (int16_t)status;
}


static bool received = false;

/**
 * @brief  UART受信チェック
 * @retval 受信データあり:true、受信データなし:false
 */
bool uart_is_received()
{
  return !(RingBuffer_IsEmpty(&rx_rBuffer));

  #if 0
  if (received) {
      received = false;
      return true;
  }

  return received;
  #endif
}

/**
 * @brief  UART受信データの取得
 * @param[out] p_buff 受信データ
 * @param[in] size 最大受信データサイズ
 * @retval 取得したデータサイズ
 */
uint16_t uart_get_receive_data(uint8_t *p_buff, uint16_t size)
{
  uint16_t get_num;

  // 最大データサイズ、または、Ringbufferが尽きるまでからデータを取得
  for (get_num = 0; get_num < size; ++get_num) {
      if (!RingBuffer_Get(&rx_rBuffer, p_buff + get_num)) {
          break;
      }
  }

  return get_num;
}


/**
 * @brief UART受信完了割り込み
 * @brief 受信データを RingBuffer に格納する
 * @param huart UARTハンドル
 * @note UART2(Debug portと兼用)、UART3(通信専用)の受信完了割り込みを処理する
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
  else {
    return;
  }

  for (uint16_t u = 0; u < RX_TMP_BUFFER_SIZE; ++u) {
      RingBuffer_Put(&rx_rBuffer, rx_tmp_buff[u]);
  }

  uart_receive_start();  // 受信再開
}

#if 0
//! @todo 未実装
/**
 * @brief  UART受信
 * @param[out] pData 受信データ
 * @param[in] Size 受信データサイズ
 * @note  受信完了割り込みで受信完了を検知する
 */
int16_t uart_receive_nonblocking_start()
{
  HAL_StatusTypeDef status;

  status = HAL_UART_Receive_IT(huart_id, rxBuffer, RX_BUFFER_SIZE);

  return (uint16_t)status;
}


static uint16_t rxLength;

uint16_t uart_get_receive_data_ex(uint8_t *p_buff, uint16_t size)
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
#endif
