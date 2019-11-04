#include "AT_Commands.h"
const char SSID[] = "DJAWEB_76E8";
const char password[] = "";

const char port[] = "1000";
const char msg_hdr[] = "+IPD";
#define CR           0x0D
#define LF           0x0A
#define NEW_CONFIGURATION

char rx_buff[64];
char data_ready;
char data_len, tmp;

void UART3_Interrupt() iv IVT_INT_USART3 ics ICS_AUTO {
   tmp = UART3_Read();
   switch(tmp) {
     case LF:
        if(data_len > 1)
        {
          data_ready = 1;
        }
        break;
     case CR:
        rx_buff[data_len] = 0; //but not necessary
        break;
     default:
        rx_buff[data_len] = tmp;
        data_len++;
   }
}

USB_Write_Text(char *txt)
{
 HID_Write(txt,64);
}

void UART_Write_AT(char *CMD) 
{
   UART3_Write_Text(CMD);
   UART3_Write(0x0D);
   UART3_Write(0x0A);
}

void reset_buff() 
{
  memset(rx_buff,0,data_len);
  data_ready = 0;
  data_len=0;
}

char response_success() 
{
    char result;
    while(!data_ready);
    if((strstr(rx_buff,"OK")) || (strstr(rx_buff,"no change")) || (strstr(rx_buff,"ready"))) {
       result = 1;
    }else result = 0;
    reset_buff();
    return result;
}

void Get_IP()
{
   char response_cnt, conn_flg;
     USB_Write_Text("Getting IP Address\n\r");
     USB_Write_Text("Please wait...\n\r");
     UART_Write_AT("AT+CIFSR");
     while(!data_ready);
     reset_buff();
     while(!data_ready);
     //TFT_Write_Text(rx_buff,10,70);  //this is the IP address
     reset_buff();
}

void Wifi_Click_Config()
{
   char ap_cnt = 0;
   memset(rx_buff,0,64);
   USB_Write_Text("Resetting module");
   USB_Write_Text("Please wait...");
   UART_Write_AT("AT+RST");        		//Reset module
   while(!response_success());
   //delay_ms(1000);

   USB_Write_Text("Setting STA mode");
   USB_Write_Text("Please wait...");
   UART_Write_AT("AT+CWMODE=1");		//change the working mode to 1
   while(!response_success());                  //wait to respond success

   USB_Write_Text("Setting connection mode");
   USB_Write_Text("Please wait...");
   UART_Write_AT("AT+CIPMUX=1");               //change the connection mode to 1
   while(!response_success());                 //wait to respond succes
   //delay_ms(1000);
  
   while(1) 
   {                                                  
     USB_Write_Text("Connecting to AP\n\r");  //Join to AP
     USB_Write_Text("Please wait...\n\r");
     UART3_Write_Text("AT+CWJAP=");
     UART3_Write('"');
     UART3_Write_Text(SSID);
     UART3_Write('"');
     UART3_Write(',');
     UART3_Write('"');
     UART3_Write_Text(password);
     UART3_Write('"');
     UART_Write(CR);
     UART_Write(LF);
     if(response_success())
     {
       USB_Write_Text("Success\n\r");
       break;
     }else
     {
       USB_Write_Text("Try again...");
       //delay_ms(2000);
     }
   }
   //delay_ms(1000);
   //test
   USB_Write_Text("Check connection\n\r");
   USB_Write_Text("Please wait...\n\r");
   UART3_Write_Text("AT+CWJAP=?");
   while(!response_success());
   USB_Write_Text("WiFi Click Connected\n\r");
   //delay_ms(1000);
   //end test

   USB_Write_Text("Configure TCP server\n\r");
   USB_Write_Text("Please wait...\n\r");
   UART3_Write_Text("AT+CIPSERVER=1");           //Configure TCP server
   UART3_Write(',');
   UART3_Write_Text(port);
   UART3_Write(CR);
   UART3_Write(LF);
   while(!response_success());
   //delay_ms(1000);
   //
   Get_IP();                                     //Get IP address or check connection
   }
}