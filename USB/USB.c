char kk;
char readbuff[64],writebuff[64];
char i=0;

void main() {
  
  HID_Enable(&readbuff,&writebuff);
  //initialize write buffer with random values
  for(i=0;i<64;i++)writebuff[i]=i;
 
  while(1)
  {
   
    USB_Polling_Proc();
    kk = HID_Read();
   
    if(kk != 0)
    {
      if((char)readbuff[0]=='O'){
    
       HID_Write(writebuff,64);
       readbuff[0]='x';    
      }
    }   
  }
}
