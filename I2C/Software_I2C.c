// Software I2C connections
sbit Soft_I2C_Scl_Output    at GPIOB_ODR.B6;
sbit Soft_I2C_Scl_Input     at GPIOB_IDR.B6;
sbit Soft_I2C_Sda_Output    at GPIOB_ODR.B7;
sbit Soft_I2C_Sda_Input     at GPIOB_IDR.B7;

char err;
char txt[4];

void main() {

        Soft_I2C_Init();
        UART1_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART1_PA9_10);
        UART1_Write_Text("Setup\n");
        Delay_ms(200);
         
        while{
        
        // Used this to read from the MAX30100 heartbeat sensor, check my repository for the sensor's library for more info
        Soft_I2C_Start();
        Soft_I2C_Write(0xAE);
        Soft_I2C_Write(0x00);
        Soft_I2C_Start();
        Soft_I2C_Write(0xAF);
        err = Soft_I2C_Read(0);
        Soft_I2C_Stop();
        
        //use Conversion library to make the byte readable
        ByteToStr(err, txt);
        UART1_Write_Text("message:");
        UART1_Write_Text(txt);
        UART1_Write_Text("\n\r");
        
        }
}        
        
