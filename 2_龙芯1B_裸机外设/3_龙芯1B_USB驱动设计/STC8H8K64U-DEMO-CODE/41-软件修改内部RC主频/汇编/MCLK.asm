;/*---------------------------------------------------------------------*/
;/* --- STC MCU Limited ------------------------------------------------*/
;/* --- STC 1T Series MCU Demo Programme -------------------------------*/
;/* --- Mobile: (86)13922805190 ----------------------------------------*/
;/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
;/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
;/* --- Web: www.STCMCU.com --------------------------------------------*/
;/* --- Web: www.STCMCUDATA.com ----------------------------------------*/
;/* --- QQ:  800003751 -------------------------------------------------*/
;/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����        */
;/*---------------------------------------------------------------------*/


;/************* ����˵��    **************

;�����̻���STC8H8K64UΪ����оƬ��ʵ����8���б�д���ԣ�STC8G��STC8Hϵ��оƬ��ͨ�òο�.

;����ʹ��P6��������ƣ���ʾϵͳʱ��Դ�л�Ч����

;�����ÿ��һ���л�һ��ʱ��Դ���ֱ���Ĭ��IRC��Ƶ����Ƶ48��Ƶ���ڲ�32K IRC��

;����ʱ, ѡ��ʱ�� 24MHZ (�û��������޸�Ƶ��).

;******************************************/

;/****************************** �û������ ***********************************/

Fosc_KHZ    EQU 24000   ;24000KHZ

STACK_POIRTER   EQU     0D0H    ;��ջ��ʼ��ַ

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
    MOV     P0M1, A     ;����Ϊ׼˫���
    MOV     P0M0, A
    MOV     P1M1, A     ;����Ϊ׼˫���
    MOV     P1M0, A
    MOV     P2M1, A     ;����Ϊ׼˫���
    MOV     P2M0, A
    MOV     P3M1, A     ;����Ϊ׼˫���
    MOV     P3M0, A
    MOV     P4M1, A     ;����Ϊ׼˫���
    MOV     P4M0, A
    MOV     P5M1, A     ;����Ϊ׼˫���
    MOV     P5M0, A
    MOV     P6M1, A     ;����Ϊ׼˫���
    MOV     P6M0, A
    MOV     P7M1, A     ;����Ϊ׼˫���
    MOV     P7M0, A

    MOV     SP, #STACK_POIRTER
    MOV     PSW, #0     ;ѡ���0��R0~R7


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
    LCALL   F_delay_ms      ;��ʱ10ms
    SJMP    L_MainLoop

;========================================================================
; ����: F_delay_ms
; ����: ��ʱ�ӳ���
; ����: ACC: ��ʱms��.
; ����: none.
; �汾: VER1.0
; ����: 2013-4-1
; ��ע: ����ACCC��PSW��, ���õ���ͨ�üĴ�������ջ
;========================================================================
F_delay_ms:
    PUSH    02H     ;��ջR2
    PUSH    03H     ;��ջR3
    PUSH    04H     ;��ջR4

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

    POP     04H     ;��ջR2
    POP     03H     ;��ջR3
    POP     02H     ;��ջR4
    RET

;========================================================================
; ����: F_MCLK_Sel
; ����: ��Ƶ���ó���
; ����: none.
; ����: none.
; �汾: VER1.0
; ����: 2013-4-1
;========================================================================
F_MCLK_Sel:
    MOV     A,Mode
	JNZ     F_MCLK_1
F_MCLK_0:
    MOV     P_SW2,#80H
    MOV     A,#080H                    ;�����ڲ� IRC
    MOV     DPTR,#HIRCCR
    MOVX    @DPTR,A
    MOVX    A,@DPTR
    JNB     ACC.0,$-1                  ;�ȴ�ʱ���ȶ�

    MOV     A,#00H                     ;ʱ�Ӳ���Ƶ
    MOV     DPTR,#CLKDIV
    MOVX    @DPTR,A
    MOV     A,#00H                     ;ѡ���ڲ�IRC(Ĭ��)
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
    MOV     A,#080H                    ;�����ڲ� IRC
    MOV     DPTR,#HIRCCR
    MOVX    @DPTR,A
    MOVX    A,@DPTR
    JNB     ACC.0,$-1                  ;�ȴ�ʱ���ȶ�

    MOV     A,#48                      ;ʱ��48��Ƶ
    MOV     DPTR,#CLKDIV
    MOVX    @DPTR,A
    MOV     A,#00H                     ;ѡ���ڲ�IRC(Ĭ��)
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
    MOV     A,#080H                    ;�����ڲ� 32K IRC
    MOV     DPTR,#IRC32KCR
    MOVX    @DPTR,A
    MOVX    A,@DPTR
    JNB     ACC.0,$-1                  ;�ȴ�ʱ���ȶ�

    MOV     A,#00H                     ;ʱ�Ӳ���Ƶ
    MOV     DPTR,#CLKDIV
    MOVX    @DPTR,A
    MOV     A,#03H                     ;ѡ���ڲ� 32K
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
    ;MOV     A,#0C0H                    ;�����ⲿ����
    ;MOV     DPTR,#XOSCCR
    ;MOVX    @DPTR,A
    ;MOVX    A,@DPTR
    ;JNB     ACC.0,$-1                  ;�ȴ�ʱ���ȶ�

    ;MOV     A,#00H                     ;ʱ�Ӳ���Ƶ
    ;MOV     DPTR,#CLKDIV
    ;MOVX    @DPTR,A
    ;MOV     A,#01H                     ;ѡ���ⲿ����
    ;MOV     DPTR,#CKSEL
    ;MOVX    @DPTR,A
    ;MOV     P_SW2,#00H
    ;MOV     Mode, #0
    ;RET

    END

