/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����        */
/*---------------------------------------------------------------------*/


/*************  ����˵��    **************

�����̻���STC8H8K64UΪ����оƬ��ʵ����8���б�д���ԣ�STC8Hϵ��оƬ��ͨ�òο�.

ͨ��P2.7�����PWM����������.

����ɨ�谴��ÿ���������º��������һ��.

����ʱ, ѡ��ʱ�� 24MHZ (�û��������޸�Ƶ��).

******************************************/

#include    "reg51.h"       //������ͷ�ļ������������ļĴ�������Ҫ���ֶ����룬�����ظ�����
#include    "intrins.h"

#define     MAIN_Fosc       24000000L   //������ʱ��

typedef     unsigned char   u8;
typedef     unsigned int    u16;
typedef     unsigned long   u32;

//�ֶ���������"reg51.h"ͷ�ļ�����û�ж���ļĴ���
sfr TH2  = 0xD6;
sfr TL2  = 0xD7;
sfr IE2   = 0xAF;
sfr INT_CLKO = 0x8F;
sfr AUXR = 0x8E;
sfr P_SW1 = 0xA2;
sfr P_SW2 = 0xBA;

sfr P4   = 0xC0;
sfr P5   = 0xC8;
sfr P6   = 0xE8;
sfr P7   = 0xF8;
sfr P1M1 = 0x91;    //PxM1.n,PxM0.n     =00--->Standard,    01--->push-pull
sfr P1M0 = 0x92;    //                  =10--->pure input,  11--->open drain
sfr P0M1 = 0x93;
sfr P0M0 = 0x94;
sfr P2M1 = 0x95;
sfr P2M0 = 0x96;
sfr P3M1 = 0xB1;
sfr P3M0 = 0xB2;
sfr P4M1 = 0xB3;
sfr P4M0 = 0xB4;
sfr P5M1 = 0xC9;
sfr P5M0 = 0xCA;
sfr P6M1 = 0xCB;
sfr P6M0 = 0xCC;
sfr P7M1 = 0xE1;
sfr P7M0 = 0xE2;

sbit P00 = P0^0;
sbit P01 = P0^1;
sbit P02 = P0^2;
sbit P03 = P0^3;
sbit P04 = P0^4;
sbit P05 = P0^5;
sbit P06 = P0^6;
sbit P07 = P0^7;
sbit P10 = P1^0;
sbit P11 = P1^1;
sbit P12 = P1^2;
sbit P13 = P1^3;
sbit P14 = P1^4;
sbit P15 = P1^5;
sbit P16 = P1^6;
sbit P17 = P1^7;
sbit P20 = P2^0;
sbit P21 = P2^1;
sbit P22 = P2^2;
sbit P23 = P2^3;
sbit P24 = P2^4;
sbit P25 = P2^5;
sbit P26 = P2^6;
sbit P27 = P2^7;
sbit P30 = P3^0;
sbit P31 = P3^1;
sbit P32 = P3^2;
sbit P33 = P3^3;
sbit P34 = P3^4;
sbit P35 = P3^5;
sbit P36 = P3^6;
sbit P37 = P3^7;
sbit P40 = P4^0;
sbit P41 = P4^1;
sbit P42 = P4^2;
sbit P43 = P4^3;
sbit P44 = P4^4;
sbit P45 = P4^5;
sbit P46 = P4^6;
sbit P47 = P4^7;
sbit P50 = P5^0;
sbit P51 = P5^1;
sbit P52 = P5^2;
sbit P53 = P5^3;
sbit P54 = P5^4;
sbit P55 = P5^5;
sbit P56 = P5^6;
sbit P57 = P5^7;

/****************************** �û������ ***********************************/

#define Timer0_Reload   (65536UL -(MAIN_Fosc / 1000))       //Timer 0 �ж�Ƶ��, 1000��/��

#define PWMA_ENO     (*(unsigned char  volatile xdata *)  0xFEB1)
#define PWMA_PS      (*(unsigned char  volatile xdata *)  0xFEB2)

