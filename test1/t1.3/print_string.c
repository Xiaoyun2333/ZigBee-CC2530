#include "ioCC2530.h"  
    
#define KEY1 P1_3

unsigned int counter = 0;    

void initUART0();//串口0初始化,波特率9600
void UART0SendByte(unsigned char dat);//发送一个字符
void UART0SendString(unsigned char *str);//发送字符串
void initKey();//key初始化
void KeyScan();//按键扫描
void DelayMS(unsigned int msec);//ms延时

//--------------------------------------------------------------
void main(void)
{  
  initUART0();
  initKey();
  while(1)
  {
    KeyScan();
  }
}
//--------------------------------------------------------------
void initUART0(void)
{
  CLKCONCMD &= 0x80;   //时钟速度设置为32MHz    
  PERCFG = 0x00;    
  P0SEL = 0x3c; 
  U0CSR |= 0x80; 
  U0BAUD = 216;
  U0GCR = 11; 
  U0UCR |= 0x80;  
  UTX0IF = 0;  // 清零UART0 TX中断标志

  EA = 1;
}

void UART0SendByte(unsigned char dat)
{
  U0DBUF = dat;       
  while (!UTX0IF);  // 等待TX中断标志，即U0DBUF就绪
  UTX0IF = 0;       // 清零TX中断标志 
}

void UART0SendString(unsigned char *str)
{
  while(*str != '\0')
  {
    UART0SendByte(*str++);   // 发送一字节
  } 
}

void initKey()
{
  P1SEL &= ~0X08;
  P1DIR &= ~0X08; 
  P1INP &= ~0x08;
}

void KeyScan()
{
  if(KEY1 == 0)
  {
    DelayMS(10);
    if(KEY1 == 0)
    {
      while(KEY1);
      UART0SendString("Hello! I am Xiaoyun Sifu :) \n");
    }
  }
}

void DelayMS(unsigned int msec)
{
  unsigned int i,j; 
  for (i=0; i<msec; i++)
    for (j=0; j<536; j++);
}