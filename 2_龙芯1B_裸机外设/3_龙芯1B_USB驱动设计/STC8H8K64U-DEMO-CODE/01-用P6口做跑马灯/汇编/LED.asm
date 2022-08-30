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

; 本例程基于STC8H8K64U为主控芯片的实验箱8进行编写测试，STC8G、STC8H系列芯片可通用参考.

; 程序使用P6口来演示跑马灯，输出低驱动。

; 下载时, 选择时钟 24MHZ (用户可自行修改频率).

; 当用户使用硬件 USB 对 STC8H8K64U 系列进行 ISP 下载时不能调节内部 IRC 的频率，
; 但用户可用选择内部预置的 16 个频率
; （分别是 5.5296M、 6M、 11.0592M、 12M、 18.432M、 20M、 22.1184M、 
; 24M、27M、 30M、 33.1776M、 35M、 36.864M、 40M、 44.2368M 和 48M）。
; 下载时用户只能从频率下拉列表中进行选择其中之一，而不能手动输入其他频率。
; （使用串口下载则可用输入 4M～48M 之间的任意频率）。

;******************************************

Fosc_KHZ    EQU 24000   ;24000KHZ

STACK_POIRTER   EQU     0D0H    ;堆栈开始地址

;*******************************************************************
;*******************************************************************
P4   DATA 0C0H
P5   DATA 0C8H
P6   DATA 0E8H
P7   DATA 0F8H

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


;*******************************************************************
;*******************************************************************
        ORG     0000H               ;reset
        LJMP    F_Main

        ORG     0003H               ;0 INT0 interrupt
        RETI
        LJMP    F_INT0_Interrupt      

        ORG     000BH               ;1  Timer0 interrupt
        LJMP    F_Timer0_Interrupt

        ORG     0013H               ;2  INT1 interrupt
        LJMP    F_INT1_Interrupt      

        ORG     001BH               ;3  Timer1 interrupt
        LJMP    F_Timer1_Interrupt

        ORG     0023H               ;4  UART1 interrupt
        LJMP    F_UART1_Interrupt

        ORG     002BH               ;5  ADC and SPI interrupt
        LJMP    F_ADC_Interrupt

        ORG     0033H               ;6  Low Voltage Detect interrupt
        LJMP    F_LVD_Interrupt

        ORG     003BH               ;7  PCA interrupt
        LJMP    F_PCA_Interrupt

        ORG     0043H               ;8  UART2 interrupt
        LJMP    F_UART2_Interrupt

        ORG     004BH               ;9  SPI interrupt
        LJMP    F_SPI_Interrupt

        ORG     0053H               ;10  INT2 interrupt
        LJMP    F_INT2_Interrupt

        ORG     005BH               ;11  INT3 interrupt
        LJMP    F_INT3_Interrupt

        ORG     0063H               ;12  Timer2 interrupt
        LJMP    F_Timer2_Interrupt

        ORG     0083H               ;16  INT4 interrupt
        LJMP    F_INT4_Interrupt


;******************** 主程序 **************************/
        ORG     0100H       ;reset
F_Main:
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
    MOV     R0, #0XFE
L_MainLoop:
    MOV     P6,R0
    
    MOV     A,R0
	RL      A
    MOV     R0,A
    
    MOV     A, #250
    LCALL   F_delay_ms      ;延时250ms
    LCALL   F_delay_ms      ;延时250ms

    SJMP    L_MainLoop

;*******************************************************************
;*******************************************************************



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


;**************** 中断函数 ***************************************************

F_Timer0_Interrupt: ;Timer0 1ms中断函数
    RETI
    
F_Timer1_Interrupt:
    RETI

F_Timer2_Interrupt:
    RETI

F_INT0_Interrupt:
    RETI
    
F_INT1_Interrupt:
    RETI

F_INT2_Interrupt:
    RETI

F_INT3_Interrupt:
    RETI

F_INT4_Interrupt:
    RETI

F_UART1_Interrupt:
    RETI

F_UART2_Interrupt:
    RETI

F_ADC_Interrupt:
    RETI

F_LVD_Interrupt:
    RETI

F_PCA_Interrupt:
    RETI

F_SPI_Interrupt:
    RETI


    END

