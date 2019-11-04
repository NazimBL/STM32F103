 /*
 * Nazim BL
 * mikroC PRO for ARM
 */
 
 unsigned int PWM_T,d;
 
 void main(){
	 
	 GPIO_Digital_Output(&GPIOB_BASE, _GPIO_PINMASK_11);
	 
	 // 2 Khz
	 PWM_T=PWM_TIM2_Init(2000);
	 PWM_TIM2_Start(_PWM_CHANNEL4, &_GPIO_MODULE_TIM2_CH4_PB11);
     PWM_TIM2_Set_Duty(PWM_T/2, _PWM_NON_INVERTED, _PWM_CHANNEL4);
	 
	 while(1);	 
 }
 
 