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

;本例程基于STC8H8K64U为主控芯片的实验箱8进行编写测试，STC8G、STC8H系列芯片可通用参考.

;读写RTC, IC为PCF8563.

;用STC的MCU的IO方式驱动8位数码管。

;使用Timer0的16位自动重装来产生1ms节拍,程序运行于这个节拍下, 用户修改MCU主时钟频率时,自动定时于1ms.

;8位数码管显示时间(小时-分钟-秒).

;行列扫描按键键码为17~32.

;按键只支持单键按下, 不支持多键同时按下, 那样将会有不可预知的结果.

;键按下超过1秒后,将以10键/秒的速度提供重键输出. 用户只需要检测KeyCode是否非0来判断键是否按下.

;调整时间键:
;键码17: 小时+.
;键码18: 小时-.
;键码19: 分钟+.
;键码20: 分钟-.

;下载时, 选择时钟 24MHZ (用户可自行修改频率).

;******************************************/

;/****************************** 用户定义宏 ***********************************/

Fosc_KHZ    EQU 24000   ;24000KHZ

STACK_POIRTER   EQU     0D0H    ;堆栈开始地址

Timer0_Reload   EQU     (65536 - Fosc_KHZ)  ; Timer 0 中断频率, 1000次/秒

DIS_DOT         EQU     020H
DIS_BLACK       EQU     010H
DIS_            EQU     011H

;*******************************************************************
;*******************************************************************

AUXR      DATA 08EH
P4        DATA 0C0H
P5        DATA 0C8H
P6        DATA 0E8H
P7        DATA 0F8H

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
        
P_SW2   DATA    0BAH

I2CCFG       XDATA   0FE80H
I2CMSCR      XDATA   0FE81H
I2CMSST      XDATA   0FE82H
I2CSLCR      XDATA   0FE83H
I2CSLST      XDATA   0FE84H
I2CSLADR     XDATA   0FE85H
I2CTXD       XDATA   0FE86H
I2CRXD       XDATA   0FE87H
I2CMSAUX     XDATA   0FE88H

;*************  IO口定义    **************/


;*************  本地变量声明    **************/
Flag0           DATA    20H
B_1ms           BIT     Flag0.0 ;   1ms标志

LED8            DATA    30H     ;   显示缓冲 30H ~ 37H
display_index   DATA    38H     ;   显示位索引

hour            DATA    39H     ;RTC变量
minute          DATA    3AH
second          DATA    3BH     ;
msecond_H       DATA    3CH     ;
msecond_L       DATA    3DH     ;

IO_KeyState     DATA    3EH ; IO行列键状态变量
IO_KeyState1    DATA    3FH
IO_KeyHoldCnt   DATA    40H ; IO键按下计时
KeyCode         DATA    41H ; 给用户使用的键码, 1~16为ADC键， 17~32为IO键

RTC             DATA    42H ;连续3个字节, 读写RTC时使用
cnt50ms         DATA    45H;


;*******************************************************************
;*******************************************************************
        ORG     0000H               ;reset
        LJMP    F_Main


        ORG     000BH               ;1  Timer0 interrupt
        LJMP    F_Timer0_Interrupt


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
    MOV     PSW, #0     ;选择第0组R0~R7
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

    LCALL   F_I2C_Init
    SETB    EA          ; 打开总中断

;   MOV     hour,   #12 ; 初始化时间值
;   MOV     minute, #0
;   MOV     second, #0
;   LCALL   F_DisplayRTC
    
    LCALL   F_ReadRTC   ;读RTC
    CLR     F0

    MOV     A, second   ;if(second >= 60)   F0 = 1; //错误
    CLR     C
    SUBB    A, #60
    JC      $+4
    SETB    F0

    MOV     A, minute   ;if(minute >= 60)   F0 = 1; //错误
    CLR     C
    SUBB    A, #60
    JC      $+4
    SETB    F0

    MOV     A, hour     ;if(hour   >= 24)   F0 = 1; //错误
    CLR     C
    SUBB    A, #24
    JC      $+4
    SETB    F0

    JNB     F0, L_RTC_NoErr
    MOV     hour, #12       ;有错误, 默认12:00:00
    MOV     second, #0      ;
    MOV     minute, #0      ;
    LCALL   F_WriteRTC      ;写入RTC
L_RTC_NoErr:

    LCALL   F_DisplayRTC
    MOV     LED8+2, #DIS_;
    MOV     LED8+5, #DIS_;

    CLR     A
    MOV     IO_KeyState, A
    MOV     IO_KeyState1, A
    MOV     IO_KeyHoldCnt, A
    MOV     KeyCode, A      ; 给用户使用的键码, 17~32有效
    MOV     cnt50ms, #50

;=====================================================

