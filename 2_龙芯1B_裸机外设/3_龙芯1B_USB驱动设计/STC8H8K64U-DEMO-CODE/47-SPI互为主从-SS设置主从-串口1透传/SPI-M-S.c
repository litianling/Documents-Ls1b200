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

本例程基于STC8H8K64U为主控芯片的实验箱8进行编写测试，STC8G、STC8H系列芯片可通用参考.

通过串口发送数据给MCU1，MCU1将接收到的数据由SPI发送给MCU2，MCU2再通过串口发送出去.

设置方法 1：
两个设备初始化时都设置 SSIG 为 0，MSTR 设置为0，且将 SS 脚设置为双向口模式输出高电平。
此时两个设备都是不忽略 SS 的从机模式。
当其中一个设备需要启动传输时，MSTR 设置为1，且通过 IO 口输出低电平，拉低对方的 SS 脚，
这样另一个设备就被强行设置为从机模式。

         MCU1                          MCU2
  |-----------------|           |-----------------|
  |            MISO |-----------| MISO            |
--| TX         MOSI |-----------| MOSI         TX |--
  |            SCLK |-----------| SCLK            |
--| RX         GPIO |-----------| SS           RX |--
  |              SS |-----------| GPIO            |
  |-----------------|           |-----------------|


下载时, 选择时钟 22.1184MHz (用户可自行修改频率).

******************************************/

#include    "reg51.h"       //包含此头文件后，里面声明的寄存器不需要再手动输入，避免重复定义
#include    "intrins.h"

#define     MAIN_Fosc       22118400L   //定义主时钟（精确计算115200波特率）

typedef     unsigned char   u8;
typedef     unsigned int    u16;
typedef     unsigned long   u32;

//手动输入声明"reg51.h"头文件里面没有定义的寄存器
sfr TH2  = 0xD6;
sfr TL2  = 0xD7;
sfr IE2   = 0xAF;
#define ESPI 0x02

sfr INT_CLKO = 0x8F;
sfr AUXR = 0x8E;
sfr AUXR1 = 0xA2;
sfr P_SW1 = 0xA2;
sfr P_SW2 = 0xBA;
sfr S2CON = 0x9A;
sfr S2BUF = 0x9B;

sfr SPSTAT = 0xCD;
sfr SPCTL  = 0xCE;
sfr SPDAT  = 0xCF;
#define SPIF    0x80        //SPI传输完成标志。写入1清0。
#define WCOL    0x40        //SPI写冲突标志。写入1清0。

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


/*************  本地常量声明    **************/

sbit    SPI_SS  = P2^2;
sbit    SPI_SI  = P2^3;
sbit    SPI_SO  = P2^4;
sbit    SPI_SCK = P2^5;
sbit    SPI_CE  = P1^0;

#define Baudrate1           115200L
#define BUF_LENGTH          128

#define UART1_BUF_LENGTH    (BUF_LENGTH)   //串口缓冲长度
#define SPI_BUF_LENGTH      (BUF_LENGTH)   //SPI缓冲长度

/*************  本地变量声明    **************/

u8  RX1_TimeOut;
u8  SPI_TimeOut;
u8  SPI_Cnt;    //SPI计数
u8  RX1_Cnt;    //UART计数
bit B_TX1_Busy; //发送忙标志
bit B_SPI_Busy; //发送忙标志

u8  xdata   RX1_Buffer[UART1_BUF_LENGTH];   //接收缓冲
u8  xdata   SPI_Buffer[SPI_BUF_LENGTH];     //接收缓冲


void    delay_ms(u8 ms);
void    UART1_config(u8 brt);   // 选择波特率, 2: 使用Timer2做波特率, 其它值: 使用Timer1做波特率.
void    PrintString1(u8 *puts);
void    UART1_TxByte(u8 dat);

void    SPI_init(void);
void    SPI_WriteByte(u8 out);

