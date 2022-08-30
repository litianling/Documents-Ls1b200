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

;�߼�PWM��ʱ�� PWM1P/PWM1N,PWM2P/PWM2N,PWM3P/PWM3N,PWM4P/PWM4N ÿ��ͨ�����ɶ���ʵ��PWM������������������Գ����.

;8��ͨ��PWM���ö�ӦP6��8���˿�.

;ͨ��P6�������ӵ�8��LED�ƣ�����PWMʵ�ֺ�����Ч��.

;PWM���ں�ռ�ձȿ��Ը�����Ҫ�������ã���߿ɴ�65535.

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
PWM2_Flag       BIT     20H.1
PWM3_Flag       BIT     20H.2
PWM4_Flag       BIT     20H.3

PWM1_Duty_H     DATA    30H
PWM1_Duty_L     DATA    31H
PWM2_Duty_H     DATA    32H
PWM2_Duty_L     DATA    33H
PWM3_Duty_H     DATA    34H
PWM3_Duty_L     DATA    35H
PWM4_Duty_H     DATA    36H
PWM4_Duty_L     DATA    37H

;*******************************************************************
;*******************************************************************
        ORG     0000H               ;reset
        LJMP    F_Main

        ORG     000BH               ;1  Timer0 interrupt
        LJMP    F_Timer0_Interrupt

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
    CLR     PWM2_Flag
    CLR     PWM3_Flag
    CLR     PWM4_Flag
    MOV     PWM1_Duty_L, #0
    MOV     PWM1_Duty_H, #0
    MOV     PWM2_Duty_L, #LOW 256
    MOV     PWM2_Duty_H, #HIGH 256
    MOV     PWM3_Duty_L, #LOW 512
    MOV     PWM3_Duty_H, #HIGH 512
    MOV     PWM4_Duty_L, #LOW 1024
    MOV     PWM4_Duty_H, #HIGH 1024

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
    CLR     P4.0

;=================== ��ѭ�� ==================================
L_Main_Loop:

    LJMP    L_Main_Loop

;========================================================================
; ����: F_PWM_Init
; ����: PWM��ʼ������.
; ����: none
; ����: none.
; �汾: V1.0, 2021-3-3
;========================================================================
F_PWM_Init:
    ORL     P_SW2, #080H        ;ʹ�ܷ���XFR

    CLR     A                   ;д CCMRx ǰ���������� CCxE �ر�ͨ��
    MOV     DPTR,#PWMA_CCER1
    MOVX    @DPTR,A
    MOV     DPTR,#PWMA_CCER2
    MOVX    @DPTR,A
    MOV     A,#060H             ;���� PWMx ģʽ1 ���
    MOV     DPTR,#PWMA_CCMR1
    MOVX    @DPTR,A
    MOV     DPTR,#PWMA_CCMR2
    MOVX    @DPTR,A
    MOV     DPTR,#PWMA_CCMR3
    MOVX    @DPTR,A
    MOV     DPTR,#PWMA_CCMR4
    MOVX    @DPTR,A
    MOV     A,#055H             ;����ͨ�����ʹ�ܺͼ���
    MOV     DPTR,#PWMA_CCER1
    MOVX    @DPTR,A
    MOV     DPTR,#PWMA_CCER2
    MOVX    @DPTR,A

    MOV     A,#3                ;��������ʱ��
    MOV     DPTR,#PWMA_ARRH
    MOVX    @DPTR,A
    MOV     A,#0FFH
    MOV     DPTR,#PWMA_ARRL
    MOVX    @DPTR,A

    MOV     A,#0FFH             ;ʹ�� PWM1~4 ���
    MOV     DPTR,#PWMA_ENO
    MOVX    @DPTR,A
    MOV     A,#0AAH             ;�߼� PWM ͨ�������ѡ��λ, P6
    MOV     DPTR,#PWMA_PS
    MOVX    @DPTR,A
    MOV     A,#080H             ;ʹ�������
    MOV     DPTR,#PWMA_BKR
    MOVX    @DPTR,A

    MOV     DPTR,#PWMA_CR1
    MOVX    A,@DPTR
    ORL     A,#01H              ;��ʼ��ʱ
    MOVX    @DPTR,A

    ANL     P_SW2, #NOT 080H    ;��ֹ����XFR
    RET

;========================================================================
; ����: F_UpdatePwm
; ����: ����PWMռ�ձ�ֵ. 
; ����: [PWMn_Duty_H PWMn_Duty_L]: pwmռ�ձ�ֵ.
; ����: none.
; �汾: V1.0, 2021-3-3
;========================================================================
F_UpdatePwm:
    ORL     P_SW2, #080H        ;ʹ�ܷ���XFR

    MOV     A, PWM1_Duty_H      ;����ռ�ձ�ʱ��
    MOV     DPTR, #PWMA_CCR1H
    MOVX    @DPTR, A
    MOV     A, PWM1_Duty_L
    MOV     DPTR, #PWMA_CCR1L
    MOVX    @DPTR, A

    MOV     A, PWM2_Duty_H      ;����ռ�ձ�ʱ��
    MOV     DPTR, #PWMA_CCR2H
    MOVX    @DPTR, A
    MOV     A, PWM2_Duty_L
    MOV     DPTR, #PWMA_CCR2L
    MOVX    @DPTR, A

    MOV     A, PWM3_Duty_H      ;����ռ�ձ�ʱ��
    MOV     DPTR, #PWMA_CCR3H
    MOVX    @DPTR, A
    MOV     A, PWM3_Duty_L
    MOV     DPTR, #PWMA_CCR3L
    MOVX    @DPTR, A

    MOV     A, PWM4_Duty_H      ;����ռ�ձ�ʱ��
    MOV     DPTR, #PWMA_CCR4H
    MOVX    @DPTR, A
    MOV     A, PWM4_Duty_L
    MOV     DPTR, #PWMA_CCR4L
    MOVX    @DPTR, A

    ANL     P_SW2, #NOT 080H    ;��ֹ����XFR
    RET