;=================== 主循环 ==================================
L_Main_Loop:
    JNB     B_1ms,  L_Main_Loop     ;1ms未到
    CLR     B_1ms
    
;=================== 检测1000ms是否到 ==================================
    INC     msecond_L       ;msecond + 1
    MOV     A, msecond_L
    JNZ     L_Check1000ms
    INC     msecond_H
L_Check1000ms:
    CLR     C
    MOV     A, msecond_L    ;msecond - 1000
    SUBB    A, #LOW 1000
    MOV     A, msecond_H
    SUBB    A, #HIGH 1000
    JC      L_Quit_Check_1000ms     ;if(msecond < 1000), jmp
    
;================= 1000ms到， 处理RTC ====================================
    MOV     msecond_L, #0   ;if(msecond >= 1000)
    MOV     msecond_H, #0

    LCALL   F_ReadRTC   ;读RTC
    LCALL   F_DisplayRTC
L_Quit_Check_1000ms:

;======================= 50ms扫描一次行列键盘 ==============================
L_Read_IO_Key:
    DJNZ    cnt50ms, L_Quit_Read_IO_Key     ; (cnt50ms - 1) != 0, jmp
    MOV     cnt50ms, #50    ;
    LCALL   F_IO_KeyScan    ;50ms扫描一次行列键盘
L_Quit_Read_IO_Key:
;=====================================================


;=====================================================
L_KeyProcess:
    MOV     A, KeyCode
    JZ      L_Quit_KeyProcess
                        ;有键按下
    MOV     A, KeyCode
    CJNE    A, #17, L_Quit_K17
    INC     hour        ;if(KeyCode == 17)  hour +1
    MOV     A, hour
    CLR     C
    SUBB    A, #24
    JC      L_K17_Display
    MOV     hour, #0
L_K17_Display:
    LCALL   F_WriteRTC
    LCALL   F_DisplayRTC
L_Quit_K17:

    MOV     A, KeyCode
    CJNE    A, #18, L_Quit_K18
    DEC     hour    ; hour - 1
    MOV     A, hour
    CJNE    A, #255, L_K18_Display
    MOV     hour, #23
L_K18_Display:
    LCALL   F_WriteRTC
    LCALL   F_DisplayRTC
L_Quit_K18:

    MOV     A, KeyCode
    CJNE    A, #19, L_Quit_K19
    MOV     second, #0      ;调整分钟时清除秒
    INC     minute  ; minute + 1
    MOV     A, minute
    CLR     C
    SUBB    A, #60
    JC      L_K19_Display
    MOV     minute, #0
L_K19_Display:
    LCALL   F_WriteRTC
    LCALL   F_DisplayRTC
L_Quit_K19:

    MOV     A, KeyCode
    CJNE    A, #20, L_Quit_K20
    MOV     second, #0      ;调整分钟时清除秒
    DEC     minute  ; minute - 1
    MOV     A, minute
    CJNE    A, #255, L_K20_Display
    MOV     minute, #59
L_K20_Display:
    LCALL   F_WriteRTC
    LCALL   F_DisplayRTC
L_Quit_K20:

    MOV     KeyCode, #0
L_Quit_KeyProcess:

    LJMP    L_Main_Loop

;**********************************************/

;========================================================================
; 函数: F_I2C_Init
; 描述: I2C初始化程序.
; 参数: none
; 返回: none.
; 版本: V1.0, 2021-3-4
;========================================================================
F_I2C_Init:
    ORL     P_SW2, #080H        ;使能访问XFR

    ORL     P_SW2, #00H         ;I2C功能脚选择，00:P1.5,P1.4; 01:P2.5,P2.4; 11:P3.2,P3.3

    MOV     A,#0E0H
    MOV     DPTR,#I2CCFG        ;使能I2C主机模式
    MOVX    @DPTR,A

    MOV     A,#00H
    MOV     DPTR,#I2CMSST       ;
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
    REt



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
    

;************ I2C相关函数 ****************
SLAW    EQU     0xA2
SLAR    EQU     0xA3

SDA     BIT     P1.4    ;定义SDA
SCL     BIT     P1.5    ;定义SCL

;========================================================================
; 函数: Wait
; 描述: I2C访问延时.
; 参数: none.
; 返回: none.
; 版本: VER1.0
; 日期: 2013-4-1
;========================================================================
Wait:
    MOV     DPTR,#I2CMSST
    MOVX    A,@DPTR
    JNB     ACC.6,Wait
    ANL     A,#NOT 40H
    MOVX    @DPTR,A
    RET

;========================================================================
; 函数: Start
; 描述: 启动I2C. 
; 参数: none.
; 返回: none.
; 版本: VER1.0
; 日期: 2013-4-1
;========================================================================
Start:
    MOV     A,#01H
    MOV     DPTR,#I2CMSCR  ;发送START命令
    MOVX    @DPTR,A
    LCALL   Wait
    RET


