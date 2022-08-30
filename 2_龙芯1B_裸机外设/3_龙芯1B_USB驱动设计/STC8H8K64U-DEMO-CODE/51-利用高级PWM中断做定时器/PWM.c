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

利用高级PWMA/PWMB中断实现定时器功能.

定时周期 = ((PWMx_PSCR + 1) * (PWMx_ARR + 1)) / SYSclk.

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
sbit P60 = P6^0;
sbit P61 = P6^1;
sbit P62 = P6^2;
sbit P63 = P6^3;
sbit P64 = P6^4;
sbit P65 = P6^5;
sbit P66 = P6^6;
sbit P67 = P6^7;

/****************************** 用户定义宏 ***********************************/

#define PWMA_ENO     (*(unsigned char  volatile xdata *)  0xFEB1)
#define PWMA_PS      (*(unsigned char  volatile xdata *)  0xFEB2)
#define PWM2_ENO     (*(unsigned char  volatile xdata *)  0xFEB5)
#define PWM2_PS      (*(unsigned char  volatile xdata *)  0xFEB6)                               

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
#define PWMA_PSCR    (*(unsigned int   volatile xdata *)  0xFED0)
#define PWMA_PSCRH   (*(unsigned char  volatile xdata *)  0xFED0)
#define PWMA_PSCRL   (*(unsigned char  volatile xdata *)  0xFED1)
#define PWMA_ARR     (*(unsigned int   volatile xdata *)  0xFED2)
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

#define PWMB_CR1     (*(unsigned char  volatile xdata *)  0xFEE0)
#define PWMB_CR2     (*(unsigned char  volatile xdata *)  0xFEE1)
#define PWMB_SMCR    (*(unsigned char  volatile xdata *)  0xFEE2)
#define PWMB_ETR     (*(unsigned char  volatile xdata *)  0xFEE3)
#define PWMB_IER     (*(unsigned char  volatile xdata *)  0xFEE4)
#define PWMB_SR1     (*(unsigned char  volatile xdata *)  0xFEE5)
#define PWMB_SR2     (*(unsigned char  volatile xdata *)  0xFEE6)
#define PWMB_EGR     (*(unsigned char  volatile xdata *)  0xFEE7)
#define PWMB_CCMR1   (*(unsigned char  volatile xdata *)  0xFEE8)
#define PWMB_CCMR2   (*(unsigned char  volatile xdata *)  0xFEE9)
#define PWMB_CCMR3   (*(unsigned char  volatile xdata *)  0xFEEA)
#define PWMB_CCMR4   (*(unsigned char  volatile xdata *)  0xFEEB)
#define PWMB_CCER1   (*(unsigned char  volatile xdata *)  0xFEEC)
#define PWMB_CCER2   (*(unsigned char  volatile xdata *)  0xFEED)
#define PWMB_CNTRH   (*(unsigned char  volatile xdata *)  0xFEEE)
#define PWMB_CNTRL   (*(unsigned char  volatile xdata *)  0xFEEF)
#define PWMB_PSCR    (*(unsigned int   volatile xdata *)  0xFEF0)
#define PWMB_PSCRH   (*(unsigned char  volatile xdata *)  0xFEF0)
#define PWMB_PSCRL   (*(unsigned char  volatile xdata *)  0xFEF1)
#define PWMB_ARR     (*(unsigned int   volatile xdata *)  0xFEF2)
#define PWMB_ARRH    (*(unsigned char  volatile xdata *)  0xFEF2)
#define PWMB_ARRL    (*(unsigned char  volatile xdata *)  0xFEF3)
#define PWMB_RCR     (*(unsigned char  volatile xdata *)  0xFEF4)
#define PWMB_CCR1H   (*(unsigned char  volatile xdata *)  0xFEF5)
#define PWMB_CCR1L   (*(unsigned char  volatile xdata *)  0xFEF6)
#define PWMB_CCR2H   (*(unsigned char  volatile xdata *)  0xFEF7)
#define PWMB_CCR2L   (*(unsigned char  volatile xdata *)  0xFEF8)
#define PWMB_CCR3H   (*(unsigned char  volatile xdata *)  0xFEF9)
#define PWMB_CCR3L   (*(unsigned char  volatile xdata *)  0xFEFA)
#define PWMB_CCR4H   (*(unsigned char  volatile xdata *)  0xFEFB)
#define PWMB_CCR4L   (*(unsigned char  volatile xdata *)  0xFEFC)
#define PWMB_BKR     (*(unsigned char  volatile xdata *)  0xFEFD)
#define PWMB_DTR     (*(unsigned char  volatile xdata *)  0xFEFE)
#define PWMB_OISR    (*(unsigned char  volatile xdata *)  0xFEFF)

/*****************************************************************************/

void PWMA_Timer(u32 timer);
void PWMB_Timer(u32 timer);

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

	PWMA_Timer(1000);		//1ms
	PWMB_Timer(5000000);	//5s

	P40 = 0;		//给LED供电
	EA = 1;     //打开总中断

	while (1);
}

//========================================================================
// 函数: void PWMA_Timer(u32 timer)
// 描述: PWMA配置函数.
// 参数: None
// 返回: none.
// 版本: V1.0, 2021-02-08
//========================================================================
void PWMA_Timer(u32 timer)
{
	u16 psc=1;
	P_SW2 |= 0x80;
		
	timer *= (MAIN_Fosc / 1000000);	//timer us
	while(timer > 65535L)
	{
		psc *= 2;
		timer >>= 1;
	}
	timer--;
	psc--;
	
	//定时周期 = ((PWMx_PSCR + 1) * (PWMx_ARR + 1)) / SYSclk
	PWMA_ARR = timer; //设置周期时间
	PWMA_PSCR = psc;  //设置预分频器

	PWMA_IER = 0x01; // 使能中断
	PWMA_CR1 |= 0x01; //开始计时
//	P_SW2 &= 0x7f;
}

//========================================================================
// 函数: void PWMB_Timer(u32 timer)
// 描述: PWMB配置函数.
// 参数: None
// 返回: none.
// 版本: V1.0, 2021-02-08
//========================================================================
void PWMB_Timer(u32 timer)
{
	u16 psc=1;
	P_SW2 |= 0x80;
	
	timer *= (MAIN_Fosc / 1000000);	//timer us
	while(timer > 65535L)
	{
		psc *= 2;
		timer >>= 1;
	}
	timer--;
	psc--;
	
	//定时周期 = ((PWMx_PSCR + 1) * (PWMx_ARR + 1)) / SYSclk
	PWMB_ARR = timer; //设置周期时间
	PWMB_PSCR = psc;  //设置预分频器

	PWMB_IER = 0x01; // 使能中断
	PWMB_CR1 |= 0x01; //开始计时
//	P_SW2 &= 0x7f;
}

//========================================================================
// 函数: PWMA_ISR() interrupt 26
// 描述: PWMA中断函数.
// 参数: None
// 返回: none.
// 版本: V1.0, 2020-12-04
//========================================================================
void PWMA_ISR() interrupt 26
{
	PWMA_SR1 = 0;
	P60 = ~P60;
}

//========================================================================
// 函数: PWMB_ISR() interrupt 27
// 描述: PWMB中断函数.
// 参数: None
// 返回: none.
// 版本: V1.0, 2020-12-04
//========================================================================
void PWMB_ISR() interrupt 27
{
	PWMB_SR1 = 0;
	P64 = ~P64;
}

