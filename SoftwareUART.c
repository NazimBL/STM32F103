/*
* NazimBL
* mikroC PRO for ARM
*/
//stm32f103, 72 Mhz,external quartz +PLLx9

char error;

void main(){

	error = Soft_UART_Init(&GPIOB_ODR, 11, 10,19200, 1);

	while(1){
		
		Soft_UART_Write('D');
		Delay_ms(500);
	}

}