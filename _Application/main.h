#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "stdio.h"
void Error_Handler(void);
void assert_failed(uint8_t *file,uint32_t line);
#ifdef __cplusplus
}
#endif

#endif
