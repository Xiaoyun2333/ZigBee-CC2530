/****************************************************************************
* 设置串口调试助手波特率：115200bps
*使用的是串口0
* 串口调试助手给CC2530发字符串时，开发板会返回接收到的字符串
****************************************************************************/
#include <ioCC2530.h>
#include <string.h>

typedef unsigned char uchar;
typedef unsigned int  uint;

#define UART0_RX    1
#define UART0_TX    2
#define SIZE       51

#define LED1 P1_0  
#define LED2 P1_1
#define KEY1 P1_3

char RxBuf;
char UartState;
uchar count;
char RxData[SIZE];        //存储发送字符串

void DelayMS(uint msec);//ms延时
void InitUart(void);//串口0初始化
void UartSendString(char *Data, int len);//发送字符串
void InitLed();//LED初始化
void InitKey();//KEY初始化
void KeyScan();//按键扫描

#pragma vector = URX0_VECTOR 
__interrupt void UART0_ISR(void) 
{ 
    URX0IF = 0;       // 清中断标志 
    RxBuf = U0DBUF;                           
}
//-------------------------------------------------------------
void main(void)
{    
    CLKCONCMD &= ~0x40;                        //设置系统时钟源为32MHZ晶振
    while(CLKCONSTA & 0x40);                   //等待晶振稳定为32M
    CLKCONCMD &= ~0x47;                        //设置系统主时钟频率为32MHZ   
    InitUart();                                //调用串口初始化函数   
    UartState = UART0_RX;
    memset(RxData, 0, SIZE);
    InitKey();
    InitLed();
    while(1)
    {
        if(UartState == UART0_RX)              //接收状态 1
        { 
            LED1 = 0;
            LED2 = 1;
            if(RxBuf != 0) 
            {                 
                if((RxBuf != '#')&&(count < 50))//以'＃'为结束符,一次最多接收50个字符            
                    RxData[count++] = RxBuf;
                else
                {
                    if(count >= 50)             //判断数据合法性，防止溢出
                    {
                        count = 0;              //计数清0
                        memset(RxData, 0, SIZE);//清空接收缓冲区
                    }
                    else
                        U0CSR &= ~0x40;         //禁止接收
                        UartState = UART0_TX;  //进入发送状态
                        LED1 = 1;
                        LED2 = 0;
                }
                RxBuf  = 0;
            }
        }
        KeyScan();  
    }
}
//-------------------------------------------------------------
void DelayMS(uint msec)
{ 
    uint i,j;  
    for(i=0; i<msec; i++)
        for(j=0; j<1070; j++); 
}

void InitUart(void)
{ 
    PERCFG = 0x00;           //外设控制寄存器 USART 0的IO位置:0为P0口位置1 
    P0SEL = 0x0c;            //P0_2,P0_3用作串口（外设功能）
    P2DIR &= ~0xC0;          //P0优先作为UART0
    U0CSR |= 0x80;           //设置为UART方式
    U0GCR |= 11;                       
    U0BAUD |= 216;           //波特率设为115200
    UTX0IF = 0;              //UART0 TX中断标志初始置位0
    U0CSR |= 0x40;           //允许接收 
    IEN0 |= 0x84;            //开总中断允许接收中断 

    EA = 1;
}

void UartSendString(char *Data, int len)
{
    uint i;
      
    for(i=0; i<len; i++)
    {
        U0DBUF = *Data++;
        while(UTX0IF == 0);
        UTX0IF = 0;
    }
}

void InitLed()
{
    P1DIR |= 0x03;
    P1SEL &= ~0x03;
}
void KeyScan()
{
    if(KEY1 == 0)
    {
        DelayMS(10);
        if(KEY1 == 0)
        {
        while(KEY1);
        if(UartState == UART0_TX)              //发送状态 2
        {                          
            UartSendString(RxData, count);     //发送已记录的字符串。
            U0CSR |= 0x40;                     //允许接收 
            UartState = UART0_RX;              //恢复到接收状态 
            count = 0;                         //计数清0
            memset(RxData, 0, SIZE);           //清空接收缓冲区
        }
    }
  }
}
void InitKey()
{
    P1SEL &= ~0X08;
    P1DIR &= ~0X08; 
    P1INP &= ~0x08;
}