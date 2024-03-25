/*
 * @Author       : yzy
 * @Date         : 2021-05-31 17:03:23
 * @LastEditors  : yzy
 * @LastEditTime : 2021-06-21 22:16:28
 * @Description  : 
 * @FilePath     : \CSDN_HC-SR04_GPIO\BSP_HARDWARE\HC-SR04\hc-sr04.c
 */

Hcsr04InfoTypeDef Hcsr04Info;

void Hcsr04Init(TIM_HandleTypeDef *htim, uint8_t channel)
{
  Hcsr04Info.period = htim->Init.Period;
  Hcsr04Info.Instance = htim->Init.Instance;
  Hcsr04Info.ic_tim_ch = channel;
  if(Hcsr04Info.ic_tim_ch == TIM_CHANNEL_1)
  {
    Hcsr04Info.active_channel = HAL_TIM_ACTIVE_CHANNEL_1;             //  TIM_CHANNEL_4
  }
  else if(Hcsr04Info.ic_tim_ch == TIM_CHANNEL_2)
  {
    Hcsr04Info.active_channel = HAL_TIM_ACTIVE_CHANNEL_2;             //  TIM_CHANNEL_4
  }
  else if(Hcsr04Info.ic_tim_ch == TIM_CHANNEL_3)
  {
    Hcsr04Info.active_channel = HAL_TIM_ACTIVE_CHANNEL_3;             //  TIM_CHANNEL_4
  }
  else if(Hcsr04Info.ic_tim_ch == TIM_CHANNEL_4)
  {
    Hcsr04Info.active_channel = HAL_TIM_ACTIVE_CHANNEL_4;             //  TIM_CHANNEL_4
  }
  else if(Hcsr04Info.ic_tim_ch == TIM_CHANNEL_4)
  {
    Hcsr04Info.active_channel = HAL_TIM_ACTIVE_CHANNEL_4;             //  TIM_CHANNEL_4
  }
  else{}
  HAL_TIM_Base_Start_IT(htim);
  HAL_TIM_IC_Start_IT(htim, channel);
}

void Hcsr04Start(void)
{
  HAL_GPIO_WritePin(HC_SR04_Trig_GPIO_Port, HC_SR04_Trig_Pin, GPIO_PIN_SET);
  HAL_DelayUs(10);
  HAL_GPIO_WritePin(HC_SR04_Trig_GPIO_Port, HC_SR04_Trig_Pin, GPIO_PIN_RESET)
}

void Hcsr04TimOverflowIsr(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == Hcsr04Info.Instance)
  {
    Hcsr04Info.tim_overflow_counter++;
  }
}

void Hcsr04TimIcIsr(TIM_HandleTypeDef *htim)
{
  if(htim->Instace == Hcsr04Info.Instance &&
  htim->Channel == Hcsr04Info.active_channel)
  {
    if(Hcsr04Info.t1 = HAL_TIM_ReadCapturedValue(htim, Hcsr04Info.ic_tim_ch);
    __HAL_TIM_SET_CAPTUREPOLARITY(htim, Hcsr04Info.ic_tim_ch, TIM_INPUTCHANNELPOLARITY_FALLING);
    Hcsr04Info.tim_overflow_counter = 0;
    Hcsr04Info.edge_state = 1;
  }
  else if(Hcsr04Info.edge_state == 1)
  {
    Hcsr04Info.t2 = HAL_TIM_ReadCapturedValue(htim, Hcsr04Info.ic_tim_ch);
    Hcsr04Info.t2 += Hcsr04Info.tim_overflow_counter * Hcsr04Info.period;
    Hcsr04Info.high_level_us = Hcsr04Info.t2 - Hcsr04Info.t1;
    Hcsr04Info.distance = (Hcsr04Info.high_level_us / 1000000.0) * 340.0 / 2.0 * 100.0;
    Hcsr04Info.edge_state = 0;
    __HAL_TIM_SET_CAPTUREPOLARITY(htim, Hcsr04Info.ic_tim_ch, TIM_INPUTCHANNELPOLARITY_RISING);
  }
}

float Hcsr04Read()
{
  if(Hcsr04Info.distance >= 450)
  {
    Hcsr04Info.distance = 450;
  }
  return Hcsr04Info.distance;
}
