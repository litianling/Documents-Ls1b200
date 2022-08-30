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

高级PWM定时器 PWM1P/PWM1N,PWM2P/PWM2N,PWM3P/PWM3N,PWM4P/PWM4N 每个通道都可独立实现PWM输出，或者两两互补对称输出.

演示使用PWM1P,PWM1N产生互补的SPWM.

主时钟选择24MHZ, PWM时钟选择1T, PWM周期2400, 死区12个时钟(0.5us).正弦波表用200点. 

输出正弦波频率 = 24000000 / 2400 / 200 = 50 HZ.

本程序仅仅是一个SPWM的演示程序, 用户可以通过上面的计算方法修改PWM周期和正弦波的点数和幅度.

本程序输出频率固定, 如果需要变频, 请用户自己设计变频方案.

本程序从P6.0(PWM1P)输出正相脉冲, 从P6.1(PWM1N)输出反相脉冲(互补).

******************************************/

#include    "reg51.h"       //包含此头文件后，里面声明的寄存器不需要再手动输入，避免重复定义
#include    "intrins.h"
#include    "T_SineTable.h"

#define     MAIN_Fosc       24000000L   //定义主时钟

typedef     unsigned char   u8;
typedef     unsigned int    u16;
typedef     unsigned long   u32;

//手动输入声明"reg51.h"头文件里面没有定义的寄存器
sfr TH2  = 0xD6;
sfr TL2  = 0xD7;
sfr IE2  = 0xAF;
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

/*************  本地变量声明    **************/

u16 PWM1_Duty;
u8	PWM_Index;	//SPWM查表索引

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

    PWM1_Duty = 1220;

    P_SW2 |= 0x80;
	
    PWMA_CCER1 = 0x00; //写 CCMRx 前必须先清零 CCxE 关闭通道
    PWMA_CCER2 = 0x00;
    PWMA_CCMR1 = 0x60; //通道模式配置
//    PWMA_CCMR2 = 0x60;
//    PWMA_CCMR3 = 0x60;
//    PWMA_CCMR4 = 0x60;
    PWMA_CCER1 = 0x05; //配置通道输出使能和极性
//    PWMA_CCER2 = 0x55;

    PWMA_ARRH = 0x09; //设置周期时间
    PWMA_ARRL = 0x60;

    PWMA_CCR1H = (u8)(PWM1_Duty >> 8); //设置占空比时间
    PWMA_CCR1L = (u8)(PWM1_Duty);

    PWMA_DTR = 0x0C;   //设置死区时间

    PWMA_ENO = 0x00;
    PWMA_ENO |= ENO1P; //使能输出
    PWMA_ENO |= ENO1N; //使能输出
//    PWMA_ENO |= ENO2P; //使能输出
//    PWMA_ENO |= ENO2N; //使能输出
//    PWMA_ENO |= ENO3P; //使能输出
//    PWMA_ENO |= ENO3N; //使能输出
//    PWMA_ENO |= ENO4P; //使能输出
//    PWMA_ENO |= ENO4N; //使能输出

    PWMA_PS = 0x00;  //高级 PWM 通道输出脚选择位
    PWMA_PS |= PWM1_3; //选择 PWM1_3 通道
//    PWMA_PS |= PWM2_3; //选择 PWM2_3 通道
//    PWMA_PS |= PWM3_3; //选择 PWM3_3 通道
//    PWMA_PS |= PWM4_3; //选择 PWM4_3 通道

    PWMA_BKR = 0x80; //使能主输出
    PWMA_IER = 0x01; //使能中断
    PWMA_CR1 |= 0x01; //开始计时

    P_SW2 &= 0x7f;

    EA = 1;     //打开总中断

    while (1)
    {
    }
}

/******************** 中断函数 **************************/
void PWMA_ISR() interrupt 26 
{ 
    P_SW2 |= 0x80;
    if(PWMA_SR1 & 0X01)
    {
        PWMA_SR1 &=~0X01; 
        PWM1_Duty = T_SinTable[PWM_Index];
        if(++PWM_Index >= 200)	PWM_Index = 0;

        PWMA_CCR1H = (u8)(PWM1_Duty >> 8); //设置占空比时间
        PWMA_CCR1L = (u8)(PWM1_Duty);
    }
    PWMA_SR1 = 0;
    P_SW2 &= 0x7f;
}
