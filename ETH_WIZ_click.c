/*

 * Copyright:
 c) MicroTech Lab , Bellabaci Nazim.

*/
 
#include "Wizchip_Conf.h"
#include "W5500.h"
#include "Socket.h"

// Socket & Port number definition
#define SOCK_ID_TCP       0
#define SOCK_ID_UDP       1
#define PORT_UDP          5010
#define DATA_BUF_SIZE     2048

uint8_t gDATABUF[DATA_BUF_SIZE];
uint8_t sendDATABUF[DATA_BUF_SIZE];
uint8_t destip[4];
char i=0,j=0;
uint16_t destport;
uint8_t connected=0;
void UDP_Send(uint8_t sendBUF[],uint16_t size);
unsigned char lowB,highB;

volatile wiz_NetInfo gWIZNETINFO =
{
  {0x00, 0x14, 0xA3, 0x72, 0x17, 0x3F},    // Source Mac Address
  {169, 254, 197, 52},                    // LOCAL RECEIVER ADDRESS
  {255, 255, 0, 0},                      // Subnet Mask
  {0, 0,  0, 0},                       // Gateway IP Address
  {0, 0,  0, 0},                      // DNS server IP Address
  NETINFO_STATIC
  
 };

volatile wiz_PhyConf phyConf =
{
  PHY_CONFBY_HW,       // PHY_CONFBY_SW
  PHY_MODE_MANUAL,     // PHY_MODE_AUTONEGO
  PHY_SPEED_10,        // PHY_SPEED_100
  PHY_DUPLEX_FULL,     // PHY_DUPLEX_HALF
};

volatile wiz_NetInfo pnetinfo;

// brief Call back function for WIZCHIP select.
void CB_ChipSelect(void)
{
GPIOB_ODRbits.ODR13=0;
}

// brief Call back function for WIZCHIP deselect.
void CB_ChipDeselect(void)
{
GPIOB_ODRbits.ODR13=1;
}

// brief Callback function to read byte usig SPI.
uint8_t CB_SpiRead(void)
{
    return SPI_Read(0xAA);
}

// brief Callback function to write byte usig SPI.
void CB_SpiWrite(uint8_t wb)
{
    SPI_Write(wb);

}

void UDP_Send(uint8_t sendBUF[],uint16_t size){

uint16_t sentsize=0;
uint8_t ret=0;

            while(sentsize <= size)
                {
                    ret = sendto(SOCK_ID_UDP, sendBUF + sentsize, size - sentsize, destip, destport);
                    if(ret < 0)
                    {
                        return;
                    }
                    // Update number of sent bytes
                    sentsize += ret;
                }

}


// brief Handle UDP socket state.
void UDP_Server(void)
{
    int32_t  ret;
    uint16_t size, sentsize;
    // Get status of socket

    switch(getSn_SR(SOCK_ID_UDP))

    {

        // Socket is opened in UDP mode
        case SOCK_UDP:
        {
            // Get received data size

            connected=1;
            UART1_Write_Text("socket is opened\n");

            if((size = getSn_RX_RSR(SOCK_ID_UDP)) > 0)
            {
                // Cut data to size of data buffer
                if(size > DATA_BUF_SIZE)
                {
                    size = DATA_BUF_SIZE;
                }

                // Get received data
                ret = recvfrom(SOCK_ID_UDP, gDATABUF, size, destip, (uint16_t*)&destport);
                UART1_Write_Text("Received data is : ");
                UART1_Write_Text(gDATABUF);
                UART1_Write('\n');
                

                // Check for error
                if(ret <= 0)
                {
                   UART1_Write_Text("Reading Error\n");
                   return;
                }


                 /*
                // Send echo to remote
                size = (uint16_t) ret;
                sentsize = 0;
                while(sentsize != size)
                {
                    ret = sendto(SOCK_ID_UDP, gDATABUF + sentsize, size - sentsize, destip, destport);
                    if(ret < 0)
                    {
                        return;
                    }
                    // Update number of sent bytes
                    sentsize += ret;
                }
                */

            }
            break;
        }

        // Socket is not opened
        case SOCK_CLOSED:
        {
            // Open UDP socket
                 connected=0;
            UART1_Write_Text("socket is closed\n");

            if((ret=socket(SOCK_ID_UDP, Sn_MR_UDP, PORT_UDP, 0x00)) != SOCK_ID_UDP)
            {
                return;
            }

            break;
        }


        default :
        {
           connected=0;
           UART1_Write_Text("ni closed ni opened\n");
           break;
        }
    }
}

