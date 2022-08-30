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


;/************* 功能说明    **************

;本例程基于STC8H8K64U为主控芯片的实验箱8进行编写测试，STC8G、STC8H系列芯片可通用参考.

;程序使用P6口做跑马灯，演示系统时钟源切换效果。

;跑马灯每跑一轮切换一次时钟源，分别是默认IRC主频，主频48分频，内部32K IRC。

;下载时, 选择时钟 24MHZ (用户可自行修改频率).

;******************************************/

;/****************************** 用户定义宏 ***********************************/

Fosc_KHZ    EQU 24000   ;24000KHZ

STACK_POIRTER   EQU     0D0H    ;堆栈开始地址

;*******************************************************************
;*******************************************************************
P_SW2   DATA    0BAH
P4      DATA    0C0H
P5      DATA    0C8H
P6      DATA    0E8H
P7      DATA    0F8H

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

CKSEL       EQU     0FE00H
CLKDIV      EQU     0FE01H
HIRCCR      EQU     0FE02H
XOSCCR      EQU     0FE03H
IRC32KCR    EQU     0FE04H

;*******************************************************************

Mode        DATA    21H

;*******************************************************************
            ORG     0000H
            LJMP    MAIN

            ORG     0100H
MAIN:
    CLR     A
    MOV     P0M1, A     ;设置为准双向口
    MOV     P0M0, A
    MOV     P1M1, A     ;设置为准双向口
    MOV     P1M0, A
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

    MOV     SP, #STACK_POIRTER
    MOV     PSW, #0     ;选择第0组R0~R7


    CLR     P4.0        ;LED Power On
    MOV     Mode, #0
    MOV     R0, #0XFE
L_MainLoop:
    MOV     P6,R0
    
    MOV     A,R0
	RL      A
    MOV     R0,A

    JB      ACC.0,L_MainDelay
    LCALL   F_MCLK_Sel

L_MainDelay:
    MOV     A, #10
    LCALL   F_delay_ms      ;延时10ms
    SJMP    L_MainLoop

;========================================================================
; 函数: F_delay_ms
; 描述: 延时子程序。
; 参数: ACC: 延时ms数.
; 返回: none.
; 版本: VER1.0
; 日期: 2013-4-1
; 备注: 除了ACCC和PSW外, 所用到的通用寄存器都入栈
;========================================================================
F_delay_ms:
    PUSH    02H     ;入栈R2
    PUSH    03H     ;入栈R3
    PUSH    04H     ;入栈R4

    MOV     R2,A

L_delay_ms_1:
    MOV     R3, #HIGH (Fosc_KHZ / 10)
    MOV     R4, #LOW (Fosc_KHZ / 10)
    
L_delay_ms_2:
    MOV     A, R4           ;1T     Total 10T/loop
    DEC     R4              ;1T
    JNZ     L_delay_ms_3    ;3T
    DEC     R3
L_delay_ms_3:
    DEC     A               ;1T
    ORL     A, R3           ;1T
    JNZ     L_delay_ms_2    ;3T
    
    DJNZ    R2, L_delay_ms_1

    POP     04H     ;出栈R2
    POP     03H     ;出栈R3
    POP     02H     ;出栈R4
    RET

;========================================================================
; 函数: F_MCLK_Sel
; 描述: 主频设置程序。
; 参数: none.
; 返回: none.
; 版本: VER1.0
; 日期: 2013-4-1
;========================================================================
F_MCLK_Sel:
    MOV     A,Mode
	JNZ     F_MCLK_1
F_MCLK_0:
    MOV     P_SW2,#80H
    MOV     A,#080H                    ;启动内部 IRC
    MOV     DPTR,#HIRCCR
    MOVX    @DPTR,A
    MOVX    A,@DPTR
    JNB     ACC.0,$-1                  ;等待时钟稳定

    MOV     A,#00H                     ;时钟不分频
    MOV     DPTR,#CLKDIV
    MOVX    @DPTR,A
    MOV     A,#00H                     ;选择内部IRC(默认)
    MOV     DPTR,#CKSEL
    MOVX    @DPTR,A
    MOV     P_SW2,#00H
	INC     Mode
    RET

F_MCLK_1:
    MOV     A,Mode
    XRL     A,#1
	JNZ     F_MCLK_2
    MOV     P_SW2,#80H
    MOV     A,#080H                    ;启动内部 IRC
    MOV     DPTR,#HIRCCR
    MOVX    @DPTR,A
    MOVX    A,@DPTR
    JNB     ACC.0,$-1                  ;等待时钟稳定

    MOV     A,#48                      ;时钟48分频
    MOV     DPTR,#CLKDIV
    MOVX    @DPTR,A
    MOV     A,#00H                     ;选择内部IRC(默认)
    MOV     DPTR,#CKSEL
    MOVX    @DPTR,A
    MOV     P_SW2,#00H
	INC     Mode
    RET

F_MCLK_2:
    MOV     A,Mode
    XRL     A,#2
	JNZ     F_MCLK_CLR
    MOV     P_SW2,#80H
    MOV     A,#080H                    ;启动内部 32K IRC
    MOV     DPTR,#IRC32KCR
    MOVX    @DPTR,A
    MOVX    A,@DPTR
    JNB     ACC.0,$-1                  ;等待时钟稳定

    MOV     A,#00H                     ;时钟不分频
    MOV     DPTR,#CLKDIV
    MOVX    @DPTR,A
    MOV     A,#03H                     ;选择内部 32K
    MOV     DPTR,#CKSEL
    MOVX    @DPTR,A
    MOV     P_SW2,#00H
F_MCLK_CLR:
    MOV     Mode, #0
    RET

;F_MCLK_3:
    ;MOV     A,Mode
    ;XRL     A,#3
	;JNZ     F_MCLK_CLR
    ;MOV     P_SW2,#80H
    ;MOV     A,#0C0H                    ;启动外部晶振
    ;MOV     DPTR,#XOSCCR
    ;MOVX    @DPTR,A
    ;MOVX    A,@DPTR
    ;JNB     ACC.0,$-1                  ;等待时钟稳定

    ;MOV     A,#00H                     ;时钟不分频
    ;MOV     DPTR,#CLKDIV
    ;MOVX    @DPTR,A
    ;MOV     A,#01H                     ;选择外部晶振
    ;MOV     DPTR,#CKSEL
    ;MOVX    @DPTR,A
    ;MOV     P_SW2,#00H
    ;MOV     Mode, #0
    ;RET

    END

