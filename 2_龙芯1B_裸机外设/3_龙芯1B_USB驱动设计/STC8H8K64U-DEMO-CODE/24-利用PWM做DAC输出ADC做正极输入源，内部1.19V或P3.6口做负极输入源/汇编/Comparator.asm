;/*---------------------------------------------------------------------*/
;/* --- STC MCU Limited ------------------------------------------------*/
;/* --- STC 1T Series MCU Demo Programme -------------------------------*/
;/* --- Mobile: (86)13922805190 ----------------------------------------*/
;/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
;/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
;/* --- Web: www.STCMCU.com --------------------------------------------*/
;/* --- Web: www.STCMCUDATA.com ----------------------------------------*/
;/* --- QQ:  800003751 -------------------------------------------------*/
;/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序        */
;/*---------------------------------------------------------------------*/


;*************  功能说明    **************

;本例程基于STC8H8K64U为主控芯片的实验箱8进行编写测试，STC8H系列芯片可通用参考.

;比较器的正极选择 P1.1 口 ADC 的模拟输入通道，

;而负极可以是 P3.6 端口或者是内部 BandGap 经过 OP 后的 REFV 电压(1.19V内部固定比较电压)。

;通过中断或者查询方式读取比较器比较结果，CMP+的电平低于CMP-的电平P47口(LED10)输出低电平，反之输出高电平。


;从P1.7输出16位的PWM, 输出的PWM经过RC滤波成直流电压送P1.1做ADC并用数码管显示出来.

;串口1配置为115200bps, 8,n, 1, 切换到P3.0 P3.1, 下载后就可以直接测试. 通过串口1设置占空比.

;串口命令使用ASCII码的数字，可以设置的值为0~256.

;左边4位数码管显示PWM的占空比值，右边4位数码管显示ADC值。


;实测PWM占空比设置为120以下时，P1.1 口 ADC 的模拟输入CMP+的电平低于CMP-的电平（1.19V），P47口输出低电平(LED10灯亮)，反之输出高电平(LED10灯灭)。

;下载时, 选择时钟 22.1184MHZ (用户可自行修改频率).

;******************************************

;/****************************** 用户定义宏 ***********************************/

Fosc_KHZ    EQU 22118   ;22.118MHZ

STACK_POIRTER   EQU     0D0H    ; 堆栈开始地址

Timer0_Reload   EQU     (65536 - Fosc_KHZ)  ; Timer 0 中断频率, 1000次/秒

DIS_DOT         EQU     020H
DIS_BLACK       EQU     010H
DIS_            EQU     011H

;UART1_Baudrate EQU     (-4608) ;   600bps @ 11.0592MHz
;UART1_Baudrate EQU     (-2304) ;  1200bps @ 11.0592MHz UART1_Baudrate = (MAIN_Fosc / Baudrate)
;UART1_Baudrate EQU     (-1152) ;  2400bps @ 11.0592MHz
;UART1_Baudrate EQU     (-576)  ;  4800bps @ 11.0592MHz
;UART1_Baudrate EQU     (-288)  ;  9600bps @ 11.0592MHz
;UART1_Baudrate EQU     (-144)  ; 19200bps @ 11.0592MHz
;UART1_Baudrate EQU     (-72)   ; 38400bps @ 11.0592MHz
;UART1_Baudrate EQU     (-48)   ; 57600bps @ 11.0592MHz
;UART1_Baudrate EQU     (-24)   ;115200bps @ 11.0592MHz

;UART1_Baudrate EQU     (-7680) ;   600bps @ 18.432MHz
;UART1_Baudrate EQU     (-3840) ;  1200bps @ 18.432MHz
;UART1_Baudrate EQU     (-1920) ;  2400bps @ 18.432MHz
;UART1_Baudrate EQU     (-960)  ;  4800bps @ 18.432MHz
;UART1_Baudrate EQU     (-480)  ;  9600bps @ 18.432MHz
;UART1_Baudrate EQU     (-240)  ; 19200bps @ 18.432MHz
;UART1_Baudrate EQU     (-120)  ; 38400bps @ 18.432MHz
;UART1_Baudrate EQU     (-80)   ; 57600bps @ 18.432MHz
;UART1_Baudrate EQU     (-40)   ;115200bps @ 18.432MHz

;UART1_Baudrate EQU     (-9216) ;   600bps @ 22.1184MHz
;UART1_Baudrate EQU     (-4608) ;  1200bps @ 22.1184MHz
;UART1_Baudrate EQU     (-2304) ;  2400bps @ 22.1184MHz
;UART1_Baudrate EQU     (-1152) ;  4800bps @ 22.1184MHz
;UART1_Baudrate EQU     (-576)  ;  9600bps @ 22.1184MHz
;UART1_Baudrate EQU     (-288)  ; 19200bps @ 22.1184MHz
;UART1_Baudrate EQU     (-144)  ; 38400bps @ 22.1184MHz
;UART1_Baudrate EQU     (-96)   ; 57600bps @ 22.1184MHz
UART1_Baudrate  EQU     (-48)   ;115200bps @ 22.1184MHz

