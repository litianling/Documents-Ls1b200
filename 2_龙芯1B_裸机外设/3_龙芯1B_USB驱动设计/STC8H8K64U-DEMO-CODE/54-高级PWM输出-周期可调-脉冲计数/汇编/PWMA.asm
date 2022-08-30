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

;�����̻���STC8H8K64UΪ����оƬ��ʵ����8���б�д���ԣ�STC8Hϵ��оƬ��ͨ�òο�.

;�߼�PWM��ʱ��ʵ�ָ���PWM�������.

;����/ռ�ձȿɵ�, ͨ���Ƚ�/�����жϽ��������������.

;ͨ��P6����ʾ���,ÿ��10ms���һ��PWM,����10�������ֹͣ���.

;��ʱ��ÿ1ms����PWM����.

;����ʱ, ѡ��ʱ�� 24MHZ (�û��������޸�Ƶ��).

;******************************************/

;/****************************** �û������ ***********************************/

Fosc_KHZ    EQU 24000   ;24000KHZ

STACK_POIRTER   EQU     0D0H    ;��ջ��ʼ��ַ

Timer0_Reload   EQU     (65536 - Fosc_KHZ)  ; Timer 0 �ж�Ƶ��, 1000��/��

;*******************************************************************
;*******************************************************************


;*******************************************************************
AUXR        DATA 08EH
P4          DATA 0C0H
P5          DATA 0C8H
P6          DATA 0E8H
P7          DATA 0F8H
PCON2       DATA 097H

INT_CLKO    DATA    0x8F
P_SW1       DATA    0A2H
IE2         DATA    0AFH
T2H         DATA    0D6H
T2L         DATA    0D7H

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
P_SW2   DATA    0BAH

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

;*******************************************************************

;*************  ���ر�������    **************/
PWM1_Flag       BIT     20H.0
B_1ms           BIT     20H.1
;PWM2_Flag       BIT     20H.1
;PWM3_Flag       BIT     20H.2
;PWM4_Flag       BIT     20H.3

Period_H         DATA    30H
Period_L         DATA    31H

msecond          DATA    32H
Counter          DATA    33H

;PWM1_Duty_H     DATA    30H
;PWM1_Duty_L     DATA    31H
;PWM2_Duty_H     DATA    32H
;PWM2_Duty_L     DATA    33H
;PWM3_Duty_H     DATA    34H
;PWM3_Duty_L     DATA    35H
;PWM4_Duty_H     DATA    36H
;PWM4_Duty_L     DATA    37H

;*******************************************************************
;*******************************************************************
        ORG     0000H               ;reset
        LJMP    F_Main

        ORG     000BH               ;1  Timer0 interrupt
        LJMP    F_Timer0_Interrupt

        ORG     00D3H               ;26  PWMA interrupt
        LJMP    F_PWMA_Interrupt

;******************** ������ **************************/
        ORG     0100H       ;reset
F_Main:
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
    MOV     PSW, #0
    USING   0       ;ѡ���0��R0~R7

;================= �û���ʼ������ ====================================
    CLR     PWM1_Flag
    MOV     Period_L, #LOW 01000H
    MOV     Period_H, #HIGH 01000H

    CLR     TR0
    ORL     AUXR, #(1 SHL 7)    ; Timer0_1T();
    ANL     TMOD, #NOT 04H      ; Timer0_AsTimer();
    ANL     TMOD, #NOT 03H      ; Timer0_16bitAutoReload();
    MOV     TH0, #Timer0_Reload / 256   ;Timer0_Load(Timer0_Reload);
    MOV     TL0, #Timer0_Reload MOD 256
    SETB    ET0         ; Timer0_InterruptEnable();
    SETB    TR0         ; Timer0_Run();
    SETB    EA          ; �����ж�
    
    LCALL   F_PWM_Init          ; PWM��ʼ��
    LCALL   F_UpdatePwm
    CLR     P4.0

;=================== ��ѭ�� ==================================
L_Main_Loop:
    JNB     B_1ms,  L_Main_Loop     ;1msδ��
    CLR     B_1ms

    INC     msecond       ;msecond + 1
    CLR     C
    MOV     A, msecond    ;msecond - 10
    SUBB    A, #10
    JC      L_Main_Loop     ;if(msecond < 10), jmp
    MOV     msecond, #0   ;if(msecond >= 10)

    LCALL   F_TxPulse
    LJMP    L_Main_Loop

;========================================================================
; ����: F_TxPulse
; ����: �������庯��.
; ����: none.
; ����: none.
; �汾: V1.0, 2013-11-22
;========================================================================
F_TxPulse:
    ORL     P_SW2, #080H          ;ʹ�ܷ���XFR

    MOV     A,#00                 ;д CCMRx ǰ���������� CCxE �ر�ͨ��
    MOV     DPTR,#PWMA_CCER1
    MOVX    @DPTR,A
    MOV     A,#060H               ;���� PWM1 ģʽ1 ���
    MOV     DPTR,#PWMA_CCMR1
    MOVX    @DPTR,A
    MOV     A,#01H                ;ʹ�� CC1E ͨ��, �ߵ�ƽ��Ч
    MOV     DPTR,#PWMA_CCER1
    MOVX    @DPTR,A
    MOV     A,#00                 ;���־λ
    MOV     DPTR,#PWMA_SR1
    MOVX    @DPTR,A
    MOV     A,#00                 ;�������
    MOV     DPTR,#PWMA_CNTRH
    MOVX    @DPTR,A
    MOV     A,#00                 ;�������
    MOV     DPTR,#PWMA_CNTRL
    MOVX    @DPTR,A
    MOV     A,#02H                ;ʹ�ܲ���/�Ƚ� 1 �ж�
    MOV     DPTR,#PWMA_IER
    MOVX    @DPTR,A

