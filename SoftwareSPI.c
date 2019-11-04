/*
 Copyright:
 DeadLine, Bellabaci Nazim
 mikroC PRO for ARM
*/

#define spi_T 10

sbit Spi_CLK at GPIOA_ODR.B6;
sbit Spi_SDI at GPIOB_ODR.B0;
sbit Spi_CS at GPIOB_ODR.B1;

char SPI_transfer(char data0);
void writeDAC(unsigned dac,unsigned char channel);

void main(){
	
	
	writeDAC(2000,1);
}

//DAC interface function
void writeDAC(unsigned dac,unsigned char channel){
    
        char lowB,highB;
        lowB=dac & 0xFF;
        highB=dac >> 8;
        //add command word
        highB+=16;
        highB+=(channel*128);
        
        Spi_CS=0;
        SPI_transfer(highB);
        SPI_transfer(lowB);
        Spi_CS=1;
}

char SPI_transfer(char data0){
  
  char counter;
  
  for(counter = 8; counter>0; counter--)
  {
    if (data0 & 0x80)Spi_SDI = 1;
    else Spi_SDI = 0;
    data0 <<= 1;
  
    Spi_CLK = 1;     
    Delay_us(spi_T);
    Spi_CLK = 0;     
    Delay_us(spi_T);
  }
  return(data0);
}