;UART1_Baudrate EQU     (-6912) ; 1200bps @ 33.1776MHz
;UART1_Baudrate EQU     (-3456) ; 2400bps @ 33.1776MHz
;UART1_Baudrate EQU     (-1728) ; 4800bps @ 33.1776MHz
;UART1_Baudrate EQU     (-864)  ; 9600bps @ 33.1776MHz
;UART1_Baudrate EQU     (-432)  ;19200bps @ 33.1776MHz
;UART1_Baudrate EQU     (-216)  ;38400bps @ 33.1776MHz
;UART1_Baudrate EQU     (-144)  ;57600bps @ 33.1776MHz
;UART1_Baudrate EQU     (-72)   ;115200bps @ 33.1776MHz

;*******************************************************************
;*******************************************************************

P_SW2     DATA 0BAH
CMPCR1    DATA 0E6H
CMPCR2    DATA 0E7H
P4        DATA 0C0H
P5        DATA 0C8H
P6        DATA 0E8H
P7        DATA 0F8H

ADC_CONTR   DATA 0BCH   ;带AD系列
ADC_RES     DATA 0BDH   ;带AD系列
ADC_RESL    DATA 0BEH   ;带AD系列
ADCCFG      DATA 0DEH

AUXR        DATA 08EH
INT_CLKO    DATA 0x8F
P_SW1       DATA 0A2H
IE2         DATA 0AFH
T2H         DATA 0D6H
T2L         DATA 0D7H

P0M1    DATA    0x93    ; P0M1.n,P0M0.n     =00--->Standard,    01--->push-pull
P0M0    DATA    0x94    ;                   =10--->pure input,  11--->open drain
P1M1    DATA    0x91    ; P1M1.n,P1M0.n     =00--->Standard,    01--->push-pull
P1M0    DATA    0x92    ;                   =10--->pure input,  11--->open drain
P2M1    DATA    0x95    ; P2M1.n,P2M0.n     =00--->Standard,    01--->push-pull
P2M0    DATA    0x96    ;                   =10--->pure input,  11--->open drain
P3M1    DATA    0xB1    ; P3M1.n,P3M0.n     =00--->Standard,    01--->push-pull
P3M0    DATA    0xB2    ;                   =10--->pure input,  11--->open drain
P4M1    DATA    0xB3    ; P4M1.n,P4M0.n     =00--->Standard,    01--->push-pull
P4M0    DATA    0xB4    ;                   =10--->pure input,  11--->open drain
P5M1    DATA    0xC9    ; P5M1.n,P5M0.n     =00--->Standard,    01--->push-pull
P5M0    DATA    0xCA    ;                   =10--->pure input,  11--->open drain
P6M1    DATA    0xCB    ; P6M1.n,P6M0.n     =00--->Standard,    01--->push-pull
P6M0    DATA    0xCC    ;                   =10--->pure input,  11--->open drain
P7M1    DATA    0xE1    ;
P7M0    DATA    0xE2    ;

ADCTIM        XDATA   0FEA8H

PWMB_ENO      XDATA   0FEB5H
PWMB_PS       XDATA   0FEB6H

PWMB_CR1      XDATA   0FEE0H
PWMB_CR2      XDATA   0FEE1H
PWMB_SMCR     XDATA   0FEE2H
PWMB_ETR      XDATA   0FEE3H
PWMB_IER      XDATA   0FEE4H
PWMB_SR1      XDATA   0FEE5H
PWMB_SR2      XDATA   0FEE6H
PWMB_EGR      XDATA   0FEE7H
PWMB_CCMR1    XDATA   0FEE8H
PWMB_CCMR2    XDATA   0FEE9H
PWMB_CCMR3    XDATA   0FEEAH
PWMB_CCMR4    XDATA   0FEEBH
PWMB_CCER1    XDATA   0FEECH
PWMB_CCER2    XDATA   0FEEDH
PWMB_CNTRH    XDATA   0FEEEH
PWMB_CNTRL    XDATA   0FEEFH
PWMB_PSCRH    XDATA   0FEF0H
PWMB_PSCRL    XDATA   0FEF1H
PWMB_ARRH     XDATA   0FEF2H
PWMB_ARRL     XDATA   0FEF3H
PWMB_RCR      XDATA   0FEF4H
PWMB_CCR1H    XDATA   0FEF5H
PWMB_CCR1L    XDATA   0FEF6H
PWMB_CCR2H    XDATA   0FEF7H
PWMB_CCR2L    XDATA   0FEF8H
PWMB_CCR3H    XDATA   0FEF9H
PWMB_CCR3L    XDATA   0FEFAH
PWMB_CCR4H    XDATA   0FEFBH
PWMB_CCR4L    XDATA   0FEFCH
PWMB_BKR      XDATA   0FEFDH
PWMB_DTR      XDATA   0FEFEH
PWMB_OISR     XDATA   0FEFFH

