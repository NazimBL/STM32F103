void SetupInterrupt();
char data_ready=0;

// ISR
void INTERRUPT() iv IVT_INT_EXTI1 ics ICS_AUTO {   
   EXTI_PR.B1 = 1;                     // clear flag
   data_ready=1;
  //Do something else:
}

void main() {

         UART1_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART1_PA9_10);
         delay_ms(100);
         SetupInterrupt();
         
         while(1){
          //message get displayed each time the interrupt is trigered
          if(data_ready==1){
          UART1_Write_Text("Interrupt Event\n\r");
          data_ready=0;
          }       
          }
}

void SetupInterrupt(){
    //Initialize Interrupt Pin (PB.1):
    GPIO_Digital_Input(&GPIOB_BASE, _GPIO_PINMASK_1);
    RCC_APB2ENRbits.AFIOEN=1;            // Enable clock for alternate pin function
    AFIO_EXTICR1bits.EXTI1 = 1;          // PB2 as External interrupt
    EXTI_FTSR = 0x00000002;              // Set interrupt on Falling edge
    EXTI_IMR |= 0x00000002;              // Set mask
    NVIC_IntEnable(IVT_INT_EXTI1);       // Enable External interrupt
    EnableInterrupts();                  // Enables the processor interrupt.
}
}
