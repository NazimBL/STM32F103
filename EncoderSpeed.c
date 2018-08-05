#include "TIM_COMMUN.h"
#include "TIM4.h"
#include "TIM1.h"
#include "AFIO.h"
#include "GPIO.h"

unsigned int overflow_count = 0;
unsigned long pulse_ticks = 0;
unsigned long start_time = 0;
unsigned long end_time = 0;
unsigned long channelA,channelB=0;
float f = 1;

void setup_TIM4();
void setup_TIM1();
void setup_IO();

//1 = postive , 0 negative
int motorDirection=1;
char chb=0;
char txt[15],direction[15];




void TIM4_ISR()
iv IVT_INT_TIM4
ics ICS_AUTO
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
     

      
      GPIO_Config(&GPIOB_BASE,_GPIO_PINMASK_6 | _GPIO_PINMASK_7,_GPIO_CFG_MODE_INPUT | _GPIO_CFG_PULL_UP);
     // GPIO_Config(&GPIOB_BASE,_GPIO_PINMASK_6 | _GPIO_PINMASK_7,_GPIO_CFG_AF_TIM4);
      
      //GPIO_Config(&GPIOB_BASE,_GPIO_PINMASK_6 | _GPIO_PINMASK_7,_GPIO_CFG_PULL_NO);
      GPIO_Digital_Output(&GPIOC_BASE, _GPIO_PINMASK_13);
      
      //GPIO_Config(&GPIOA_BASE,_GPIO_PINMASK_8,_GPIO_CFG_MODE_OUTPUT| _GPIO_CFG_SPEED_MAX | _GPIO_CFG_OTYPE_PP);

      setup_TIM4();
      setup_TIM1();

      UART1_Init(9600);
      Delay_ms(200);
      GPIOC_ODR=0x0000;
      
      UART1_Write_Text("Hello, encoder + STM32 by timer. Turn encoder to see change.\n");

     while(1)
     {        


             //frequency/number of ticks
             f=(float)pulse_ticks;
             f = (36000 / f);
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

     };
}


void setup_TIM4()
{

     enable_TIM4(true);
     enable_TIM4_counter(false);
     TIM4_ARR = 599;
     //TIM4_ARR = 50;
     TIM4_PSC = 1999;
     //60 hz
     TIM4_CCERbits.CC1P=0;
     set_TIM4_counting_direction(up_counting);
     set_TIM4_clock_division(clock_division_tCK_INT);
     set_TIM4_CC1_selection(CC1_input_IC1_on_TI1);
     set_TIM4_CC1_state_and_polarity(enable, rising_edge);
     set_TIM4_IC1_input_prescalar(0);
     set_TIM4_IC1_filter(0);
     
     TIM4_CCERbits.CC2P=0;
     set_TIM4_CC2_selection(CC2_input_IC2_on_TI2);
     set_TIM4_CC2_state_and_polarity(enable, rising_edge);
     set_TIM4_IC2_input_prescalar(0);
     set_TIM4_IC2_filter(0);
     
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
     AFIO_remap(TIM1_not_remapped);

     enable_GPIOA(true);
     setup_GPIOA(8, (AFIO_PP_output | output_mode_high_speed));
     enable_GPIOB(true);
     //setup_GPIOB(6, digital_input);

}
void setup_TIM1()
{
     enable_TIM1(true);
     enable_TIM1_counter(false);
     set_TIM1_counting_direction(up_counting);
     TIM1_ARR = 799;
     TIM1_PSC = 0;
     
     set_TIM1_OC1_compare_mode(PWM_mode_1);
     set_TIM1_CC1_state_and_polarity(enable, active_high);

     set_TIM1_auto_reload_preload_mode(true);
     set_TIM1_CC_preload_control(true);
     set_TIM1_auto_reload_preload_mode(true);
     enable_TIM1_counter(true);
     TIM1_CCR1 = 400;
}