;*************  IO口定义    **************/


;*************  本地变量声明    **************/
B_1ms           BIT     20H.0   ;   1ms标志

LED8            DATA    30H     ;   显示缓冲 30H ~ 37H
display_index   DATA    38H     ;   显示位索引

msecond_H       DATA    39H     ;
msecond_L       DATA    3AH     ;

RX1_Lenth       EQU     16      ; 串口接收缓冲长度

B_TX1_Busy      BIT     20H.1   ; 发送忙标志
B_RX1_OK        BIT     20H.2   ; 接收结束标志
TX1_Cnt         DATA    3BH     ; 发送计数
RX1_Cnt         DATA    3CH     ; 接收计数
RX1_TimeOut     DATA    3DH     ; 超时计数
RX1_Buffer      DATA    40H     ;40 ~ 4FH 接收缓冲

;*******************************************************************
;*******************************************************************
        ORG     0000H               ;reset
        LJMP    F_Main

        ORG     000BH               ;1  Timer0 interrupt
        LJMP    F_Timer0_Interrupt

        ORG     0023H               ;4  UART1 interrupt
        LJMP    F_UART1_Interrupt

        ORG     00ABH               ;21  CMP interrupt
        LJMP    F_CMP_Interrupt


;******************** 主程序 **************************/
        ORG     0100H       ;reset
F_Main:
    CLR     A
    MOV     P0M1, A     ;设置为准双向口
    MOV     P0M0, A
    MOV     P2M1, A     ;设置为准双向口
    MOV     P2M0, A
    MOV     P3M1, A     ;设置为准双向口
    MOV     P3M0, A
    MOV     P4M1, A     ;设置为准双向口
    MOV     P4M0, A
    MOV     P5M1, A     ;设置为准双向口
    MOV     P5M0, A
    MOV     P6M1, A     ;设置为准双向口
    MOV     P6M0, A
    MOV     P7M1, A     ;设置为准双向口
    MOV     P7M0, A

    MOV     P1M0, A
    MOV     P1M1, #02H  ;设置 P1.1 为 ADC 输入口
    
    MOV     SP, #STACK_POIRTER
    MOV     PSW, #0
    USING   0       ;选择第0组R0~R7

;================= 用户初始化程序 ====================================
    MOV     display_index, #0
    MOV     R0, #LED8
    MOV     R2, #8
L_ClearLoop:
    MOV     @R0, #DIS_BLACK     ;上电消隐
    INC     R0
    DJNZ    R2, L_ClearLoop
    
    CLR     TR0
    ORL     AUXR, #(1 SHL 7)    ; Timer0_1T();
    ANL     TMOD, #NOT 04H      ; Timer0_AsTimer();
    ANL     TMOD, #NOT 03H      ; Timer0_16bitAutoReload();
    MOV     TH0, #Timer0_Reload / 256   ;Timer0_Load(Timer0_Reload);
    MOV     TL0, #Timer0_Reload MOD 256
    SETB    ET0         ; Timer0_InterruptEnable();
    SETB    TR0         ; Timer0_Run();
    SETB    EA          ; 打开总中断
    
    MOV     LED8+7, #1  ;   //显示PWM默认值
    MOV     LED8+6, #2  ;
    MOV     LED8+5, #8  ;
    MOV     LED8+4, #DIS_BLACK  ;   //这位不显示

    LCALL   F_ADC_config    ; ADC初始化
    LCALL   F_PWM_Init      ; PWM初始化
    LCALL   F_CMP_Init
    
    MOV     A, #1
    LCALL   F_UART1_config  ; 选择波特率, 2: 使用Timer2做波特率, 其它值: 使用Timer1做波特率.

    MOV     DPTR, #StringText1  ;Load string address to DPTR
    LCALL   F_SendString1       ;Send string

;=================== 主循环 ==================================
L_Main_Loop:
    JNB     B_1ms,  L_Main_Loop     ;1ms未到
    CLR     B_1ms
    
    MOV     A, RX1_TimeOut
    JNZ     L_CheckRx1TimeOut   ;串口空闲超时计数非0, 
L_QuitCheckRxTimeOut:
    LJMP    L_QuitProcessUart1  ;串口空闲超时计数为0, 退出