/******************** 主函数 **************************/
void main(void)
{
    u8 i;

    P0M1 = 0x00;   P0M0 = 0x00;   //设置为准双向口
    P1M1 = 0x00;   P1M0 = 0x00;   //设置为准双向口
    P2M1 = 0x00;   P2M0 = 0x00;   //设置为准双向口
    P3M1 = 0x00;   P3M0 = 0x00;   //设置为准双向口
    P4M1 = 0x00;   P4M0 = 0x00;   //设置为准双向口
    P5M1 = 0x00;   P5M0 = 0x00;   //设置为准双向口
    P6M1 = 0x00;   P6M0 = 0x00;   //设置为准双向口
    P7M1 = 0x00;   P7M0 = 0x00;   //设置为准双向口

    UART1_config(1);    // 选择波特率, 2: 使用Timer2做波特率, 其它值: 使用Timer1做波特率.
    SPI_init();
    EA = 1;     //允许总中断
    
    PrintString1("UART1与SPI透传程序-SPI互为主从设置方法1.\r\n");

    while (1)
    {
        delay_ms(1);

        if(RX1_TimeOut > 0)
        {
            if(--RX1_TimeOut == 0)  //超时,则串口接收结束
            {
                if(RX1_Cnt > 0)
                {
                    SPCTL = 0x50;   //使能 SPI 主机模式
                    SPI_CE = 0;     //拉低从机 SS 管脚
                    delay_ms(1);
                    for(i=0;i<RX1_Cnt;i++)
                    {
                        SPI_WriteByte(RX1_Buffer[i]); //发送串口数据
                    }
                    SPI_CE = 1;    //拉高从机的 SS 管脚
                    SPCTL = 0x40;  //重新设置为从机待机
                    RX1_Cnt = 0;
                }
            }
        }
				
        if(SPI_TimeOut > 0)
        {
            if(--SPI_TimeOut == 0)  //超时,则SPI接收结束
            {
                if(SPI_Cnt > 0)
                {
                    for(i=0;i<SPI_Cnt;i++)
                    {
                        UART1_TxByte(SPI_Buffer[i]); //发送SPI数据
                    }
                    SPI_Cnt = 0;
                }
            }
        }
    }
}


//========================================================================
// 函数: void  delay_ms(unsigned char ms)
// 描述: 延时函数。
// 参数: ms,要延时的ms数, 这里只支持1~255ms. 自动适应主时钟.
// 返回: none.
// 版本: VER1.0
// 日期: 2013-4-1
// 备注: 
//========================================================================
void delay_ms(u8 ms)
{
    u16 i;
    do{
        i = MAIN_Fosc / 13000;
        while(--i)    ;   //14T per loop
    }while(--ms);
}

//========================================================================
// 函数: void UART1_TxByte(u8 dat)
// 描述: 发送一个字节.
// 参数: 无.
// 返回: 无.
// 版本: V1.0, 2014-6-30
//========================================================================

void UART1_TxByte(u8 dat)
{
    SBUF = dat;
    B_TX1_Busy = 1;
    while(B_TX1_Busy);
}

//========================================================================
// 函数: void PrintString1(u8 *puts)
// 描述: 串口1发送字符串函数。
// 参数: puts:  字符串指针.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-11-28
// 备注: 
//========================================================================
void PrintString1(u8 *puts) //发送一个字符串
{
    for (; *puts != 0;  puts++) UART1_TxByte(*puts);    //遇到停止符0结束
}

//========================================================================
// 函数: SetTimer2Baudraye(u16 dat)
// 描述: 设置Timer2做波特率发生器。
// 参数: dat: Timer2的重装值.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-11-28
// 备注: 
//========================================================================
void SetTimer2Baudraye(u16 dat)  // 选择波特率, 2: 使用Timer2做波特率, 其它值: 使用Timer1做波特率.
{
    AUXR &= ~(1<<4);    //Timer stop
    AUXR &= ~(1<<3);    //Timer2 set As Timer
    AUXR |=  (1<<2);    //Timer2 set as 1T mode
    TH2 = dat / 256;
    TL2 = dat % 256;
    IE2  &= ~(1<<2);    //禁止中断
    AUXR |=  (1<<4);    //Timer run enable
}

