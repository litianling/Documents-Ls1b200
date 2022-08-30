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

;通过P2.7口输出PWM驱动蜂鸣器.

;行列扫描按键每个按键按下后蜂鸣器响一下.

;下载时, 选择时钟 24MHZ (用户可自行修改频率).

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
B_1ms           BIT     20H.0   ;   1ms标志

KeyCode         DATA    38H ; 给用户使用的键码, 1~16为ADC键， 17~32为IO键
cnt50ms         DATA    39H;

IO_KeyState     DATA    3AH ; IO行列键状态变量
IO_KeyState1    DATA    3BH
IO_KeyHoldCnt   DATA    3CH ; IO键按下计时

;*******************************************************************
;*******************************************************************
        ORG     0000H               ;reset
        LJMP    F_Main

        ORG     000BH               ;1  Timer0 interrupt
        LJMP    F_Timer0_Interrupt



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

    CLR     TR0
    ORL     AUXR, #(1 SHL 7)    ; Timer0_1T();
    ANL     TMOD, #NOT 04H      ; Timer0_AsTimer();
    ANL     TMOD, #NOT 03H      ; Timer0_16bitAutoReload();
    MOV     TH0, #Timer0_Reload / 256   ;Timer0_Load(Timer0_Reload);
    MOV     TL0, #Timer0_Reload MOD 256
    SETB    ET0         ; Timer0_InterruptEnable();
    SETB    TR0         ; Timer0_Run();
    
    LCALL   F_PWM_Init          ; PWM初始化
    SETB    EA          ; 打开总中断

    MOV     cnt50ms, #50
    CLR     A
    MOV     IO_KeyState, A
    MOV     IO_KeyState1, A
    MOV     IO_KeyHoldCnt, A
    MOV     KeyCode, A      ; 给用户使用的键码, 17~32有效

;=================== 主循环 ==================================
L_Main_Loop:
    JNB     B_1ms,  L_Main_Loop     ;1ms未到
    CLR     B_1ms

    MOV     A, KeyCode
    JZ     L_Read_IO_Key

    ORL     P_SW2, #080H        ;使能访问XFR
    MOV     A,#080H             ;使能 PWM4N 输出
    MOV     DPTR,#PWMA_ENO
    MOVX    @DPTR,A
    MOV     A, #250
    LCALL   F_delay_ms          ;蜂鸣器响250ms
    MOV     A,#000H             ;禁止 PWM4N 输出
    MOV     DPTR,#PWMA_ENO
    MOVX    @DPTR,A

    ANL     P_SW2, #NOT 080H    ;禁止访问XFR
    MOV     KeyCode, #0

;======================= 50ms扫描一次行列键盘 ==============================
L_Read_IO_Key:
    DJNZ    cnt50ms, L_Main_Loop     ; (cnt50ms - 1) != 0, jmp
    MOV     cnt50ms, #50    ;
    LCALL   F_IO_KeyScan    ;50ms扫描一次行列键盘

    LJMP    L_Main_Loop

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
; 函数: F_PWM_Init
; 描述: PWM初始化程序.
; 参数: none
; 返回: none.
; 版本: V1.0, 2021-3-3
;========================================================================
F_PWM_Init:
    ORL     P_SW2, #080H        ;使能访问XFR

    CLR     A                   ;写 CCMRx 前必须先清零 CCxE 关闭通道
    MOV     DPTR,#PWMA_CCER2
    MOVX    @DPTR,A
    MOV     A,#030H
    MOV     DPTR,#PWMA_CCMR4
    MOVX    @DPTR,A
    MOV     A,#050H             ;配置通道输出使能和极性
    MOV     DPTR,#PWMA_CCER2
    MOVX    @DPTR,A

    MOV     A,#7                ;设置周期时间
    MOV     DPTR,#PWMA_ARRH
    MOVX    @DPTR,A
    MOV     A,#0FFH
    MOV     DPTR,#PWMA_ARRL
    MOVX    @DPTR,A

    MOV     A,#080H             ;使能 PWM4N 输出
    MOV     DPTR,#PWMA_ENO
    MOVX    @DPTR,A
    MOV     A,#040H             ;高级 PWM 通道输出脚选择位
    MOV     DPTR,#PWMA_PS
    MOVX    @DPTR,A
    MOV     A,#080H             ;使能主输出
    MOV     DPTR,#PWMA_BKR
    MOVX    @DPTR,A

    MOV     DPTR,#PWMA_CR1
    MOVX    A,@DPTR
    ORL     A,#01H              ;开始计时
    MOVX    @DPTR,A

    MOV     A, #250
    LCALL   F_delay_ms          ;上电蜂鸣器响500ms
    LCALL   F_delay_ms
    MOV     A,#000H             ;禁止 PWM4N 输出
    MOV     DPTR,#PWMA_ENO
    MOVX    @DPTR,A

    ANL     P_SW2, #NOT 080H    ;禁止访问XFR
    RET