L_CheckRx1TimeOut:
    DJNZ    RX1_TimeOut, L_QuitCheckRxTimeOut   ;串口空闲超时计数-1非0退出

    SETB    B_RX1_OK            ;串口空闲超时, 接收结束, 做标志
    MOV     A, RX1_Cnt
    JNZ     L_RxCntNotZero      ;接收到字符数非空, 则处理
    LJMP    L_QuitProcessUart1  ;接收到字符数空, 则退出

L_RxCntNotZero:
    ANL     A, #NOT 3   ;限制为3位数字
    JZ      L_NumberLengthOk
    MOV     DPTR, #StringError3 ;Load string address to DPTR    "错误! 输入字符过多! 输入占空比为0~256!"
    LCALL   F_SendString1       ;Send string
    LJMP    L_QuitCalculatePWM  ;退出
    
L_NumberLengthOk:
    MOV     R2, #0
    MOV     R3, #0
    MOV     R0, #RX1_Buffer
    MOV     R1, RX1_Cnt

L_GetUartPwmLoop:
    MOV     A, @R0
    CLR     C
    SUBB    A, #'0'             ;计算是否小于ASCII数字0
    JNC     L_RxdataLargeThan0
    MOV     DPTR, #StringError1 ;Load string address to DPTR    "错误! 接收到小于0的数字字符! 占空比为0~256!"
    LCALL   F_SendString1       ;Send string
    LJMP    L_QuitCalculatePWM
    
L_RxdataLargeThan0:
    MOV     A, @R0
    CLR     C
    SUBB    A, #03AH            ;计算是否大于ASCII数字9
    JC      L_RxdataLessThan03A
    MOV     DPTR, #StringError4 ;Load string address to DPTR    "错误! 接收到大于9的数字字符! 占空比为0~256!"
    LCALL   F_SendString1       ;Send string
    LJMP    L_QuitCalculatePWM

L_RxdataLessThan03A:
    MOV     AR4, R2
    MOV     AR5, R3
    MOV     R7, #10
    LCALL   F_MUL16x8   ;(R4,R5)* R7 -->(R5,R6,R7)
    MOV     AR2, R6
    MOV     AR3, R7
    MOV     A, @R0      ;计算 [R2 R3] = [R2 R3] * 10 + RX1_Buffer[i] - '0';
    CLR     C
    SUBB    A, #'0'
    ADD     A, R3
    MOV     R3, A
    CLR     A
    ADDC    A, R2
    MOV     R2, A
    INC     R0
    DJNZ    R1, L_GetUartPwmLoop

    MOV     A, R3
    CLR     C
    SUBB    A, #LOW 257     ;if(j >= 257)
    MOV     A, R2
    SUBB    A, #HIGH 257
    JC      L_SetPWM_Right
    MOV     DPTR, #StringError2 ;Load string address to DPTR    错误! 输入占空比过大, 请不要大于256!
    LCALL   F_SendString1       ;Send string
    LJMP    L_QuitCalculatePWM

L_SetPWM_Right:

    LCALL   F_UpdatePwm     ;更新占空比
    MOV     A, R2
    JZ      L_PWM_LessTan256
    MOV     LED8+7, #2      ;占空比为256,显示256
    MOV     LED8+6, #5
    MOV     LED8+5, #6
    SJMP    L_SetPWM_OK
L_PWM_LessTan256:
    MOV     A, R3
    MOV     B, #100     ;显示0~255占空比
    DIV     AB
    MOV     LED8+7, A
    MOV     A, B
    MOV     B, #10
    DIV     AB
    MOV     LED8+6, A
    MOV     LED8+5, B
    MOV     A, LED8+7
    JNZ     L_SetPWM_OK
    MOV     LED8+7, #DIS_BLACK    ;百位消0

L_SetPWM_OK:
    MOV     DPTR, #StringText2  ;Load string address to DPTR    "已更新占空比!"
    LCALL   F_SendString1       ;Send string
L_QuitCalculatePWM:
    MOV     RX1_Cnt, #0
    CLR     B_RX1_OK
L_QuitProcessUart1:

;=================== 检测300ms是否到 ==================================
    INC     msecond_L       ;msecond + 1
    MOV     A, msecond_L
    JNZ     $+4
    INC     msecond_H
    
    CLR     C
    MOV     A, msecond_L    ;msecond - 300
    SUBB    A, #LOW 300
    MOV     A, msecond_H
    SUBB    A, #HIGH 300
    JNC     L_300msIsGood   ;if(msecond < 300), jmp
    LJMP    L_Main_Loop     ;if(msecond == 300), jmp
L_300msIsGood:

;================= 300ms到 ====================================
    MOV     msecond_L, #0   ;if(msecond >= 1000)
    MOV     msecond_H, #0

    MOV     A, #01H
    LCALL   F_Get_ADC12bitResult    ; 读外部电压ADC, 查询方式做一次ADC, 返回值(R6 R7)就是ADC结果, == 4096 为错误

    LCALL   F_HEX2_DEC      ;(R6 R7) HEX Change to DEC ---> (R3 R4 R5), use (R2~R7)
    
    MOV     A, R4           ;显示ADC值
    SWAP    A
    ANL     A, #0x0F
    MOV     LED8+3, A
    MOV     A, R4
    ANL     A, #0x0F
    MOV     LED8+2, A
    MOV     A, R5
    SWAP    A
    ANL     A, #0x0F
    MOV     LED8+1, A
    MOV     A, R5
    ANL     A, #0x0F
    MOV     LED8+0, A

    MOV     A, LED8+3           ;显示消无效0
    JNZ     L_QuitProcessADC
    MOV     LED8+3, #DIS_BLACK
    MOV     A, LED8+2
    JNZ     L_QuitProcessADC
    MOV     LED8+2, #DIS_BLACK
    MOV     A, LED8+1
    JNZ     L_QuitProcessADC
    MOV     LED8+1, #DIS_BLACK
L_QuitProcessADC:

    LJMP    L_Main_Loop

;**********************************************/

;========================================================================
; 函数: F_PWM_Init
; 描述: PWM初始化程序.
; 参数: none
; 返回: none.
; 版本: V1.0, 2021-3-3
;========================================================================
F_PWM_Init:
    ORL     P_SW2, #080H        ;使能访问XFR

    MOV     A,#00H              ;写 CCMRx 前必须先清零 CCxE 关闭通道
    MOV     DPTR,#PWMB_CCER1
    MOVX    @DPTR,A
    MOV     A,#068H             ;设置 CC1 为 PWM模式1 输出
    MOV     DPTR,#PWMB_CCMR1
    MOVX    @DPTR,A
    MOV     A,#01H              ;使能 CC5 通道
    MOV     DPTR,#PWMB_CCER1
    MOVX    @DPTR,A

    MOV     A,#2                ;设置周期时间
    MOV     DPTR,#PWMB_ARRH
    MOVX    @DPTR,A
    MOV     A,#0
    MOV     DPTR,#PWMB_ARRL
    MOVX    @DPTR,A
    MOV     A,#0                ;设置占空比时间
    MOV     DPTR,#PWMB_CCR1H
    MOVX    @DPTR,A
    MOV     A,#128
    MOV     DPTR,#PWMB_CCR1L
    MOVX    @DPTR,A

    MOV     A,#01H              ;使能 PWM5 输出
    MOV     DPTR,#PWMB_ENO
    MOVX    @DPTR,A
    MOV     A,#01H              ;高级 PWM 通道 5 输出脚选择位, 0x00:P2.0, 0x01:P1.7, 0x02:P0.0, 0x03:P7.4
    MOV     DPTR,#PWMB_PS
    MOVX    @DPTR,A
    MOV     A,#080H             ;使能主输出
    MOV     DPTR,#PWMB_BKR
    MOVX    @DPTR,A

    MOV     DPTR,#PWMB_CR1
    MOVX    A,@DPTR
    ORL     A,#01H              ;开始计时
    MOVX    @DPTR,A

    ANL     P_SW2, #NOT 080H    ;禁止访问XFR
    RET

;========================================================================
; 函数: F_CMP_Init
; 描述: CMP初始化程序.
; 参数: none
; 返回: none.
; 版本: V1.0, 2021-3-3
;========================================================================
F_CMP_Init:
    ANL     P_SW2, #NOT 08H     ; 选择P3.4作为比较器输出脚
    CLR     A
    ANL     A, #NOT 080H        ;比较器正向输出
;    ORL     A, #080H            ;比较器反向输出
    ANL     A, #NOT 040H        ;使能0.1us滤波
;    ORL     A, #040H            ;禁止0.1us滤波
;    ANL     A, #NOT 03FH        ;比较器结果直接输出
    ORL     A, #010H            ;比较器结果经过16个去抖时钟后输出
    MOV     CMPCR2, A

    CLR     A
    ORL     A, #030H            ;使能比较器边沿中断
;    ANL     A, #NOT 020H        ;禁止比较器上升沿中断
;    ORL     A, #020H            ;使能比较器上升沿中断
;    ANL     A, #NOT 010H        ;禁止比较器下降沿中断
;    ORL     A, #010H            ;使能比较器下降沿中断
;    ANL     A, #NOT 08H         ;P3.7为CMP+输入脚
    ORL     A, #08H             ;ADC输入脚为CMP+输入教
    ANL     A, #NOT 04H         ;内部参考电压为CMP-输入脚