// brief Initialize modules
static void W5500_Init(void)
{
    // Set Tx and Rx buffer size to 2KB
    uint8_t buffsize[8] = { 2, 2, 2, 2, 2, 2, 2, 2 };

    GPIO_Digital_Output(&GPIOB_BASE,_GPIO_PINMASK_12);                           // Set Rst pin to be output
    GPIO_Digital_Output(&GPIOB_BASE,_GPIO_PINMASK_13);
    GPIO_Digital_Output(&GPIOC_BASE,_GPIO_PINMASK_13);

                            // Set CS pin to be output

    CB_ChipDeselect();                                                          // Deselect module

    // Reset module
    //Wizchip_Rst = 0;
    GPIOB_ODRbits.ODR12=0;
    Delay_ms(1);
    // Wizchip_Rst = 1;
    GPIOB_ODRbits.ODR12=1;
    Delay_ms(1);

      AFIOEN_bit = 1;
      SWJ_CFG1_bit = 1;  // PB3 alternate function enable
    SPI1_Init_Advanced(_SPI_FPCLK_DIV4, _SPI_MASTER | _SPI_8_BIT |
                       _SPI_CLK_IDLE_LOW | _SPI_FIRST_CLK_EDGE_TRANSITION |
                       _SPI_MSB_FIRST | _SPI_SS_DISABLE | _SPI_SSM_ENABLE | _SPI_SSI_1,
                      &_GPIO_MODULE_SPI1_PB345);
                      
    AFIO_MAPR.SPI1_REMAP=1;

    // Wizchip initialize
    wizchip_init(buffsize, buffsize, 0, 0, CB_ChipSelect, CB_ChipDeselect, 0, 0, CB_SpiRead, CB_SpiWrite);

    // Wizchip netconf
    ctlnetwork(CN_SET_NETINFO, (void*) &gWIZNETINFO);
    ctlwizchip(CW_SET_PHYCONF, (void*) &phyConf);
    

    
}

void TimerSetup(){

RCC_APB1ENR.TIM3EN = 1;       // Enable clock gating for timer module 2
TIM3_CR1.CEN = 0;             // Disable timer
  // 1 hz == one sec
TIM3_PSC = 1098;              // Set timer prescaler.
TIM3_ARR = 65514;
NVIC_IntEnable(IVT_INT_TIM3); // Enable timer interrupt
TIM3_DIER.UIE = 1;            // Update interrupt enable
TIM3_CR1.CEN = 1;             // Enable timer

}
void Timer3_interrupt() iv IVT_INT_TIM3
{
  TIM3_SR.UIF = 0;
  // test dummy values
  sendDATABUF[0]=0xFF;
  sendDATABUF[1]=0x10;
  if(connected==1)UDP_Send(sendDATABUF,2);
  /*
   for(i=0;i<6;i++){
   adc_result=ADC1_Get_Sample(i);
   lowB=adc_result & 0xFF;
   highB=adc_result >> 8;
   sendDATABUF[j++]=lowB;
   sendDATABUF[j++]=highB;
   
   }
   12 = 6x2 , size of thedata to be sent
   if(connected==1)UDP_Send(sendDATABUF,12);
   
  */
}

void main()
{
    W5500_Init();
    TimerSetup();
    GPIOC_ODRbits.ODR13=0;
    UART1_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART1_PB67);
    wizchip_getnetinfo(&pnetinfo);



    while(1)
    {

          UDP_Server();


    }
}