;    ANL     P_SW2, #NOT 080H    ;��ֹ����XFR
    RET

;========================================================================
; ����: F_PWM_Init
; ����: PWM��ʼ������.
; ����: none
; ����: none.
; �汾: V1.0, 2021-3-3
;========================================================================
F_PWM_Init:
    ORL     P_SW2, #080H        ;ʹ�ܷ���XFR

    MOV     A,#01H              ;ʹ�� PWM1 ���
    MOV     DPTR,#PWMA_ENO
    MOVX    @DPTR,A
    MOV     A,#02H              ;�߼� PWM ͨ�������ѡ��λ, P6
    MOV     DPTR,#PWMA_PS
    MOVX    @DPTR,A
    MOV     A,#080H             ;ʹ�������
    MOV     DPTR,#PWMA_BKR
    MOVX    @DPTR,A

    MOV     DPTR,#PWMA_CR1
    MOVX    A,@DPTR
    ORL     A,#01H              ;��ʼ��ʱ
    MOVX    @DPTR,A

    ;ANL     P_SW2, #NOT 080H    ;��ֹ����XFR
    RET

;========================================================================
; ����: F_UpdatePwm
; ����: PWM����ռ�ձ�.
; ����: [Period_H Period_H]: pwm����ֵ.
; ����: none.
; �汾: V1.0, 2021-8-24
;========================================================================
F_UpdatePwm:
    ORL     P_SW2, #080H        ;ʹ�ܷ���XFR

    MOV     A, Period_H         ;��������ʱ��
    MOV     DPTR, #PWMA_ARRH
    MOVX    @DPTR, A
    MOV     A, Period_L
    MOV     DPTR, #PWMA_ARRL
    MOVX    @DPTR, A

    MOV     A, Period_H         ;����ռ�ձ�ʱ��: Period/2
	CLR     C
	RRC     A
    MOV     DPTR, #PWMA_CCR1H
    MOVX    @DPTR, A
    MOV     A, Period_L
	RRC     A
    MOV     DPTR, #PWMA_CCR1L
    MOVX    @DPTR, A

    ;ANL     P_SW2, #NOT 080H    ;��ֹ����XFR
    RET

;**************** �жϺ��� ***************************************************
F_Timer0_Interrupt: ;Timer0 1ms�жϺ���
    PUSH    PSW     ;PSW��ջ
    PUSH    ACC     ;ACC��ջ

    SETB    B_1ms           ; 1ms��־
    JNB     PWM1_Flag, T0_PWM1_SUB
    INC     Period_L       ;Period + 1
    MOV     A, Period_L
    JNZ     $+4
    INC     Period_H
    
    CLR     C
    MOV     A, Period_L
    SUBB    A, #LOW 01000H
    MOV     A, Period_H
    SUBB    A, #HIGH 01000H
    JC      F_QuitTimer0
    CLR     PWM1_Flag
    SJMP    F_QuitTimer0
T0_PWM1_SUB:
    MOV     A, Period_L
    DEC     Period_L       ;Period - 1
    JNZ     $+4
    DEC     Period_H

    CLR     C
    MOV     A, Period_L
    SUBB    A, #LOW 0100H
    MOV     A, Period_H
    SUBB    A, #HIGH 0100H
    JNC     F_QuitTimer0
    SETB    PWM1_Flag

F_QuitTimer0:
    LCALL   F_UpdatePwm

    POP     ACC     ;ACC��ջ
    POP     PSW     ;PSW��ջ
    RETI

;========================================================================
; ����: F_PWMA_Interrupt
; ����: PWMA�жϴ������.
; ����: None
; ����: none.
; �汾: V1.0, 2021-08-23
;========================================================================
F_PWMA_Interrupt:
    PUSH    PSW
    PUSH    ACC
    PUSH    P_SW2
    ORL     P_SW2, #080H          ;ʹ�ܷ���XFR

    MOV     DPTR,#PWMA_SR1        ;���ӻ�״̬
    MOVX    A,@DPTR
    JNB     ACC.1,F_PWMA_QuitInt
    CLR     A
    MOVX    @DPTR,A

    INC     Counter               ;Counter + 1
    CLR     C
    MOV     A, Counter            ;Counter - 10
    SUBB    A, #10
    JC      F_PWMA_QuitInt        ;if(Counter < 10), jmp
    MOV     Counter, #0           ;if(Counter >= 10)

    MOV     A,#00                 ;д CCMRx ǰ���������� CCxE �ر�ͨ��
    MOV     DPTR,#PWMA_CCER1
    MOVX    @DPTR,A
    MOV     A,#040H               ;���� PWM1 ǿ��Ϊ��Ч��ƽ
    MOV     DPTR,#PWMA_CCMR1
    MOVX    @DPTR,A
    MOV     A,#01H                ;ʹ�� CC1E ͨ��, �ߵ�ƽ��Ч
    MOV     DPTR,#PWMA_CCER1
    MOVX    @DPTR,A
    MOV     A,#00H                ;�ر��ж�
    MOV     DPTR,#PWMA_IER
    MOVX    @DPTR,A

F_PWMA_QuitInt:
    POP     P_SW2
    POP     ACC
    POP     PSW
    RETI

;======================================================================

    END