;    ORL     A, #04H             ;P3.6为CMP-输入脚
;    ANL     A, #NOT 02H         ;禁止比较器输出
    ORL     A, #02H             ;使能比较器输出
    ORL     A, #080H            ;使能比较器模块
    MOV     CMPCR1, A

;    MOV     CMPCR2, #10H        ; 比较器正向输出,使能0.1us滤波,比较器结果经过16个去抖时钟后输出
;    MOV     CMPCR1, #0B2H       ; 使能比较器模块,使能比较器边沿中断,P3.7为CMP+输入脚,内部参考电压为CMP-输入脚,使能比较器输出
    SETB    EA          ; 打开总中断
    
    RET

;========================================================================
; 函数: F_UpdatePwm
; 描述: 更新PWM占空比值. 
; 参数: [R2 R3]: pwm占空比值.
; 返回: none.
; 版本: V1.0, 2021-3-3
;========================================================================
F_UpdatePwm:
    ORL     P_SW2, #080H        ;使能访问XFR

    MOV     A, R2               ;设置占空比时间
    MOV     DPTR, #PWMB_CCR1H
    MOVX    @DPTR, A
    MOV     A, R3
    MOV     DPTR, #PWMB_CCR1L
    MOVX    @DPTR, A

    ANL     P_SW2, #NOT 080H    ;禁止访问XFR
    RET

;***************************************************************************
F_MUL16x8:               ;(R4,R5)* R7 -->(R5,R6,R7)
        MOV   A,R7      ;1T     1
        MOV   B,R5      ;2T     3
        MUL   AB        ;4T  R3*R7  4
        MOV   R6,B      ;1T     4
        XCH   A,R7      ;2T     3
        
        MOV   B,R4      ;1T     3
        MUL   AB        ;4T  R3*R6  4
        ADD   A,R6      ;1T     2
        MOV   R6,A      ;1T     3
        CLR   A         ;1T     1
        ADDC  A,B       ;1T     3
        MOV   R5,A      ;1T     2
        RET             ;4T     10

;//========================================================================
;// 函数: F_HEX2_DEC
;// 描述: 把双字节十六进制数转成十进制BCD数.
;// 参数: (R6 R7): 要转换的双字节十六进制数.
;// 返回: (R3 R4 R5) = BCD码.
;// 版本: V1.0, 2013-10-22
;//========================================================================
F_HEX2_DEC:         ;(R6 R7) HEX Change to DEC ---> (R3 R4 R5), use (R2~R7)
        MOV     R2,#16
        MOV     R3,#0
        MOV     R4,#0
        MOV     R5,#0

L_HEX2_DEC:
        CLR     C   
        MOV     A,R7
        RLC     A   
        MOV     R7,A
        MOV     A,R6
        RLC     A   
        MOV     R6,A

        MOV     A,R5
        ADDC    A,R5
        DA      A   
        MOV     R5,A

        MOV     A,R4
        ADDC    A,R4
        DA      A   
        MOV     R4,A

        MOV     A,R3
        ADDC    A,R3
        DA      A   
        MOV     R3,A

        DJNZ    R2,L_HEX2_DEC
        RET
;**********************************************/

F_ADC_config:
    ORL     P_SW2, #080H        ; 使能访问XFR
    MOV     A,#03FH             ; 设置 ADC 内部时序，ADC采样时间建议设最大值
    MOV     DPTR,#ADCTIM
    MOVX    @DPTR,A
    ANL     P_SW2, #NOT 080H    ; 禁止访问XFR

    MOV     ADCCFG, #02FH       ; 设置转换结果右对齐模式， ADC 时钟为系统时钟/2/16
    MOV     ADC_CONTR, #080H    ; 使能 ADC 模块
    RET

;========================================================================
; 函数: F_Get_ADC12bitResult
; 描述: 查询法读一次ADC结果.
; 参数: ACC: 选择要转换的ADC.
; 返回: (R6 R7) = 12位ADC结果.
; 版本: V1.0, 2020-11-09
;========================================================================
F_Get_ADC12bitResult:   ;ACC - 通道0~7, 查询方式做一次ADC, 返回值(R6 R7)就是ADC结果, == 4096 为错误
    MOV     R7, A           //channel
    MOV     ADC_RES,  #0;
    MOV     ADC_RESL, #0;

    MOV     A, ADC_CONTR        ;ADC_CONTR = (ADC_CONTR & 0xd0) | ADC_START | channel; 
    ANL     A, #0D0H
    ORL     A, #40H
    ORL     A, R7
    MOV     ADC_CONTR, A
    NOP
    NOP
    NOP
    NOP

