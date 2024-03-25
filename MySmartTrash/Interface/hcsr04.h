#ifndef 	__HCSR04_H
#define 	__HCSR04_H

#ifdef 		__cplusplus
extern "C"{
#endif

typedef struct hcsr04
{
	uint8_t  	  	  	  edge_state;
	uint32_t 			  period;
	uint16_t			  tim_overflow_counter;
	uint32_t 			  t1;
	uint32_t 			  t2;
	uint32_t 			  high_level_us;
	TIM_TypeDef 		  *Instance;
	float 				  distance;
	uint32_t 			  ic_tim_ch;
	HAL_TIM_ActiveChannel active_channel;
}Hcsr04InfoTypeDef;


void Hcsr04Init(TIM_HandleTypeDef *htim, uint8_t channel);
void Hcsr04Start(void);
void Hcsr04TimOverflowIsr(TIM_HandleTypeDef *htim);
void Hcsr04TimIcIsr(TIM_HandleTypeDef *htim);
float Hcsr04Read();

#ifdef		__cplusplus
}
#endif

#endif
