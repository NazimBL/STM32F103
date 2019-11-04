 unsigned adc;
 Timer3_Setup(); 
 
 
//Timer ISR
void Timer3_interrupt() iv IVT_INT_TIM3
{
  TIM3_SR.UIF = 0;
  GPIOC_ODRbits.ODR13=~GPIOC_ODRbits.ODR13=1;; //Toggle pin13 led's
  adc=ADC1_Read(0);
 } 
 
 void main(){
	 
	 GPIO_Digital_Output(&GPIOC_BASE, _GPIO_PINMASK_13);
	 ADC_Set_Input_Channel(_ADC_CHANNEL_0);
	 ADC1_Init();
	 Timer3_Setup();
	 
	 while(1);	 
 }
 
 void Timer3_Setup(){
  RCC_APB1ENR.TIM3EN = 1;       // Enable clock gating for timer module 2
  TIM3_CR1.CEN = 0;             // Disable timer
  // 1 hz == one sec
  TIM3_PSC = 1098;              // Set timer prescaler.
  TIM3_ARR = 65514;
  NVIC_IntEnable(IVT_INT_TIM3); // Enable timer interrupt
  TIM3_DIER.UIE = 1;            // Update interrupt enable
  TIM3_CR1.CEN = 1;             // Enable timer
 }