L_WaitAdcLoop:
    MOV     A, ADC_CONTR
    JNB     ACC.5, L_WaitAdcLoop

    ANL     ADC_CONTR, #NOT 020H    ;清除完成标志

    MOV     A, ADC_RES      ;12位AD结果的高4位放ADC_RES的低4位，低8位在ADC_RESL。
    ANL     A, #0FH
    MOV     R6, A
    MOV     R7, ADC_RESL

L_QuitAdc:
    RET

;====================================================================================
StringText1:
    DB  "PWM和ADC测试程序, 输入占空比为 0~256!",0DH,0AH,0
StringText2:
    DB  "已更新占空比!",0DH,0AH,0
StringError1:
    DB  "错误! 接收到小于0的数字字符! 占空比为0~256!",0DH,0AH,0
StringError2:
    DB  "错误! 输入占空比过大, 请不要大于256!",0DH,0AH,0
StringError3:
    DB  "错误! 输入字符过多! 输入占空比为0~256!",0DH,0AH,0
StringError4:
    DB  "错误! 接收到大于9的数字字符! 占空比为0~256!",0DH,0AH,0


;========================================================================
; 函数: F_SendString1
; 描述: 串口1发送字符串函数。
; 参数: DPTR: 字符串首地址.
; 返回: none.
; 版本: VER1.0
; 日期: 2014-11-28
; 备注: 
;========================================================================
F_SendString1:
    CLR     A
    MOVC    A, @A+DPTR      ;Get current char
    JZ      L_SendEnd1      ;Check the end of the string
    SETB    B_TX1_Busy      ;标志发送忙
    MOV     SBUF, A         ;发送一个字节
    JB      B_TX1_Busy, $   ;等待发送完成
    INC     DPTR            ;increment string ptr
    SJMP    F_SendString1       ;Check next
L_SendEnd1:
    RET

;========================================================================
; 函数: F_SetTimer2Baudraye
; 描述: 设置Timer2做波特率发生器。
; 参数: DPTR: Timer2的重装值.
; 返回: none.
; 版本: VER1.0
; 日期: 2014-11-28
; 备注: 
;========================================================================
F_SetTimer2Baudraye:    ; 选择波特率, 2: 使用Timer2做波特率, 其它值: 使用Timer1做波特率.
    ANL     AUXR, #NOT (1 SHL 4)    ; Timer stop    波特率使用Timer2产生
    ANL     AUXR, #NOT (1 SHL 3)    ; Timer2 set As Timer
    ORL     AUXR, #(1 SHL 2)        ; Timer2 set as 1T mode
    MOV     T2H, DPH
    MOV     T2L, DPL
    ANL     IE2, #NOT (1 SHL 2)     ; 禁止中断
    ORL     AUXR, #(1 SHL 4)        ; Timer run enable
    RET

;========================================================================
; 函数: F_UART1_config
; 描述: UART1初始化函数。
; 参数: ACC: 选择波特率, 2: 使用Timer2做波特率, 其它值: 使用Timer1做波特率.
; 返回: none.
; 版本: VER1.0
; 日期: 2014-11-28
; 备注: 
;========================================================================
F_UART1_config:
    CJNE    A, #2, L_Uart1NotUseTimer2
    ORL     AUXR, #0x01     ; S1 BRT Use Timer2;
    MOV     DPTR, #UART1_Baudrate
    LCALL   F_SetTimer2Baudraye
    SJMP    L_SetupUart1

L_Uart1NotUseTimer2:
    CLR     TR1                 ; Timer Stop    波特率使用Timer1产生
    ANL     AUXR, #NOT 0x01     ; S1 BRT Use Timer1;
    ORL     AUXR, #(1 SHL 6)    ; Timer1 set as 1T mode
    ANL     TMOD, #NOT (1 SHL 6); Timer1 set As Timer
    ANL     TMOD, #NOT 0x30     ; Timer1_16bitAutoReload;
    MOV     TH1, #HIGH UART1_Baudrate
    MOV     TL1, #LOW  UART1_Baudrate
    CLR     ET1                 ; 禁止中断
    ANL     INT_CLKO, #NOT 0x02 ; 不输出时钟
    SETB    TR1

L_SetupUart1:
    SETB    REN     ; 允许接收
    SETB    ES      ; 允许中断

    ANL     SCON, #0x3f
    ORL     SCON, #0x40     ; UART1模式, 0x00: 同步移位输出, 0x40: 8位数据,可变波特率, 0x80: 9位数据,固定波特率, 0xc0: 9位数据,可变波特率
;   SETB    PS      ; 高优先级中断
    SETB    REN     ; 允许接收
    SETB    ES      ; 允许中断

    ANL     P_SW1, #0x3f
    ORL     P_SW1, #0x00        ; UART1 switch to, 0x00: P3.0 P3.1, 0x40: P3.6 P3.7, 0x80: P1.6 P1.7, 0xC0: P4.3 P4.4
