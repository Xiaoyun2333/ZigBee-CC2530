#include "ioCC2530.h"  
    
#define KEY1 P1_3

unsigned int counter = 0;    

void initUART0();//����0��ʼ��,������9600
void UART0SendByte(unsigned char dat);//����һ���ַ�
void UART0SendString(unsigned char *str);//�����ַ���
void initKey();//key��ʼ��
void KeyScan();//����ɨ��
void DelayMS(unsigned int msec);//ms��ʱ

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
  CLKCONCMD &= 0x80;   //ʱ���ٶ�����Ϊ32MHz    
  PERCFG = 0x00;    
  P0SEL = 0x3c; 
  U0CSR |= 0x80; 
  U0BAUD = 216;
  U0GCR = 11; 
  U0UCR |= 0x80;  
  UTX0IF = 0;  // ����UART0 TX�жϱ�־

  EA = 1;
}

void UART0SendByte(unsigned char dat)
{
  U0DBUF = dat;       
  while (!UTX0IF);  // �ȴ�TX�жϱ�־����U0DBUF����
  UTX0IF = 0;       // ����TX�жϱ�־ 
}

void UART0SendString(unsigned char *str)
{
  while(*str != '\0')
  {
    UART0SendByte(*str++);   // ����һ�ֽ�
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