unsigned int startTime,endTime;
char txt[12];
void TimerSetup();

void main(){
  
  UART1_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART1_PA9_10);
  TimerSetup();
  
  startTime=TIM6_CNT;
  delay_ms(200);
  endTime=(int)TIM3_CNT-startTime;
  
  IntToStr(endTime, txt);
  UART1_Write_Text("Time in millis: ");
  UART1_Write_Text(txt);
  UART1_Write_Text("\n\r");
  
  while(1);

}

void TimerSetup(){

  //counter clock is 1Khz (1ms)
  RCC_APB1ENR.TIM3EN = 1;       // Enable clock gating for timer module 2
   TIM3_CR1.CEN = 0;             // Disable timer
  //1khz  when System clokc is 8 Mhz
   TIM3_PSC = 7999;              // Set timer prescaler.
   TIM3_ARR = 65535;
   TIM3_CR1.CEN = 1;             // Enable timer
}