;**************** �жϺ��� ***************************************************
F_Timer0_Interrupt: ;Timer0 1ms�жϺ���
    PUSH    PSW     ;PSW��ջ
    PUSH    ACC     ;ACC��ջ

    JB      PWM1_Flag, T0_PWM1_SUB
    INC     PWM1_Duty_L       ;PWM1_Duty + 1
    MOV     A, PWM1_Duty_L
    JNZ     $+4
    INC     PWM1_Duty_H
    
    CLR     C
    MOV     A, PWM1_Duty_L
    SUBB    A, #LOW 2047
    MOV     A, PWM1_Duty_H
    SUBB    A, #HIGH 2047
    JC      T0_PWM2_Start
    SETB    PWM1_Flag
    SJMP    T0_PWM2_Start
T0_PWM1_SUB:
    MOV     A, PWM1_Duty_L
    DEC     PWM1_Duty_L       ;PWM1_Duty - 1
    JNZ     $+4
    DEC     PWM1_Duty_H
    
    MOV     A, PWM1_Duty_L
    JNZ     T0_PWM2_Start
    MOV     A, PWM1_Duty_H
    JNZ     T0_PWM2_Start
    CLR     PWM1_Flag

T0_PWM2_Start:
    JB      PWM2_Flag, T0_PWM2_SUB
    INC     PWM2_Duty_L       ;PWM2_Duty + 1
    MOV     A, PWM2_Duty_L
    JNZ     $+4
    INC     PWM2_Duty_H
    
    CLR     C
    MOV     A, PWM2_Duty_L
    SUBB    A, #LOW 2047
    MOV     A, PWM2_Duty_H
    SUBB    A, #HIGH 2047
    JC      T0_PWM3_Start
    SETB    PWM2_Flag
    SJMP    T0_PWM3_Start
T0_PWM2_SUB:
    MOV     A, PWM2_Duty_L
    DEC     PWM2_Duty_L       ;PWM2_Duty - 1
    JNZ     $+4
    DEC     PWM2_Duty_H
    
    MOV     A, PWM2_Duty_L
    JNZ     T0_PWM3_Start
    MOV     A, PWM2_Duty_H
    JNZ     T0_PWM3_Start
    CLR     PWM2_Flag
    
T0_PWM3_Start:
    JB      PWM3_Flag, T0_PWM3_SUB
    INC     PWM3_Duty_L       ;PWM3_Duty + 1
    MOV     A, PWM3_Duty_L
    JNZ     $+4
    INC     PWM3_Duty_H
    
    CLR     C
    MOV     A, PWM3_Duty_L
    SUBB    A, #LOW 2047
    MOV     A, PWM3_Duty_H
    SUBB    A, #HIGH 2047
    JC      T0_PWM4_Start
    SETB    PWM3_Flag
    SJMP    T0_PWM4_Start
T0_PWM3_SUB:
    MOV     A, PWM3_Duty_L
    DEC     PWM3_Duty_L       ;PWM3_Duty - 1
    JNZ     $+4
    DEC     PWM3_Duty_H
    
    MOV     A, PWM3_Duty_L
    JNZ     T0_PWM4_Start
    MOV     A, PWM3_Duty_H
    JNZ     T0_PWM4_Start
    CLR     PWM3_Flag
    
T0_PWM4_Start:
    JB      PWM4_Flag, T0_PWM4_SUB
    INC     PWM4_Duty_L       ;PWM4_Duty + 1
    MOV     A, PWM4_Duty_L
    JNZ     $+4
    INC     PWM4_Duty_H
    
    CLR     C
    MOV     A, PWM4_Duty_L
    SUBB    A, #LOW 2047
    MOV     A, PWM4_Duty_H
    SUBB    A, #HIGH 2047
    JC      F_QuitTimer0
    SETB    PWM4_Flag
    SJMP    F_QuitTimer0
T0_PWM4_SUB:
    MOV     A, PWM4_Duty_L
    DEC     PWM4_Duty_L       ;PWM4_Duty - 1
    JNZ     $+4
    DEC     PWM4_Duty_H
    
    MOV     A, PWM4_Duty_L
    JNZ     F_QuitTimer0
    MOV     A, PWM4_Duty_H
    JNZ     F_QuitTimer0
    CLR     PWM4_Flag
        
F_QuitTimer0:
    LCALL   F_UpdatePwm

    POP     ACC     ;ACC��ջ
    POP     PSW     ;PSW��ջ
    RETI

;======================================================================

    END

