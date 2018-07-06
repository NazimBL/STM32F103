
unsigned rec;
void main() {

  UART1_Init(115200);
  GPIO_Digital_Output(&GPIOC_BASE, _GPIO_PINMASK_13);
  Delay_ms(100);


  while(1) {
    if (UART1_Data_Ready()){
    rec = UART1_Read();
    GPIOC_ODR=0xFFFF;
    }else GPIOC_ODR=0x0000;

    UART1_Write_Text("GOT IT\n");
    Delay_ms(100);
  }
}