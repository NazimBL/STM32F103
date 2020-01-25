#define CR  0x0D

char tag=1,tmp='a';
char rx_buff[64];
char data_ready=0;
char data_len;


void UART1_Interrupt() iv IVT_INT_USART1 ics ICS_AUTO {
       
   if(USART1_SRbits.RXNE==1){
      tmp = UART1_Read();
      USART1_SRbits.RXNE=0;
   }
       
   switch(tmp) {
     case '\n':
        if(data_len > 1)
        {
          data_ready = 1;
        }
        break;
     case CR:
        rx_buff[data_len] = 0; 
        break;
     default:
        rx_buff[data_len] = tmp;
        data_len++;
   }   
}

void main() {

     UART1_Init_Advanced(115200, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART1_PA9_10);
     NVIC_IntEnable(IVT_INT_USART1);   
     USART1_CR1bits.RXNEIE=1;
     USART1_CR1bits.PEIE=1;
     
     while(1){
     
     if(data_ready==1){
       UART1_Write_Text(rx_buff);
       data_ready=0;
       data_len=0;
       }
     }         
}