//========================================================================
// 函数: void UART1_config(u8 brt)
// 描述: UART1初始化函数。
// 参数: brt: 选择波特率, 2: 使用Timer2做波特率, 其它值: 使用Timer1做波特率.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-11-28
// 备注: 
//========================================================================
void UART1_config(u8 brt)    // 选择波特率, 2: 使用Timer2做波特率, 其它值: 使用Timer1做波特率.
{
    /*********** 波特率使用定时器2 *****************/
    if(brt == 2)
    {
        AUXR |= 0x01;       //S1 BRT Use Timer2;
        SetTimer2Baudraye(65536UL - (MAIN_Fosc / 4) / Baudrate1);
    }

    /*********** 波特率使用定时器1 *****************/
    else
    {
        TR1 = 0;
        AUXR &= ~0x01;      //S1 BRT Use Timer1;
        AUXR |=  (1<<6);    //Timer1 set as 1T mode
        TMOD &= ~(1<<6);    //Timer1 set As Timer
        TMOD &= ~0x30;      //Timer1_16bitAutoReload;
        TH1 = (u8)((65536UL - (MAIN_Fosc / 4) / Baudrate1) / 256);
        TL1 = (u8)((65536UL - (MAIN_Fosc / 4) / Baudrate1) % 256);
        ET1 = 0;    //禁止中断
        INT_CLKO &= ~0x02;  //不输出时钟
        TR1  = 1;
    }
    /*************************************************/

    SCON = (SCON & 0x3f) | 0x40;    //UART1模式, 0x00: 同步移位输出, 0x40: 8位数据,可变波特率, 0x80: 9位数据,固定波特率, 0xc0: 9位数据,可变波特率
//  PS  = 1;    //高优先级中断
    ES  = 1;    //允许中断
    REN = 1;    //允许接收
    P_SW1 &= 0x3f;
    P_SW1 |= 0x00;      //UART1 switch to, 0x00: P3.0 P3.1, 0x40: P3.6 P3.7, 0x80: P1.6 P1.7, 0xC0: P4.3 P4.4

    RX1_TimeOut = 0;
    B_TX1_Busy = 0;
    RX1_Cnt = 0;
}

//========================================================================
// 函数: void UART1_int (void) interrupt UART1_VECTOR
// 描述: UART1中断函数。
// 参数: nine.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-11-28
// 备注: 
//========================================================================
void UART1_int (void) interrupt 4
{
    if(RI)
    {
        RI = 0;
        if(RX1_Cnt >= UART1_BUF_LENGTH) RX1_Cnt = 0;
        RX1_Buffer[RX1_Cnt] = SBUF;
        RX1_Cnt++;
        RX1_TimeOut = 5;
    }

    if(TI)
    {
        TI = 0;
        B_TX1_Busy = 0;
    }
}

/************************************************************************/

void SPI_Isr() interrupt 9 
{ 
    SPSTAT = 0xc0; //清中断标志
    if (SPCTL & 0x10) 
    { //主机模式
        B_SPI_Busy = 0;
    }
    else 
    { //从机模式
        if(SPI_Cnt >= SPI_BUF_LENGTH) SPI_Cnt = 0;
        SPI_Buffer[SPI_Cnt] = SPDAT;
        SPI_Cnt++;
        SPI_TimeOut = 10;
    }
}

void SPI_init(void)
{
    SPI_SS = 1;
    SPI_CE = 1;
    SPCTL = 0x40;           //使能 SPI 从机模式进行待机
    AUXR1 = (AUXR1 & ~(3<<2)) | (1<<2);  //IO口切换. 0: P1.2/P5.4 P1.3 P1.4 P1.5, 1: P2.2 P2.3 P2.4 P2.5, 2: P5.4 P4.0 P4.1 P4.3, 3: P3.5 P3.4 P3.3 P3.2
    IE2 |= ESPI;            //使能 SPI 中断
    SPSTAT = SPIF + WCOL;   //清0 SPIF和WCOL标志

    SPI_TimeOut = 0;
    B_SPI_Busy = 0;
    SPI_Cnt = 0;
}

/************************************************************************/
void SPI_WriteByte(u8 out)
{
    SPDAT = out;
    B_SPI_Busy = 1;
    while(B_SPI_Busy) ;
}