;========================================================================
; 函数: Stop
; 描述: 停止I2C. 
; 参数: none.
; 返回: none.
; 版本: VER1.0
; 日期: 2013-4-1
;========================================================================
Stop:
    MOV     A,#06H
    MOV     DPTR,#I2CMSCR  ;发送STOP命令
    MOVX    @DPTR,A
    LCALL   Wait
    RET

;========================================================================
; 函数: SendData
; 描述: 发送数据.
; 参数: I2CTXD -> A.
; 返回: none.
; 版本: VER1.0
; 日期: 2013-4-1
;========================================================================
SendData:
    MOV     DPTR,#I2CTXD   ;写数据到数据缓冲区
    MOVX    @DPTR,A
    MOV     A,#02H
    MOV     DPTR,#I2CMSCR  ;发送SEND命令
    MOVX    @DPTR,A
    LCALL   Wait
    RET

;========================================================================
; 函数: RecvACK
; 描述: 检测应答.
; 参数: none.
; 返回: none.
; 版本: VER1.0
; 日期: 2013-4-1
;========================================================================
RecvACK:
    MOV     A,#03H
    MOV     DPTR,#I2CMSCR  ;发送读ACK命令
    MOVX    @DPTR,A
    LCALL   Wait
    CLR     C
    MOV     DPTR,#I2CMSST
    MOVX    A,@DPTR
    JNB     ACC.1,$+4
    SETB    C
    RET
        
;========================================================================
; 函数: RecvData
; 描述: 接收数据.
; 参数: none.
; 返回: I2CRXD -> A.
; 版本: VER1.0
; 日期: 2013-4-1
;========================================================================
RecvData:
    MOV     A,#04H
    MOV     DPTR,#I2CMSCR  ;发送RECV命令
    MOVX    @DPTR,A
    LCALL   Wait
    MOV     DPTR,#I2CRXD
    MOVX    A,@DPTR
    RET

;========================================================================
; 函数: SendACK
; 描述: 发送应答.
; 参数: none.
; 返回: none.
; 版本: VER1.0
; 日期: 2013-4-1
;========================================================================
SendACK:
    MOV     A,#00H
    MOV     DPTR,#I2CMSST  ;设置ACK信号
    MOVX    @DPTR,A
    MOV     A,#05H
    MOV     DPTR,#I2CMSCR  ;发送ACK命令
    MOVX    @DPTR,A
    LCALL   Wait
    RET

;========================================================================
; 函数: SendNAK
; 描述: 发送非应答.
; 参数: none.
; 返回: none.
; 版本: VER1.0
; 日期: 2013-4-1
;========================================================================
SendNAK:
    MOV     A,#01H
    MOV     DPTR,#I2CMSST  ;设置NAK信号
    MOVX    @DPTR,A
    MOV     A,#05H
    MOV     DPTR,#I2CMSCR  ;发送ACK命令
    MOVX    @DPTR,A
    LCALL   Wait
    RET

;========================================================================
; 函数: F_WriteNbyte
; 描述: 写N个字节子程序。
; 参数: R2: 写I2C数据首地址,  R0: 写入数据存放首地址,  R3: 写入字节数
; 返回: none.
; 版本: VER1.0
; 日期: 2013-4-1
; 备注: 除了ACCC和PSW外, 所用到的通用寄存器都入栈
;========================================================================
F_WriteNbyte:
    ORL     P_SW2, #080H        ;使能访问XFR
    LCALL   Start
    MOV     A, #SLAW
    LCALL   SendData
    LCALL   RecvACK
    JC      L_WriteN_StopI2C

    MOV     A, R2
    LCALL   SendData
    LCALL   RecvACK
    JC      L_WriteN_StopI2C

L_WriteNbyteLoop:
    MOV     A, @R0
    LCALL   SendData
    INC     R0
    LCALL   RecvACK
    JC      L_WriteN_StopI2C
    DJNZ    R3, L_WriteNbyteLoop 
L_WriteN_StopI2C:
    LCALL   Stop
    ANL     P_SW2, #NOT 080H    ;禁止访问XFR
    RET


;========================================================================
; 函数: F_ReadNbyte
; 描述: 读N个字节子程序。
; 参数: R2: 读I2C数据首地址,  R0: 读出数据存放首地址,  R3: 读出字节数
; 返回: none.
; 版本: VER1.0
; 日期: 2013-4-1
; 备注: 除了ACCC和PSW外, 所用到的通用寄存器都入栈
;========================================================================
F_ReadNbyte:
    ORL     P_SW2, #080H        ;使能访问XFR
    LCALL   Start
    MOV     A, #SLAW
    LCALL   SendData
    LCALL   RecvACK
    JC      L_ReadN_StopI2C

    MOV     A, R2
    LCALL   SendData
    LCALL   RecvACK
    JC      L_ReadN_StopI2C

    LCALL   Start
    MOV     A, #SLAR
    LCALL   SendData
    LCALL   RecvACK
    JC      L_ReadN_StopI2C

    MOV     A, R3
    ANL     A, #0xfe    ;判断是否大于1
    JZ      L_ReadLastByte
    DEC     R3          ;大于1字节, 则-1
