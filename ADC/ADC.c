/*
* Nazim BL
* DeadLine Tech
* mikroC PRO for ARM IDE
*/
unsigned long adc_result = 0;
unsigned char lowB,highB;
char val1[4],val2[4];

void main() {

     ADC_Set_Input_Channel(_ADC_CHANNEL_0);
     ADC1_Init();
     UART1_Init(9600);
     Delay_ms(100);
    
     while(1){
     
      adc_result=ADC1_Get_Sample(0);
      lowB=adc_result & 0xFF;
      highB=adc_result >> 8;
      
      UART1_Write_Text("ADC Value 1: ");
      sprintf(val1,"%d",lowB);
      UART1_Write_Text(val1);
      UART1_Write('\n');

      UART1_Write_Text("ADC Value 2: ");
      sprintf(val2,"%d",highB);
      UART1_Write_Text(val2);
      UART1_Write('\n');
      Delay_us(1000);
     }
}