;/*****************************************************
;   行列键扫描程序
;   使用XY查找4x4键的方法，只能单键，速度快
;
;   Y     P04      P05      P06      P07
;          |        |        |        |
;X         |        |        |        |
;P00 ---- K00 ---- K01 ---- K02 ---- K03 ----
;          |        |        |        |
;P01 ---- K04 ---- K05 ---- K06 ---- K07 ----
;          |        |        |        |
;P02 ---- K08 ---- K09 ---- K10 ---- K11 ----
;          |        |        |        |
;P03 ---- K12 ---- K13 ---- K14 ---- K15 ----
;          |        |        |        |
;******************************************************/


T_KeyTable:  DB 0,1,2,0,3,0,0,0,4,0,0,0,0,0,0,0

F_IO_KeyDelay:
    PUSH    03H     ;R3入栈
    MOV     R3, #60
    DJNZ    R3, $   ; (n * 4) T
    POP     03H     ;R3出栈
    RET

F_IO_KeyScan:   ;50ms call
    PUSH    06H     ;R6入栈
    PUSH    07H     ;R7入栈

    MOV     R6, IO_KeyState1    ; 保存上一次状态

    MOV     P0, #0F0H       ;X低，读Y
    LCALL   F_IO_KeyDelay       ;delay about 250T
    MOV     A, P0
    ANL     A, #0F0H
    MOV     IO_KeyState1, A     ; IO_KeyState1 = P0 & 0xf0

    MOV     P0, #0FH        ;Y低，读X
    LCALL   F_IO_KeyDelay       ;delay about 250T
    MOV     A, P0
    ANL     A, #0FH
    ORL     A, IO_KeyState1         ; IO_KeyState1 |= (P0 & 0x0f)
    MOV     IO_KeyState1, A
    XRL     IO_KeyState1, #0FFH     ; IO_KeyState1 ^= 0xff; 取反

    MOV     A, R6                   ;if(j == IO_KeyState1), 连续两次读相等
    CJNE    A, IO_KeyState1, L_QuitCheckIoKey   ;不相等, jmp
    
    MOV     R6, IO_KeyState     ;暂存IO_KeyState
    MOV     IO_KeyState, IO_KeyState1
    MOV     A, IO_KeyState
    JZ      L_NoIoKeyPress      ; if(IO_KeyState != 0), 有键按下

    MOV     A, R6   
    JZ      L_CalculateIoKey    ;if(R6 == 0)    F0 = 1; 第一次按下
    MOV     A, R6   
    CJNE    A, IO_KeyState, L_QuitCheckIoKey    ; if(j != IO_KeyState), jmp
    
    INC     IO_KeyHoldCnt   ; if(++IO_KeyHoldCnt >= 20),    1秒后重键
    MOV     A, IO_KeyHoldCnt
    CJNE    A, #20, L_QuitCheckIoKey
    MOV     IO_KeyHoldCnt, #18;
L_CalculateIoKey:
    MOV     A, IO_KeyState
    SWAP    A       ;R6 = T_KeyTable[IO_KeyState >> 4];
    ANL     A, #0x0F
    MOV     DPTR, #T_KeyTable
    MOVC    A, @A+DPTR
    MOV     R6, A
    
    JZ      L_QuitCheckIoKey    ; if(R6 == 0)
    MOV     A, IO_KeyState
    ANL     A, #0x0F
    MOVC    A, @A+DPTR
    MOV     R7, A
    JZ      L_QuitCheckIoKey    ; if(T_KeyTable[IO_KeyState& 0x0f] == 0)
    
    MOV     A, R6       ;KeyCode = (j - 1) * 4 + T_KeyTable[IO_KeyState & 0x0f] + 16;   //计算键码，17~32
    ADD     A, ACC
    ADD     A, ACC
    MOV     R6, A
    MOV     A, R7
    ADD     A, R6
    ADD     A, #12
    MOV     KeyCode, A
    SJMP    L_QuitCheckIoKey
    
L_NoIoKeyPress:
    MOV     IO_KeyHoldCnt, #0

L_QuitCheckIoKey:
    MOV     P0, #0xFF
    POP     07H     ;R7出栈
    POP     06H     ;R6出栈
    RET

;*******************************************************************
;**************** 中断函数 ***************************************************

F_Timer0_Interrupt: ;Timer0 1ms中断函数
    PUSH    PSW     ;PSW入栈
    PUSH    ACC     ;ACC入栈

    SETB    B_1ms           ; 1ms标志
    

    POP     ACC     ;ACC出栈
    POP     PSW     ;PSW出栈
    RETI

    END

