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

;高级PWM定时器 PWM1P/PWM1N,PWM2P/PWM2N,PWM3P/PWM3N,PWM4P/PWM4N 每个通道都可独立实现PWM输出，或者两两互补对称输出.

;演示使用PWM1P,PWM1N产生互补的SPWM.

;主时钟选择24MHZ, PWM时钟选择1T, PWM周期2400, 死区12个时钟(0.5us).正弦波表用200点. 

;输出正弦波频率 = 24000000 / 2400 / 200 = 50 HZ.

;本程序仅仅是一个SPWM的演示程序, 用户可以通过上面的计算方法修改PWM周期和正弦波的点数和幅度.

;本程序输出频率固定, 如果需要变频, 请用户自己设计变频方案.

;本程序从P6.0(PWM1P)输出正相脉冲, 从P6.1(PWM1N)输出反相脉冲(互补).

;******************************************/

;/****************************** 用户定义宏 ***********************************/

Fosc_KHZ    EQU 24000   ;24000KHZ

STACK_POIRTER   EQU     0D0H    ;堆栈开始地址

Timer0_Reload   EQU     (65536 - Fosc_KHZ)  ; Timer 0 中断频率, 1000次/秒


;*******************************************************************
;*******************************************************************
AUXR    DATA    08EH
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

;*******************************************************************

PWMA_ENO      XDATA   0FEB1H
PWMA_PS       XDATA   0FEB2H

PWMA_CR1      XDATA   0FEC0H
PWMA_CR2      XDATA   0FEC1H
PWMA_SMCR     XDATA   0FEC2H
PWMA_ETR      XDATA   0FEC3H
PWMA_IER      XDATA   0FEC4H
PWMA_SR1      XDATA   0FEC5H
PWMA_SR2      XDATA   0FEC6H
PWMA_EGR      XDATA   0FEC7H
PWMA_CCMR1    XDATA   0FEC8H
PWMA_CCMR2    XDATA   0FEC9H
PWMA_CCMR3    XDATA   0FECAH
PWMA_CCMR4    XDATA   0FECBH
PWMA_CCER1    XDATA   0FECCH
PWMA_CCER2    XDATA   0FECDH
PWMA_CNTRH    XDATA   0FECEH
PWMA_CNTRL    XDATA   0FECFH
PWMA_PSCRH    XDATA   0FED0H
PWMA_PSCRL    XDATA   0FED1H
PWMA_ARRH     XDATA   0FED2H
PWMA_ARRL     XDATA   0FED3H
PWMA_RCR      XDATA   0FED4H
PWMA_CCR1H    XDATA   0FED5H
PWMA_CCR1L    XDATA   0FED6H
PWMA_CCR2H    XDATA   0FED7H
PWMA_CCR2L    XDATA   0FED8H
PWMA_CCR3H    XDATA   0FED9H
PWMA_CCR3L    XDATA   0FEDAH
PWMA_CCR4H    XDATA   0FEDBH
PWMA_CCR4L    XDATA   0FEDCH
PWMA_BKR      XDATA   0FEDDH
PWMA_DTR      XDATA   0FEDEH
PWMA_OISR     XDATA   0FEDFH

;*************  IO口定义    **************/

;*************  本地变量声明    **************/

PWM_Index       DATA    30H     ;


;*******************************************************************
;*******************************************************************
        ORG     0000H               ;reset
        LJMP    F_Main

        ORG     00D3H               ;26  PWMA interrupt
        LJMP    F_PWMA_Interrupt



;*******************************************************************
;*******************************************************************



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
    MOV     PSW, #0
    USING   0       ;选择第0组R0~R7

;================= 用户初始化程序 ====================================

    LCALL   F_PWM_Init          ; PWM初始化
    SETB    EA          ; 打开总中断

;=====================================================
L_Main_Loop:

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

    CLR     A                   ;写 CCMRx 前必须先清零 CCxE 关闭通道
    MOV     DPTR,#PWMA_CCER1
    MOVX    @DPTR,A
    MOV     A,#060H
    MOV     DPTR,#PWMA_CCMR1
    MOVX    @DPTR,A
    MOV     A,#05H              ;配置通道输出使能和极性
    MOV     DPTR,#PWMA_CCER1
    MOVX    @DPTR,A

    MOV     A,#9                ;设置周期时间
    MOV     DPTR,#PWMA_ARRH
    MOVX    @DPTR,A
    MOV     A,#060H
    MOV     DPTR,#PWMA_ARRL
    MOVX    @DPTR,A

    MOV     A,#HIGH 1220        ;设置占空比时间
    MOV     DPTR,#PWMA_CCR1H
    MOVX    @DPTR,A
    MOV     A,#LOW 1220
    MOV     DPTR,#PWMA_CCR1L
    MOVX    @DPTR,A

    MOV     A,#0CH              ;设置死区时间
    MOV     DPTR,#PWMA_DTR
    MOVX    @DPTR,A

    MOV     A,#03H              ;使能 PWM1P/PWM1N 输出
    MOV     DPTR,#PWMA_ENO
    MOVX    @DPTR,A
    MOV     A,#02H              ;高级 PWM 通道输出脚选择位
    MOV     DPTR,#PWMA_PS
    MOVX    @DPTR,A
    MOV     A,#080H             ;使能主输出
    MOV     DPTR,#PWMA_BKR
    MOVX    @DPTR,A

    MOV     A,#01H              ;使能中断
    MOV     DPTR,#PWMA_IER
    MOVX    @DPTR,A
    MOV     DPTR,#PWMA_CR1
    MOVX    A,@DPTR
    ORL     A,#01H              ;开始计时
    MOVX    @DPTR,A

    ANL     P_SW2, #NOT 080H    ;禁止访问XFR
    RET