#define PWMA_CR1     (*(unsigned char  volatile xdata *)  0xFEC0)
#define PWMA_CR2     (*(unsigned char  volatile xdata *)  0xFEC1)
#define PWMA_SMCR    (*(unsigned char  volatile xdata *)  0xFEC2) 
#define PWMA_ETR     (*(unsigned char  volatile xdata *)  0xFEC3) 
#define PWMA_IER     (*(unsigned char  volatile xdata *)  0xFEC4) 
#define PWMA_SR1     (*(unsigned char  volatile xdata *)  0xFEC5) 
#define PWMA_SR2     (*(unsigned char  volatile xdata *)  0xFEC6) 
#define PWMA_EGR     (*(unsigned char  volatile xdata *)  0xFEC7) 
#define PWMA_CCMR1   (*(unsigned char  volatile xdata *)  0xFEC8) 
#define PWMA_CCMR2   (*(unsigned char  volatile xdata *)  0xFEC9)
#define PWMA_CCMR3   (*(unsigned char  volatile xdata *)  0xFECA)
#define PWMA_CCMR4   (*(unsigned char  volatile xdata *)  0xFECB)
#define PWMA_CCER1   (*(unsigned char  volatile xdata *)  0xFECC)
#define PWMA_CCER2   (*(unsigned char  volatile xdata *)  0xFECD)
#define PWMA_CNTRH   (*(unsigned char  volatile xdata *)  0xFECE)
#define PWMA_CNTRL   (*(unsigned char  volatile xdata *)  0xFECF)
#define PWMA_PSCRH   (*(unsigned char  volatile xdata *)  0xFED0)
#define PWMA_PSCRL   (*(unsigned char  volatile xdata *)  0xFED1)
#define PWMA_ARRH    (*(unsigned char  volatile xdata *)  0xFED2)
#define PWMA_ARRL    (*(unsigned char  volatile xdata *)  0xFED3)
#define PWMA_RCR     (*(unsigned char  volatile xdata *)  0xFED4)
#define PWMA_CCR1H   (*(unsigned char  volatile xdata *)  0xFED5)
#define PWMA_CCR1L   (*(unsigned char  volatile xdata *)  0xFED6)
#define PWMA_CCR2H   (*(unsigned char  volatile xdata *)  0xFED7)
#define PWMA_CCR2L   (*(unsigned char  volatile xdata *)  0xFED8)
#define PWMA_CCR3H   (*(unsigned char  volatile xdata *)  0xFED9)
#define PWMA_CCR3L   (*(unsigned char  volatile xdata *)  0xFEDA)
#define PWMA_CCR4H   (*(unsigned char  volatile xdata *)  0xFEDB)
#define PWMA_CCR4L   (*(unsigned char  volatile xdata *)  0xFEDC)
#define PWMA_BKR     (*(unsigned char  volatile xdata *)  0xFEDD)
#define PWMA_DTR     (*(unsigned char  volatile xdata *)  0xFEDE)
#define PWMA_OISR    (*(unsigned char  volatile xdata *)  0xFEDF)

/*****************************************************************************/

#define PWM1_1      0x00	//P:P1.0  N:P1.1
#define PWM1_2      0x01	//P:P2.0  N:P2.1
#define PWM1_3      0x02	//P:P6.0  N:P6.1

#define PWM2_1      0x00	//P:P1.2/P5.4  N:P1.3
#define PWM2_2      0x04	//P:P2.2  N:P2.3
#define PWM2_3      0x08	//P:P6.2  N:P6.3

#define PWM3_1      0x00	//P:P1.4  N:P1.5
#define PWM3_2      0x10	//P:P2.4  N:P2.5
#define PWM3_3      0x20	//P:P6.4  N:P6.5

#define PWM4_1      0x00	//P:P1.6  N:P1.7
#define PWM4_2      0x40	//P:P2.6  N:P2.7
#define PWM4_3      0x80	//P:P6.6  N:P6.7
#define PWM4_4      0xC0	//P:P3.4  N:P3.3

#define ENO1P       0x01
#define ENO1N       0x02
#define ENO2P       0x04
#define ENO2N       0x08
#define ENO3P       0x10
#define ENO3N       0x20
#define ENO4P       0x40
#define ENO4N       0x80

/*************  ���ر�������    **************/
bit B_1ms;          //1ms��־

u8 cnt50ms;
u8  KeyCode;    //���û�ʹ�õļ���, 1~16��Ч
u8 IO_KeyState, IO_KeyState1, IO_KeyHoldCnt;    //���м��̱���

void IO_KeyScan(void);   //50ms call
void  delay_ms(u8 ms);
/******************** ������ **************************/
void main(void)
{
    P0M1 = 0x00;   P0M0 = 0x00;   //����Ϊ׼˫���
    P1M1 = 0x00;   P1M0 = 0x00;   //����Ϊ׼˫���
    P2M1 = 0x00;   P2M0 = 0x00;   //����Ϊ׼˫���
    P3M1 = 0x00;   P3M0 = 0x00;   //����Ϊ׼˫���
    P4M1 = 0x00;   P4M0 = 0x00;   //����Ϊ׼˫���
    P5M1 = 0x00;   P5M0 = 0x00;   //����Ϊ׼˫���
    P6M1 = 0x00;   P6M0 = 0x00;   //����Ϊ׼˫���
    P7M1 = 0x00;   P7M0 = 0x00;   //����Ϊ׼˫���

    //  Timer0��ʼ��
    AUXR = 0x80;    //Timer0 set as 1T, 16 bits timer auto-reload, 
    TH0 = (u8)(Timer0_Reload / 256);
    TL0 = (u8)(Timer0_Reload % 256);
    ET0 = 1;    //Timer0 interrupt enable
    TR0 = 1;    //Tiner0 run

		P_SW2 |= 0x80;
		
    PWMA_CCER2 = 0x00;
    PWMA_CCMR4 = 0x30;
    PWMA_CCER2 = 0x50;

    PWMA_ARRH = 0x07; //��������ʱ��
    PWMA_ARRL = 0xff;

    PWMA_ENO = 0x00;
    PWMA_ENO |= ENO4N; //ʹ�����

		PWMA_PS = 0x00;  //�߼� PWM ͨ�������ѡ��λ
    PWMA_PS |= PWM4_2; //ѡ�� PWM4_2 ͨ��

    PWMA_BKR = 0x80; //ʹ�������
    PWMA_CR1 |= 0x01; //��ʼ��ʱ

    delay_ms(250);
    delay_ms(250);
    PWMA_ENO = 0x00;
		
    P_SW2 &= 0x7f;
    EA = 1;     //�����ж�

    while (1)
    {
        if(B_1ms)   //1ms��
        {
            B_1ms = 0;
            if(++cnt50ms >= 50)     //50msɨ��һ�����м���
            {
                cnt50ms = 0;
                IO_KeyScan();
            }
            
            if(KeyCode > 0)     //�м�����
            {
                P_SW2 |= 0x80;
                PWMA_ENO |= ENO4N; //ʹ�����
                delay_ms(250);
                PWMA_ENO = 0x00;
                P_SW2 &= 0x7f;
                KeyCode = 0;
            }
        }
		}
}

//========================================================================
// ����: void  delay_ms(u8 ms)
// ����: ��ʱ������
// ����: ms,Ҫ��ʱ��ms��, ����ֻ֧��1~255ms. �Զ���Ӧ��ʱ��.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void  delay_ms(u8 ms)
{
     u16 i;
     do{
          i = MAIN_Fosc / 13000;
          while(--i);   //14T per loop
     }while(--ms);
}


/********************** Timer0 1ms�жϺ��� ************************/
void timer0(void) interrupt 1
{
    B_1ms = 1;      //1ms��־
}


/*****************************************************
    ���м�ɨ�����
    ʹ��XY����4x4���ķ�����ֻ�ܵ������ٶȿ�

   Y     P04      P05      P06      P07
          |        |        |        |
X         |        |        |        |
P00 ---- K00 ---- K01 ---- K02 ---- K03 ----
          |        |        |        |
P01 ---- K04 ---- K05 ---- K06 ---- K07 ----
          |        |        |        |
P02 ---- K08 ---- K09 ---- K10 ---- K11 ----
          |        |        |        |
P03 ---- K12 ---- K13 ---- K14 ---- K15 ----
          |        |        |        |
******************************************************/


u8 code T_KeyTable[16] = {0,1,2,0,3,0,0,0,4,0,0,0,0,0,0,0};

void IO_KeyDelay(void)
{
    u8 i;
    i = 60;
    while(--i)  ;
}

void IO_KeyScan(void)    //50ms call
{
    u8  j;

    j = IO_KeyState1;   //������һ��״̬

    P0 = 0xf0;  //X�ͣ���Y
    IO_KeyDelay();
    IO_KeyState1 = P0 & 0xf0;

    P0 = 0x0f;  //Y�ͣ���X
    IO_KeyDelay();
    IO_KeyState1 |= (P0 & 0x0f);
    IO_KeyState1 ^= 0xff;   //ȡ��
    
    if(j == IO_KeyState1)   //�������ζ����
    {
        j = IO_KeyState;
        IO_KeyState = IO_KeyState1;
        if(IO_KeyState != 0)    //�м�����
        {
            F0 = 0;
            if(j == 0)  F0 = 1; //��һ�ΰ���
            else if(j == IO_KeyState)
            {
                if(++IO_KeyHoldCnt >= 20)   //1����ؼ�
                {
                    IO_KeyHoldCnt = 18;
                    F0 = 1;
                }
            }
            if(F0)
            {
                j = T_KeyTable[IO_KeyState >> 4];
                if((j != 0) && (T_KeyTable[IO_KeyState& 0x0f] != 0)) 
                    KeyCode = (j - 1) * 4 + T_KeyTable[IO_KeyState & 0x0f] + 16;    //������룬17~32
            }
        }
        else    IO_KeyHoldCnt = 0;
    }
    P0 = 0xff;
}

