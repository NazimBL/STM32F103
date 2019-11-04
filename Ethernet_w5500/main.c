/*
 Copyright:
 DeadLine, Bellabaci Nazim.
*/
 
#include "Wizchip_Conf.h"
#include "W5500.h"
#include "Socket.h"

// Socket & Port number definition
#define SOCK_ID_TCP       0
#define SOCK_ID_UDP       1
#define PORT_UDP          10001
#define DATA_BUF_SIZE     2048

uint8_t gDATABUF[DATA_BUF_SIZE];
uint8_t sendDATABUF[DATA_BUF_SIZE];
uint8_t destip[4];
uint8_t i=0,j=0;
uint8_t connected=0;
uint16_t destport;

void UDP_Send(uint8_t sendBUF[],uint16_t size);
void TimerSetup();


volatile wiz_NetInfo gWIZNETINFO =
{
  {0x00, 0x14, 0xA3, 0x72, 0x17, 0x3F},    // Source Mac Address
  {192, 168, 197, 52},                    // LOCAL RECEIVER ADDRESS
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
GPIOB_ODRbits.ODR12= 0;
}

// brief Call back function for WIZCHIP deselect.
void CB_ChipDeselect(void)
{
 GPIOB_ODRbits.ODR12= 1;
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
             connected=1;
            // Get received data size
            if((size = getSn_RX_RSR(SOCK_ID_UDP)) > 0)
            {
                // Cut data to size of data buffer
                if(size > DATA_BUF_SIZE)size = DATA_BUF_SIZE;
                // Get received data
                ret = recvfrom(SOCK_ID_UDP, gDATABUF, size, destip, (uint16_t*)&destport);
                // Check for error
                if(ret <= 0) return;
                /*
                // Send echo to remote
                 size=(uint16_t)ret;
                 sentsize = 0;
                while(sentsize != size)
                {
                    ret = sendto(SOCK_ID_UDP, sendDATABUF + sentsize, size - sentsize, destip, destport);
                    if(ret < 0)
                    {
                        return;
                    }
                    // Update number of sent bytes
                    sentsize += ret;
                } */
            }
            break;
        }
        // Socket is not opened
        case SOCK_CLOSED:
        {
            // Open UDP socket
            connected=0;
            if((ret=socket(SOCK_ID_UDP, Sn_MR_UDP, PORT_UDP, 0x00)) != SOCK_ID_UDP)
            {
                return;
            }
            break;
        }
        default :
        {
           connected=0;
           break;
        }
    }
}
// brief Initialize modules
static void W5500_Init(void)
{
    // Set Tx and Rx buffer size to 2KB
    uint8_t buffsize[8] = { 2, 2, 2, 2, 2, 2, 2, 2 };
    
    GPIO_Digital_Output(&GPIOB_BASE, _GPIO_PINMASK_13| _GPIO_PINMASK_15); //SCK|MOSI
    GPIO_Digital_Input(&GPIOB_BASE, _GPIO_PINMASK_14);//MISO
    GPIO_Digital_Output(&GPIOB_BASE, _GPIO_PINMASK_12);// Set CS# pin as Output
    GPIO_Digital_Output(&GPIOA_BASE, _GPIO_PINMASK_8);//W5500 Rst pin
    
    CB_ChipDeselect();
        
    SPI2EN_bit=1; 
    SPI2_Init_Advanced(_SPI_FPCLK_DIV4, _SPI_MASTER | _SPI_8_BIT |
                       _SPI_CLK_IDLE_LOW | _SPI_FIRST_CLK_EDGE_TRANSITION |
                        _SPI_MSB_FIRST | _SPI_SS_DISABLE | _SPI_SSM_ENABLE | _SPI_SSI_1,
                       &_GPIO_MODULE_SPI2_PB13_14_15);
                               
    // Reset module
    GPIOA_ODRbits.ODR8= 0;
    Delay_ms(1);
    GPIOA_ODRbits.ODR8= 1;
    Delay_ms(1);
                            
    // Wizchip initialize
    wizchip_init(buffsize, buffsize, 0, 0, CB_ChipSelect, CB_ChipDeselect, 0, 0, CB_SpiRead, CB_SpiWrite);

    // Wizchip netconf
    ctlnetwork(CN_SET_NETINFO, (void*) &gWIZNETINFO);
    ctlwizchip(CW_SET_PHYCONF, (void*) &phyConf);    
}

void Timer3_interrupt() iv IVT_INT_TIM3
{
  TIM3_SR.UIF = 0;
   
  // send dummy data to destination
  sendDATABUF[0]=0x45;
  sendDATABUF[1]=0x21;
  if(connected==1)UDP_Send(sendDATABUF,2);        
}

void main()
{
    TimerSetup();
    W5500_Init();   
    wizchip_getnetinfo(&pnetinfo);

    while(1)
    {                    
        UDP_Server();      
    }                            
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
void UDP_Send(uint8_t sendBUF[],uint16_t size){

uint16_t sentsize=0;
uint8_t ret=0;
           while(sentsize <= size)
                {
                    ret = sendto(SOCK_ID_UDP, sendBUF + sentsize, size - sentsize, destip, destport);
                    if(ret < 0)return;                   
                    // Update number of sent bytes
                    sentsize += ret;
                }
}