;========================================================================
; 函数: F_PWMA_Interrupt
; 描述: PWMA中断处理程序.
; 参数: None
; 返回: none.
; 版本: V1.0, 2012-11-22
;========================================================================
F_PWMA_Interrupt:
    PUSH    PSW
    PUSH    ACC
    PUSH    P_SW2
    ORL     P_SW2, #080H  ;使能访问XFR

    MOV     DPTR,#PWMA_SR1 ;检测中断标志
    MOVX    A,@DPTR
    JNB     ACC.1,F_PWMA_QuitInt
    CLR     A
    MOVX    @DPTR,A

    MOV     A, PWM_Index
    LCALL   F_GetFirstAddress   ; DPTR = #sin_table + ACC * 2
    CLR     A
    MOVC    A, @A+DPTR
    MOV     DPTR,#PWMA_CCR1H
    MOVX    @DPTR,A

    MOV     A, #1
    MOVC    A, @A+DPTR
    MOV     DPTR,#PWMA_CCR1L
    MOVX    @DPTR,A

    INC     PWM_Index       ;PWM_Index++
    CLR     C
    MOV     A, PWM_Index    ;PWM_Index - 200
    SUBB    A, #200
    JC      F_PWMA_QuitInt  ;if(PWM_Index < 200), jmp
    MOV     PWM_Index, #0   ;if(PWM_Index >= 200)

F_PWMA_QuitInt:
    POP     P_SW2
    POP     ACC
    POP     PSW
    RETI


; 获取数据表中双字节数据的首地址
; 输入: ACC
; 输出: DPTR.   DPTR = #sin_table + ACC * 2

F_GetFirstAddress:  ;DPTR = #sin_table + ACC * 2
    MOV     DPTR, #sin_table
    PUSH    01H     ; R1入栈
    MOV     R1, A
    ADD     A, DPL
    MOV     DPL, A
    CLR     A
    ADDC    A, DPH
    MOV     DPH, A
    
    MOV     A, R1
    ADD     A, DPL
    MOV     DPL, A
    CLR     A
    ADDC    A, DPH
    MOV     DPH, A
    POP     01H     ;R1 出栈
    RET


sin_table:
    DW  1220
    DW  1256
    DW  1292
    DW  1328
    DW  1364
    DW  1400
    DW  1435
    DW  1471
    DW  1506
    DW  1541
    DW  1575
    DW  1610
    DW  1643
    DW  1677
    DW  1710
    DW  1742
    DW  1774
    DW  1805
    DW  1836
    DW  1866
    DW  1896
    DW  1925
    DW  1953
    DW  1981
    DW  2007
    DW  2033
    DW  2058
    DW  2083
    DW  2106
    DW  2129
    DW  2150
    DW  2171
    DW  2191
    DW  2210
    DW  2228
    DW  2245
    DW  2261
    DW  2275
    DW  2289
    DW  2302
    DW  2314
    DW  2324
    DW  2334
    DW  2342
    DW  2350
    DW  2356
    DW  2361
    DW  2365
    DW  2368
    DW  2369
    DW  2370
    DW  2369
    DW  2368
    DW  2365
    DW  2361
    DW  2356
    DW  2350
    DW  2342
    DW  2334
    DW  2324
    DW  2314
    DW  2302
    DW  2289
    DW  2275
    DW  2261
    DW  2245
    DW  2228
    DW  2210
    DW  2191
    DW  2171
    DW  2150
    DW  2129
    DW  2106
    DW  2083
    DW  2058
    DW  2033
    DW  2007
    DW  1981
    DW  1953
    DW  1925
    DW  1896
    DW  1866
    DW  1836
    DW  1805
    DW  1774
    DW  1742
    DW  1710
    DW  1677
    DW  1643
    DW  1610
    DW  1575
    DW  1541
    DW  1506
    DW  1471
    DW  1435
    DW  1400
    DW  1364
    DW  1328
    DW  1292
    DW  1256
    DW  1220
    DW  1184
    DW  1148
    DW  1112
    DW  1076
    DW  1040
    DW  1005
    DW  969
    DW  934
    DW  899
    DW  865
    DW  830
    DW  797
    DW  763
    DW  730
    DW  698
    DW  666
    DW  635
    DW  604
    DW  574
    DW  544
    DW  515
    DW  487
    DW  459
    DW  433
    DW  407
    DW  382
    DW  357
    DW  334
    DW  311
    DW  290
    DW  269
    DW  249
    DW  230
    DW  212
    DW  195
    DW  179
    DW  165
    DW  151
    DW  138
    DW  126
    DW  116
    DW  106
    DW  98
    DW  90
    DW  84
    DW  79
    DW  75
    DW  72
    DW  71
    DW  70
    DW  71
    DW  72
    DW  75
    DW  79
    DW  84
    DW  90
    DW  98
    DW  106
    DW  116
    DW  126
    DW  138
    DW  151
    DW  165
    DW  179
    DW  195
    DW  212
    DW  230
    DW  249
    DW  269
    DW  290
    DW  311
    DW  334
    DW  357
    DW  382
    DW  407
    DW  433
    DW  459
    DW  487
    DW  515
    DW  544
    DW  574
    DW  604
    DW  635
    DW  666
    DW  698
    DW  730
    DW  763
    DW  797
    DW  830
    DW  865
    DW  899
    DW  934
    DW  969
    DW  1005
    DW  1040
    DW  1076
    DW  1112
    DW  1148
    DW  1184

    END