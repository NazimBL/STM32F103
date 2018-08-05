#define spi_T 100
int PWM;
unsigned int PWM_T,d;

sbit PV_CS1 at GPIOB_ODR.B8;
sbit PV_CS2 at GPIOB_ODR.B7;
sbit PV_CS3 at GPIOB_ODR.B6;
sbit PV_CS4 at GPIOA_ODR.B15;
sbit PV_CS5 at GPIOA_ODR.B12;

sbit Spi_CLK at GPIOB_ODR.B3;
sbit Spi_SDI at GPIOB_ODR.B4;
sbit Spi_SDO at GPIOB_ODR.B5;

char kk,p;
char test=1;
char readbuff[64],writebuff[64];

void adc_setup();
void SendPoint();

unsigned long vadc[64],iadc[64];
unsigned int J[4]={2000,2000,2000,2000};
unsigned char T[4]={25,25,25,25};
unsigned long vs[32],is[32];
unsigned long IVs[2];
int avg=0;

char temp=25,lowb,highb;
int irr=2000;


void SendVI();
void readJT();
char val1[4];
void sendVIsamples(char p);

unsigned char lowB,highB;

unsigned buffer=45,take=0;
void my_Write();

void InitMain()
{
  long cnf;

  GPIO_Digital_Output(&GPIOB_BASE, _GPIO_PINMASK_3| _GPIO_PINMASK_5| _GPIO_PINMASK_6| _GPIO_PINMASK_7| _GPIO_PINMASK_8);     // Set CS# pin as Output
  GPIO_Digital_Output(&GPIOA_BASE, _GPIO_PINMASK_12 | _GPIO_PINMASK_15);     // Set CS# pin as Output
  //GPIO_Digital_Output(&GPIOB_BASE, _GPIO_PINMASK_ALL);
  GPIO_Digital_Output(&GPIOC_BASE, _GPIO_PINMASK_13);     // Set LED pin as Output
  UART1_Init_Advanced(19200, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART1_PB67);
  GPIOB_ODR=0;
  GPIOC_ODR.B13=1;
  RCC_APB1ENR.TIM3EN = 1;       // Enable clock gating for timer module 2
  TIM3_CR1.CEN = 0;             // Disable timer
  // 1 hz == one sec
  TIM3_PSC = 1098;              // Set timer prescaler.
  TIM3_ARR = 65514;
  NVIC_IntEnable(IVT_INT_TIM3); // Enable timer interrupt
  TIM3_DIER.UIE = 1;            // Update interrupt enable
  TIM3_CR1.CEN = 1;             // Enable timer

  PWM_T=PWM_TIM2_Init(2000);
  PWM_TIM2_Start(_PWM_CHANNEL4, &_GPIO_MODULE_TIM2_CH4_PB11);
  //950
  d= (unsigned int)((long)PWM_T/2+(700*(long)PWM_T)/10000);
  PWM_TIM2_Set_Duty(d, _PWM_NON_INVERTED, _PWM_CHANNEL4);
  //PWM_TIM2_Set_Duty(PWM_T/2, _PWM_NON_INVERTED, _PWM_CHANNEL4);
  AFIOEN_bit = 1;
  SWJ_CFG1_bit = 1;  // PB3 alternate function enable
SPI1_Init_Advanced(_SPI_FPCLK_DIV256, _SPI_MASTER | _SPI_8_BIT | _SPI_CLK_IDLE_LOW |
   _SPI_FIRST_CLK_EDGE_TRANSITION | _SPI_MSB_FIRST | _SPI_SS_DISABLE | _SPI_SSM_DISABLE |
   _SPI_SSI_1, &_GPIO_MODULE_SPI1_PB345);
    //AFIO_MAPR.SPI1_REMAP=1;


}

//critical point update
void Timer3_interrupt() iv IVT_INT_TIM3
{
  TIM3_SR.UIF = 0;
        // Toggle PORTE led's
  //GPIOC_ODR=~GPIOC_ODR;
  
  //current then voltage
  //IVs[0]=(int)(1*(ADC1_Read(1)-ADC1_Read(0)));
  //IVs[1]=(int)(1*(ADC1_Read(2)-ADC1_Read(3)));
  
   
}


void main()
{

  HID_Enable(&readbuff,&writebuff);
  //adc_setup();
  GPIO_Digital_Output(&GPIOC_BASE, _GPIO_PINMASK_13);
  UART1_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART1_PB67);

                           GPIOC_ODRbits.ODR13=0;

     lowb=irr%256;
     highb=irr/256;

  while(1) 
  {


          UART1_Write_Text("1,0,1,0,1,1");
          Delay_ms(1000);

    
  }
}


void my_Write(){ 
        
        GPIOB_ODRbits.ODR13=0;
        SPI1_Write(buffer);
        GPIOB_ODRbits.ODR13=1;
        Delay_ms(500);

}
void adc_setup(){

  ADC_Set_Input_Channel(_ADC_CHANNEL_0);
  ADC_Set_Input_Channel(_ADC_CHANNEL_1);
  ADC_Set_Input_Channel(_ADC_CHANNEL_2);
  ADC_Set_Input_Channel(_ADC_CHANNEL_3);
  ADC1_Init();

}
