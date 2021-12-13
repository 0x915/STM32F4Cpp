#include "main.h"

void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
  if(hi2c->Instance==I2C1){}
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  if(hi2c->Instance==I2C1)
  {
    __HAL_RCC_I2C1_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);
  }
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USART1){}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USART1)
  {
    __HAL_RCC_USART1_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);
  }
}


