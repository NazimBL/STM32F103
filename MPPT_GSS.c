 /*
 * Nazim BL
 * mikroC PRO for ARM
 */

 //STM32f03 , Bluepill 72Mhz, external Quartz + PLLx9 

 unsigned int k=0,v1=0,v2=0,p1=0,p2=0;
 const unsigned int VMAX=315;
 unsigned int a=5,b=300,delta=1,diff=0;
 long idc=0,vdc=0;
 
 void setup();
 void PWM_Setup(unsigned long fo);
 void setVref(unsigned int vo);
 unsigned long calPower(unsigned int vo);
 
 void main() {

        setup();
        while(1){
        
        p1=calPower(v1);
        p2=calPower(v2);
        
        if(p2>p1) a=v1;

        else{
         b=v2;
         v2=a+0.618*(b-a);
        }
        
        diff=(v2-v1);
        if(diff<delta)break;
       
        setVref((v1+v2)/2);
        } 
}

void setup(){

ADC_Set_Input_Channel(_ADC_CHANNEL_0);
ADC_Set_Input_Channel(_ADC_CHANNEL_1);

v1=b-0.618*(b-a);
v2=a+0.618*(b-a);

//2Khz
PWM_Setup(2000);
}

void PWM_Setup(unsigned long fo){

   k = PWM_TIM3_Init(fo);
   PWM_TIM3_Set_Duty(0, _PWM_NON_INVERTED, _PWM_CHANNEL1);  
   PWM_TIM3_Start(_PWM_CHANNEL1, &_GPIO_MODULE_TIM3_CH1_PC6);
}

void setVref(unsigned int vo){

     //duty cycle = (vo/VMAX)*100;
     unsigned int ratio=vo/VMAX;
     k=k*ratio;
   
     PWM_TIM3_Set_Duty(k, _PWM_NON_INVERTED, _PWM_CHANNEL1);
     PWM_TIM3_Start(_PWM_CHANNEL1, &_GPIO_MODULE_TIM3_CH1_PC6);
}

unsigned long calPower(unsigned int vo){
      
      setVref(vo);
      //add more precise condition
      while(vdc!=vo){
      vdc=ADC1_Get_Sample(0);
      idc=ADC1_Get_Sample(1);
      
      //add delay
      }
      return vdc*idc;
}
