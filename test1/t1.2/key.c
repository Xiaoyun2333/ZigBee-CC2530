#include <ioCC2530.h>
#define uint unsigned int
#define uchar unsigned char

#define LED1 P1_0
#define LED2 P1_1
#define LED3 P1_2

#define KEY1 P1_3 
#define KEY2 P1_4   

uchar state=0;

void Delayms(uint);
void InitLed(void);	
void InitKey();
void KeyScan();
void Led();
//-------------------------------------------------
void main(void)
{
	InitLed();
	InitKey();
  while(1)
	{
    KeyScan();
    Led();
  }
}
//-------------------------------------------------
void Delayms(uint xms) 
{
  uint i,j;
  for(i=xms;i>0;i--)
    for(j=587;j>0;j--);
}

void InitLed(void)
{
	P1DIR |= 0x07;
  P1SEL &= ~0x07;    
}

void InitKey()
{
  P1SEL &= ~0X18;
  P1DIR &= ~0X18; 
  P1INP &= ~0x18;
}

void KeyScan(void)
{
  if(KEY1 == 0)
  {Delayms(10);
      if(KEY1 == 0)
      {
        while(!KEY1);
        if(++state == 5)state = 0;
      }
  }
  if(KEY2 == 0)
  {Delayms(10);
      if(KEY2 == 0)
      {
        while(!KEY2);
        if(--state == 255)state = 4;
      }
  }
}

void Led()
{
  switch(state)
  {
    case 0:
      LED1 = 1;
      LED2 = 1;
      LED3 = 1;
    break;
    case 1:
      LED1 = 0;
      LED2 = 0;
      LED3 = 1;
    break;
    case 2:
      LED1 = 1;
      LED2 = 0;
      LED3 = 0;
    break;
    case 3:
      LED1 = 0;
      LED2 = 1;
      LED3 = 0;
    break;
    case 4:
      LED1 = 0;
      LED2 = 0;
      LED3 = 0;
    break;
  }
}
