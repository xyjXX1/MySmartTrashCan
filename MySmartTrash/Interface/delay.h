#ifndef __DELAY_H
#define __DELAY_H

#ifdef  __cplusplus
extern "C"{
#endif

#include "stdint.h"
#include "tim.h"
#include "stm32f4xx_hal.h"

void HAL_DelayUs(uint32_t us)
{
	__HAL_TIM_SetCounter(&htim6, 0);
	__HAL_TIM_ENABLE(&htim6);
	while(__HAL_TIM_GetCounter(&htim6) < us);
	__HAL_TIM_DISABLE(&htim6);
}

#ifdef  __cplusplus
}
#endif

#endif 
