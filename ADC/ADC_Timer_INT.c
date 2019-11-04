char sync=0;
void InitTimer2();

//Sampling interupt
void Timer2_interrupt() iv IVT_INT_TIM2 {

  TIM2_SR.UIF = 0;
  sync=1;
}

void main()
{  
  ADC_Set_Input_Channel(_ADC_CHANNEL_0);
  ADC_Set_Input_Channel(_ADC_CHANNEL_1);
  ADC1_Init();
  InitTimer2();

  while(1)
  {
   
    if(sync== 1)
    {
       ADC1_Get_Sample(0);
       ADC1_Get_Sample(1);
       sync=0;		
    }
}

//sampling rate settings
void InitTimer2(){
  RCC_APB1ENR.TIM2EN = 1;
  TIM2_CR1.CEN = 0;
  //0 and 17999
  TIM2_PSC = 0;
  //45
  TIM2_ARR = 44999;
  //TIM2_ARR = 35999;
  NVIC_IntEnable(IVT_INT_TIM2);
  TIM2_DIER.UIE = 1;
  TIM2_CR1.CEN = 1;
}