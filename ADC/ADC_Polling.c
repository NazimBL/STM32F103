void adcInit(){

GPIO_Analog_Input(&GPIOA_BASE, _GPIO_PINMASK_2);
GPIO_Analog_Input(&GPIOA_BASE, _GPIO_PINMASK_3);

ADC1_CR1bits.SCAN=1;
ADC1_CR1bits.DISCEN=1;
ADC1_CR2bits.CAL=1;
ADC1_CR1bits.DUALMOD=0;
ADC1_CR1bits.DISCNUM=0;
}
unsigned adcRead(char channel){

ADC1_SQR1bits.SQ13=channel;

ADC1_CR2bits.ALIGN=0;
ADC1_CR2bits.SWSTART=1;
ADC1_CR2bits.ADON=1;

while(ADC1_SRbits.EOC!=1);
return ADC1_DRbits.DATA;

}