void timer_setup(void)
{
     RCC_APB1ENR.TIM4EN = 1;//  clock enable for timer_4
     TIM4_CR1bits.CEN=0;    //  timer_4 disable
     TIM4_SMCRbits.SMS=1;// encoder mode
     TIM4_CCERbits.CC1P=0;// capture is done on a rising edge for PB6
     TIM4_CCERbits.CC2P=0;// capture is done on a rising edge for PB7
     TIM4_PSC = 0;    //no prescaler
     TIM4_ARR=1000;  // encoder range from 0 to 1000
     TIM4_CCMR1_Inputbits.IC1F=15;// filter
     TIM4_CR1bits.CEN=1;  //  timer_4 enable    
}

char buf[10];
unsigned int last_encoder_state=0;

void main() {
    

    GPIO_Config(&GPIOB_BASE,_GPIO_PINMASK_6 | _GPIO_PINMASK_7,_GPIO_CFG_MODE_INPUT | _GPIO_CFG_PULL_UP);// setup port B.6 and B.7 as input with pull up
    UART1_Init(9600);
    timer_setup();
    UART1_Write_Text("Encoder + STM32 by timer. Turn encoder to see change.");

    while(1)
    {
        if(last_encoder_state!=TIM4_CNT)
        {
            last_encoder_state=TIM4_CNT;
            IntToStr(last_encoder_state, buf);
            UART1_Write_Text(buf);
        }
        Delay_ms(100);
    }

}