;   ORL     PCON2, #(1 SHL 4)   ; 内部短路RXD与TXD, 做中继, ENABLE,DISABLE

    CLR     B_TX1_Busy
    MOV     RX1_Cnt, #0;
    MOV     TX1_Cnt, #0;
    RET


;========================================================================
; 函数: F_UART1_Interrupt
; 描述: UART2中断函数。
; 参数: nine.
; 返回: none.
; 版本: VER1.0
; 日期: 2014-11-28
; 备注: 
;========================================================================
F_UART1_Interrupt:
    PUSH    PSW
    PUSH    ACC
    PUSH    AR0
    
    JNB     RI, L_QuitUartRx
    CLR     RI
    JB      B_RX1_OK, L_QuitUartRx
    MOV     A, RX1_Cnt
    CJNE    A, #RX1_Lenth, L_RxCntNotOver
    MOV     RX1_Cnt, #0     ; 避免溢出处理
L_RxCntNotOver:
    MOV     A, #RX1_Buffer
    ADD     A, RX1_Cnt
    MOV     R0, A
    MOV     @R0, SBUF   ;保存一个字节
    INC     RX1_Cnt
    MOV     RX1_TimeOut, #5
L_QuitUartRx:

    JNB     TI, L_QuitUartTx
    CLR     TI
    CLR     B_TX1_Busy      ; 清除发送忙标志
L_QuitUartTx:

    POP     AR0
    POP     ACC
    POP     PSW
    RETI


; *********************** 显示相关程序 ****************************************
T_Display:                      ;标准字库
;    0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
DB  03FH,006H,05BH,04FH,066H,06DH,07DH,007H,07FH,06FH,077H,07CH,039H,05EH,079H,071H
;  black  -    H    J    K    L    N    o    P    U    t    G    Q    r    M    y
DB  000H,040H,076H,01EH,070H,038H,037H,05CH,073H,03EH,078H,03dH,067H,050H,037H,06EH
;    0.   1.   2.   3.   4.   5.   6.   7.   8.   9.   -1
DB  0BFH,086H,0DBH,0CFH,0E6H,0EDH,0FDH,087H,0FFH,0EFH,046H

T_COM:
DB  001H,002H,004H,008H,010H,020H,040H,080H     ;   位码


;//========================================================================
;// 函数: F_DisplayScan
;// 描述: 显示扫描子程序。
;// 参数: none.
;// 返回: none.
;// 版本: VER1.0
;// 日期: 2013-4-1
;// 备注: 除了ACCC和PSW外, 所用到的通用寄存器都入栈
;//========================================================================
F_DisplayScan:
    PUSH    DPH     ;DPH入栈
    PUSH    DPL     ;DPL入栈
    PUSH    00H     ;R0 入栈
    
    MOV     DPTR, #T_COM
    MOV     A, display_index
    MOVC    A, @A+DPTR
    CPL     A
    MOV     P7,A
    
    MOV     DPTR, #T_Display
    MOV     A, display_index
    ADD     A, #LED8
    MOV     R0, A
    MOV     A, @R0
    MOVC    A, @A+DPTR
    CPL     A
    MOV     P6,A

    INC     display_index
    MOV     A, display_index
    ANL     A, #0F8H            ; if(display_index >= 8)
    JZ      L_QuitDisplayScan
    MOV     display_index, #0;  ;8位结束回0
L_QuitDisplayScan:
    POP     00H     ;R0 出栈
    POP     DPL     ;DPL出栈
    POP     DPH     ;DPH出栈
    RET


;*******************************************************************
;**************** 中断函数 ***************************************************

F_Timer0_Interrupt: ;Timer0 1ms中断函数
    PUSH    PSW     ;PSW入栈
    PUSH    ACC     ;ACC入栈

    LCALL   F_DisplayScan   ; 1ms扫描显示一位
    SETB    B_1ms           ; 1ms标志
    
    POP     ACC     ;ACC出栈
    POP     PSW     ;PSW出栈
    RETI

;========================================================================
; 函数: F_CMP_Interrupt
; 描述: 比较器中断函数.
; 参数: non.
; 返回: non.
; 版本: V1.0, 2021-3-3
;========================================================================
F_CMP_Interrupt:
    PUSH    PSW
    PUSH    ACC
    PUSH    AR2

    ANL     CMPCR1, #NOT 040H      ; 清中断标志
    MOV     A, CMPCR1
	RRC     A
	MOV     P4.7,C

L_QuitCMP_Init:
    POP     AR2
    POP     ACC
    POP     PSW
    RETI

;*******************************************************************

    END