L_ReadNbyteLoop:
    LCALL   RecvData    ;*p = I2C_ReadAbyte();  p++;
    MOV     @R0, A
    INC     R0
    LCALL   SendACK     ;send ACK
    DJNZ    R3, L_ReadNbyteLoop 
L_ReadLastByte:
    LCALL   RecvData    ;*p = I2C_ReadAbyte()
    MOV     @R0, A
    LCALL   SendNAK     ;send no ACK
L_ReadN_StopI2C:
    LCALL   Stop
    ANL     P_SW2, #NOT 080H    ;禁止访问XFR
    RET

;========================================================================
; 函数: F_DisplayRTC
; 描述: 显示时钟子程序。
; 参数: none.
; 返回: none.
; 版本: VER1.0
; 日期: 2013-4-1
; 备注: 除了ACCC和PSW外, 所用到的通用寄存器都入栈
;========================================================================
F_DisplayRTC:
    PUSH    B       ;B入栈
    
    MOV     A, hour
    MOV     B, #10
    DIV     AB
    MOV     LED8+7, A
    MOV     LED8+6, B

    MOV     LED8+5, #DIS_
    
    MOV     A, minute
    MOV     B, #10
    DIV     AB
    MOV     LED8+4, A;
    MOV     LED8+3, B;

    MOV     LED8+2, #DIS_

    MOV     A, second
    MOV     B, #10
    DIV     AB
    MOV     LED8+1, A;
    MOV     LED8+0, B;
L_QuitDisplayRTC:
    POP     B       ;B出栈
    RET


;========================================================================
; 函数: F_ReadRTC
; 描述: 读RTC函数。
; 参数: none.
; 返回: none.
; 版本: VER1.0
; 日期: 2013-4-1
; 备注: 
;========================================================================
F_ReadRTC:

    MOV     R2, #2      ; 读I2C数据首地址
    MOV     R0, #RTC    ; 读出数据存放首地址
    MOV     R3, #3      ; 读出字节数
    LCALL   F_ReadNbyte ; R2: 读I2C数据首地址,  R0: 读出数据存放首地址,  R3: 读出字节数

    MOV     A, RTC      ; second
    SWAP    A
    ANL     A, #7
    MOV     B, #10
    MUL     AB
    MOV     B, A
    MOV     A, RTC
    ANL     A, #0x0f
    ADD     A, B
    MOV     second, A   ;second = ((RTC >> 4) & 0x07) * 10 + (RTC & 0x0f)

    MOV     A, RTC+1
    SWAP    A
    ANL     A, #7
    MOV     B, #10
    MUL     AB
    MOV     B, A
    MOV     A, RTC+1
    ANL     A, #0x0f
    ADD     A, B
    MOV     minute, A   ;minute = (([RTC+1] >> 4) & 0x07) * 10 + ([RTC+1] & 0x0f)
    
    MOV     A, RTC+2
    SWAP    A
    ANL     A, #3
    MOV     B, #10
    MUL     AB
    MOV     B, A
    MOV     A, RTC+2
    ANL     A, #0x0f
    ADD     A, B
    MOV     hour, A     ;hour   = (([RTC+2] >> 4) & 0x03) * 10 + ([RTC+2] & 0x0f)
    RET

;========================================================================
; 函数: F_WriteRTC
; 描述: 写RTC函数。
; 参数: none.
; 返回: none.
; 版本: VER1.0
; 日期: 2013-4-1
; 备注: 
;========================================================================
F_WriteRTC:

    MOV     A, second
    MOV     B, #10
    DIV     AB
    SWAP    A
    ADD     A, B
    MOV     RTC, A      ;tmp[0] = ((second / 10) << 4) + (second % 10);

    MOV     A, minute
    MOV     B, #10
    DIV     AB
    SWAP    A
    ADD     A, B
    MOV     RTC+1, A    ;((minute / 10) << 4) + (minute % 10);

    MOV     A, hour
    MOV     B, #10
    DIV     AB
    SWAP    A
    ADD     A, B
    MOV     RTC+2, A    ;((hour / 10) << 4) + (hour % 10);

    MOV     R2, #2      ;写I2C数据首地址
    MOV     R0, #RTC    ;写入数据存放首地址
    MOV     R3, #3      ;写入字节数
    LCALL   F_WriteNbyte    ;
    RET



    END

