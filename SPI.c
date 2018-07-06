
char val=0;
long buff;


void main() {

      // SPI1_Init();
      GPIO_Digital_Output(&GPIOA_BASE, _GPIO_PINMASK_15);
      GPIO_Digital_Output(&GPIOB_BASE, _GPIO_PINMASK_15);
      GPIO_Digital_Output(&GPIOC_BASE, _GPIO_PINMASK_13);
      GPIOA_ODR=0x0000;
      GPIOB_ODRbits.ODR15=1;
      SPI1_Init_Advanced(_SPI_FPCLK_DIV2
                         ,_SPI_MASTER | _SPI_8_BIT | _SPI_CLK_IDLE_HIGH | _SPI_SECOND_CLK_EDGE_TRANSITION | _SPI_MSB_FIRST | _SPI_SS_ENABLE | _SPI_SSM_ENABLE | _SPI_SSI_1
                         ,&_GPIO_MODULE_SPI1_PA567);
      // UART1_Init(9600);

       Delay_ms(100);

       while(1){

       GPIOC_ODR=0x0000;
       GPIOB_ODRbits.ODR15=1;
      // val = SPI_Read(buff);
       //UART1_Write(val);


       }
}