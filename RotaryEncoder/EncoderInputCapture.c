/*
* Nazim BL
* MPLAB X IDE
*/

#include "TIM_COMMUN.h"
#include "TIM4.h"
#include "AFIO.h"
#include "GPIO.h"

unsigned int overflow_count = 0;
unsigned long pulse_ticks = 0;
unsigned long start_time = 0;
unsigned long end_time = 0;
unsigned long channelA,channelB=0;
float f = 1;

void setup_TIM4();
void setup_IO();

//1 = postive , 0 negative
int motorDirection=1;
char txt[15],direction[15];

void TIM4_ISR() iv IVT_INT_TIM4 ics ICS_AUTO
{
    if(TIM4_SRbits.UIF == 1)
    {
        TIM4_SRbits.UIF = 0;
        overflow_count++;
    }

    if(TIM4_SRbits.CC1IF == 1)
    {
        //check B pulse to know direction
        //if(B==0) postive
        //else negative
        TIM4_SRbits.CC1IF = 0;
        end_time = TIM4_CCR1;
        channelA=end_time;
        channelB=TIM4_CCR2;
        if(GPIOB_IDRbits.IDR7)motorDirection=1;
        else motorDirection=0;

        pulse_ticks = ((overflow_count << 16) - start_time + end_time);
       // pulse_ticks = end_time-start_time ;
        start_time = end_time;
        overflow_count = 0;

    }
    if(TIM4_SRbits.CC2IF == 1)chb=1;
    else chb=0;
}
void main() {
     
      //Timer 4 input capture pins
      GPIO_Config(&GPIOB_BASE,_GPIO_PINMASK_6 | _GPIO_PINMASK_7,_GPIO_CFG_MODE_INPUT | _GPIO_CFG_PULL_UP);
      //debug led
      GPIO_Digital_Output(&GPIOC_BASE, _GPIO_PINMASK_13);

      setup_TIM4();
      UART1_Init(9600);

      Delay_ms(200);
      GPIOC_ODR=0xFFFF;

      UART1_Write_Text(Encoder input capture. Turn encoder to see change.\n");

     while(1)
     {        
             //frequency/number of ticks
             f=(float)pulse_ticks;
             f = (100000/f);
             f/=1024;
             //rpm=f*60
             FloatToStr(f, txt);
             UART1_Write_Text("frequency\n");
             UART1_Write_Text(txt);
             UART1_Write('\n');
             IntToStr(motorDirection, txt);
             UART1_Write_Text("direction\n");
             UART1_Write_Text(txt);
             UART1_Write('\n');
             UART1_Write_Text("------------------------------\n");
             Delay_ms(200);
     }
}


void setup_TIM4()
{
     enable_TIM4(true);
     enable_TIM4_counter(false);
     TIM4_ARR = 0xFFFF;
     TIM4_PSC = 719;
     //100 khz
     TIM4_CCERbits.CC1P=0;
     set_TIM4_counting_direction(up_counting);
     set_TIM4_clock_division(clock_division_tCK_INT);
     set_TIM4_CC1_selection(CC1_input_IC1_on_TI1);
     set_TIM4_CC1_state_and_polarity(enable, rising_edge);
     set_TIM4_IC1_input_prescalar(0);
     set_TIM4_IC1_filter(3);
     
     TIM4_CCERbits.CC2P=0;
     set_TIM4_CC2_selection(CC2_input_IC2_on_TI2);
     set_TIM4_CC2_state_and_polarity(enable, rising_edge);
     set_TIM4_IC2_input_prescalar(0);
     set_TIM4_IC2_filter(3);
     
     enable_TIM4_CC1_interrupt(true);
     enable_TIM4_update_interrupt(true);
     NVIC_IntEnable(IVT_INT_TIM4);
     EnableInterrupts();
     enable_TIM4_counter(true);
}
void setup_IO()
{
     AFIO_enable(true);
     AFIO_remap(TIM4_not_remapped);
     enable_GPIOB(true);
}
