char tx_buff[4]={'b','a','n','g'};
char rx_buff[64];
char msg[64];

char data_len=0,tmp=0,data_ready=0;
const unsigned char SLAVE_ADDRESS=0x50;

unsigned long status;

void I2C_slave_config();
void read_buff();

//error interrupt
void I2C1_ER_Interrupt() iv IVT_INT_I2C1_ER ics ICS_AUTO {

    

}

//event interrupt : successful address/ data communication
void I2C1__EV_Interrupt() iv IVT_INT_I2C1_EV{
 
  //address match
  if(I2C1_SR1bits.ADDR==1){
   //read both sr1 and sr2 to clear interupt flag
   status = I2C1_Get_Status();
  }
   //if tx=1 write to DR
  else if((I2C1_SR1bits.TxE==1) && !(I2C1_SR1bits.BTF)){
                             
      I2C1_DR=tx_buff[data_len];
      data_len++;
   }
   //if rx=1 read DR
  else if(I2C1_SR1bits.RxNE==1){
                                 
      rx_buff[data_len]=I2C1_DR;
      data_len++;
   } 
     //STOPF bit end of comm
   else if(I2C1_SR1bits.STOPF==1){
      
      data_ready=1;
      read_buff();
      //read sr1 & set stop bit to clear stop flag
      status = I2C1_Get_Status();
      I2C1_CR1bits.STOP_=1;
      GPIOC_ODRbits.ODR13= 0;
   }
}

char i=0;
void main() {

      UART1_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART1_PA9_10);
      I2C_slave_config(); 
      UART1_Write_Text("Setup\n"); 
      Delay_ms(200); 
    
      while(1){
        
      if(data_ready==1){
      UART1_Write_Text("\nData Received: ");
      UART1_Write_Text(msg); 
      UART1_Write_Text("\r\n");   
      data_len=0;
      data_ready=0;
      }
      }
}

void I2C_slave_config(){
   
   RCC_APB1ENRbits.I2C1EN=1;
   //Master 100khz 
   I2C1_Init_Advanced(100000, &_GPIO_MODULE_I2C1_PB67); 
   //8 bit mode 
   I2C1_OAR1bits.ADD0=0;
   I2C1_OAR1=SLAVE_ADDRESS << 1; 
   
  //enable events interrupts
  I2C1_CR2bits.ITEVTEN=1;
  I2C1_CR2bits.ITBUFEN=1;
  NVIC_IntEnable(IVT_INT_I2C1_EV);
  // ack and peripheral enable
  I2C1_CR1bits.ACK=1;
  I2C1_CR1bits.PE=1;
}

void read_buff(){
for(i=0;i<data_len;i++)msg[i]=rx_buff[i];
}
