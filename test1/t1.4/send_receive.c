/****************************************************************************
* ���ô��ڵ������ֲ����ʣ�115200bps
*ʹ�õ��Ǵ���0
* ���ڵ������ָ�CC2530���ַ���ʱ��������᷵�ؽ��յ����ַ���
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
char RxData[SIZE];        //�洢�����ַ���

void DelayMS(uint msec);//ms��ʱ
void InitUart(void);//����0��ʼ��
void UartSendString(char *Data, int len);//�����ַ���
void InitLed();//LED��ʼ��
void InitKey();//KEY��ʼ��
void KeyScan();//����ɨ��

#pragma vector = URX0_VECTOR 
__interrupt void UART0_ISR(void) 
{ 
    URX0IF = 0;       // ���жϱ�־ 
    RxBuf = U0DBUF;                           
}
//-------------------------------------------------------------
void main(void)
{    
    CLKCONCMD &= ~0x40;                        //����ϵͳʱ��ԴΪ32MHZ����
    while(CLKCONSTA & 0x40);                   //�ȴ������ȶ�Ϊ32M
    CLKCONCMD &= ~0x47;                        //����ϵͳ��ʱ��Ƶ��Ϊ32MHZ   
    InitUart();                                //���ô��ڳ�ʼ������   
    UartState = UART0_RX;
    memset(RxData, 0, SIZE);
    InitKey();
    InitLed();
    while(1)
    {
        if(UartState == UART0_RX)              //����״̬ 1
        { 
            LED1 = 0;
            LED2 = 1;
            if(RxBuf != 0) 
            {                 
                if((RxBuf != '#')&&(count < 50))//��'��'Ϊ������,һ��������50���ַ�            
                    RxData[count++] = RxBuf;
                else
                {
                    if(count >= 50)             //�ж����ݺϷ��ԣ���ֹ���
                    {
                        count = 0;              //������0
                        memset(RxData, 0, SIZE);//��ս��ջ�����
                    }
                    else
                        U0CSR &= ~0x40;         //��ֹ����
                        UartState = UART0_TX;  //���뷢��״̬
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
    PERCFG = 0x00;           //������ƼĴ��� USART 0��IOλ��:0ΪP0��λ��1 
    P0SEL = 0x0c;            //P0_2,P0_3�������ڣ����蹦�ܣ�
    P2DIR &= ~0xC0;          //P0������ΪUART0
    U0CSR |= 0x80;           //����ΪUART��ʽ
    U0GCR |= 11;                       
    U0BAUD |= 216;           //��������Ϊ115200
    UTX0IF = 0;              //UART0 TX�жϱ�־��ʼ��λ0
    U0CSR |= 0x40;           //������� 
    IEN0 |= 0x84;            //�����ж���������ж� 

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
        if(UartState == UART0_TX)              //����״̬ 2
        {                          
            UartSendString(RxData, count);     //�����Ѽ�¼���ַ�����
            U0CSR |= 0x40;                     //������� 
            UartState = UART0_RX;              //�ָ�������״̬ 
            count = 0;                         //������0
            memset(RxData, 0, SIZE);           //��ս��ջ�����
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