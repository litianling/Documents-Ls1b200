/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序        */
/*---------------------------------------------------------------------*/


/*************  功能说明    **************

本例程基于STC8H8K64U为主控芯片的实验箱8进行编写测试，STC8H系列芯片可通用参考.

高级PWM定时器实现高速PWM脉冲输出.

周期/占空比可调, 通过比较/捕获中断进行脉冲个数计数.

通过P6口演示输出,每隔10ms输出一次PWM,计数10个脉冲后停止输出.

定时器每1ms调整PWM周期.

下载时, 选择时钟 24MHZ (用户可自行修改频率).

******************************************/

#include    "reg51.h"       //包含此头文件后，里面声明的寄存器不需要再手动输入，避免重复定义
#include    "intrins.h"

#define     MAIN_Fosc       24000000L   //定义主时钟

typedef     unsigned char   u8;
typedef     unsigned int    u16;
typedef     unsigned long   u32;

//手动输入声明"reg51.h"头文件里面没有定义的寄存器
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

/****************************** 用户定义宏 ***********************************/

#define Timer0_Reload   (65536UL -(MAIN_Fosc / 1000))       //Timer 0 中断频率, 1000次/秒

#define PWMA_ENO     (*(unsigned char  volatile xdata *)  0xFEB1)
#define PWMA_PS      (*(unsigned char  volatile xdata *)  0xFEB2)
#define PWMB_ENO     (*(unsigned char  volatile xdata *)  0xFEB5)
#define PWMB_PS      (*(unsigned char  volatile xdata *)  0xFEB6)                               

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
#define PWMA_CNTR    (*(unsigned int   volatile xdata *)  0xFECE)
#define PWMA_CNTRH   (*(unsigned char  volatile xdata *)  0xFECE)
#define PWMA_CNTRL   (*(unsigned char  volatile xdata *)  0xFECF)
#define PWMA_PSCRH   (*(unsigned char  volatile xdata *)  0xFED0)
#define PWMA_PSCRL   (*(unsigned char  volatile xdata *)  0xFED1)
#define PWMA_ARR     (*(unsigned int   volatile xdata *)  0xFED2)
#define PWMA_ARRH    (*(unsigned char  volatile xdata *)  0xFED2)
#define PWMA_ARRL    (*(unsigned char  volatile xdata *)  0xFED3)
#define PWMA_RCR     (*(unsigned char  volatile xdata *)  0xFED4)
#define PWMA_CCR1    (*(unsigned int   volatile xdata *)  0xFED5)
#define PWMA_CCR1H   (*(unsigned char  volatile xdata *)  0xFED5)
#define PWMA_CCR1L   (*(unsigned char  volatile xdata *)  0xFED6)
#define PWMA_CCR2    (*(unsigned int   volatile xdata *)  0xFED7)
#define PWMA_CCR2H   (*(unsigned char  volatile xdata *)  0xFED7)
#define PWMA_CCR2L   (*(unsigned char  volatile xdata *)  0xFED8)
#define PWMA_CCR3    (*(unsigned int   volatile xdata *)  0xFED9)
#define PWMA_CCR3H   (*(unsigned char  volatile xdata *)  0xFED9)
#define PWMA_CCR3L   (*(unsigned char  volatile xdata *)  0xFEDA)
#define PWMA_CCR4    (*(unsigned int   volatile xdata *)  0xFEDB)
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

/*************  本地变量声明    **************/
bit B_1ms;          //1ms标志
bit PWM1_Flag;

u16 Period;
u8 Counter;
u8 msSecond;

void UpdatePwm(void);
void TxPulse(void);

/******************** 主函数 **************************/
void main(void)
{
    P0M1 = 0x00;   P0M0 = 0x00;   //设置为准双向口
    P1M1 = 0x00;   P1M0 = 0x00;   //设置为准双向口
    P2M1 = 0x00;   P2M0 = 0x00;   //设置为准双向口
    P3M1 = 0x00;   P3M0 = 0x00;   //设置为准双向口
    P4M1 = 0x00;   P4M0 = 0x00;   //设置为准双向口
    P5M1 = 0x00;   P5M0 = 0x00;   //设置为准双向口
    P6M1 = 0x00;   P6M0 = 0x00;   //设置为准双向口
    P7M1 = 0x00;   P7M0 = 0x00;   //设置为准双向口

    PWM1_Flag = 0;
    Counter = 0;
    Period = 0x1000;

    //  Timer0初始化
    AUXR = 0x80;    //Timer0 set as 1T, 16 bits timer auto-reload, 
    TH0 = (u8)(Timer0_Reload / 256);
    TL0 = (u8)(Timer0_Reload % 256);
    ET0 = 1;    //Timer0 interrupt enable
    TR0 = 1;    //Tiner0 run

		P_SW2 |= 0x80;     //使能XFR访问
		
    PWMA_ENO = 0x00;
    PWMA_ENO |= ENO1P; //使能输出

		PWMA_PS = 0x00;    //高级 PWM 通道输出脚选择位
    PWMA_PS |= PWM1_3; //选择 PWM1_3 通道

    UpdatePwm();
    PWMA_BKR = 0x80;   //使能主输出
    PWMA_CR1 |= 0x01;  //开始计时

    P40 = 0;		//给LED供电
    EA = 1;     //打开总中断

    while (1)
    {
        if(B_1ms)
        {
            B_1ms = 0;
            msSecond++;
            if(msSecond >= 10)
            {
                msSecond = 0;
                TxPulse();    //10ms启动一次PWM输出
            }
        }
    }
}

/************* 发送脉冲函数 **************/
void TxPulse(void)
{
    PWMA_CCER1 = 0x00; //写 CCMRx 前必须先清零 CCxE 关闭通道
    PWMA_CCMR1 = 0x60; //设置 PWM1 模式1 输出
    PWMA_CCER1 = 0x01; //使能 CC1E 通道, 高电平有效
    PWMA_SR1 = 0;      //清标志位
    PWMA_CNTR = 0;     //清计数器
    PWMA_IER = 0x02;   //使能捕获/比较 1 中断
}

/********************** Timer0 1ms中断函数 ************************/
void timer0(void) interrupt 1
{
    B_1ms = 1;
    if(PWM1_Flag)
    {
        Period++;  //周期递增
        if(Period >= 0x1000) PWM1_Flag = 0;
    }
    else
    {
        Period--;  //周期递减
        if(Period <= 0x0100) PWM1_Flag = 1;
    }
    UpdatePwm();	//设置周期、占空比
}

/******************* PWM中断函数 ********************/
void PWMA_ISR() interrupt 26
{
    if(PWMA_SR1 & 0X02)
    {
        PWMA_SR1 &=~0X02;    //清标志位
			
        Counter++;
        if(Counter >= 10)      //计数10个脉冲后关闭PWM计数器
        {
            Counter = 0;
            PWMA_CCER1 = 0x00; //写 CCMRx 前必须先清零 CCxE 关闭通道
            PWMA_CCMR1 = 0x40; //设置 PWM1 强制为无效电平
            PWMA_CCER1 = 0x01; //使能 CC1E 通道, 高电平有效
            PWMA_IER = 0x00;   // 关闭中断
        }
    }
}

//========================================================================
// 函数: UpdatePwm(void)
// 描述: 更新PWM周期占空比.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2012-11-22
//========================================================================
void UpdatePwm(void)
{
	  PWMA_ARR = Period;
	  PWMA_CCR1 = (Period >> 1); //设置占空比时间: Period/2
}

