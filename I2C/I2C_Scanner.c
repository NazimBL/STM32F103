char address;
char err;
char txt[4];
char I2C_check_device(unsigned short dev_address);
void I2C_Setup();

void main() {

      UART1_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART1_PA9_10);
      Delay_ms(200);
      
      while(1){
      
      UART1_Write_Text("Sacnning ... \n\r");  
      for(address=1; address <127; address++){
      
      I2C1_Start();
      err=I2C1_Write(address,0XFF,1,END_MODE_STOP);
      
      //use Conversion library to display bytes as readable strings
      ByteToStr(err, txt);
      
      if(err==0){
      UART1_Write_Text("I2C Device found at address: ");
      UART1_Write_Text(txt);
      UART1_Write_Text("\n\r");
      }
      // you can check for other error codes here
      }
    
      Delay_ms(3000); 
        
      }
}

void I2C_Setup(){

 RCC_APB1ENRbits.I2C1EN=1;
 AFIO_MAPRbits.I2C1_REMAP=0;
 //Master 100khz
 I2C1_Init_Advanced(100000, &_GPIO_MODULE_I2C1_PB67);
}

// checks if slave with specific address is attached
char I2C_check_device(unsigned short dev_address){

 I2C1_Start();
 return (I2C1_Write(dev_address,0XFF,1,END_MODE_STOP));  
 }  
