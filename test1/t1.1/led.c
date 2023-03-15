#include <ioCC2530.h>	 

typedef unsigned char uint8;
typedef unsigned short int  uint16;

#define LED1 P1_0
#define LED2 P1_1
#define LED3 P1_2//���Ŷ���

void InitGPIO(void);//�ܽų�ʼ��
void DelayMS(uint16 msec);//ms��ʱ
void LED();//��ˮ�ƺ���
//------------------------------------------------------------
void main(void)
{     
  InitGPIO();
  while(1)
  {
    LED();
  }    
}
//------------------------------------------------------------
void InitGPIO(void)
{
  P1DIR |= 0x07;
  P1SEL &= ~0x07;
}

void DelayMS(uint16 msec)          
{                                  
    uint16 i,j; 
    for (i=0; i<msec; i++)
        for (j=0; j<536; j++);
}

void LED()
{
  LED1 = 0;
  LED2 = 1;
  LED3 = 0;
  DelayMS(500);

  LED1 = ~LED1;
  LED2 = ~LED2;
  LED3 = ~LED3;
  DelayMS(500);

  LED1 = ~LED1;
  LED3 = ~LED3;
  DelayMS(500);

  LED1 = ~LED1;
  LED2 = ~LED2;
  LED3 = ~LED3;
  DelayMS(500);

  LED1 = ~LED1;
  DelayMS(500);

  LED1 = ~LED1;
  LED2 = ~LED2;
  DelayMS(500);

  LED2 = ~LED2;
  LED3 = ~LED3;
  DelayMS(500);

  LED3 = ~LED3;
  DelayMS(500);

  LED1 = ~LED1;
  LED2 = ~LED2;
  LED3 = ~LED3;
  DelayMS(500);
  
  LED1 = ~LED1;
  LED2 = ~LED2;
  LED3 = ~LED3;
  DelayMS(500);
}
