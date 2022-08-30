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


;*************  ����˵��    **************

;�����̻���STC8H8K64UΪ����оƬ��ʵ����8���б�д���ԣ�STC8Hϵ��оƬ��ͨ�òο�.

;�Ƚ���������ѡ�� P1.1 �� ADC ��ģ������ͨ����

;������������ P3.6 �˿ڻ������ڲ� BandGap ���� OP ��� REFV ��ѹ(1.19V�ڲ��̶��Ƚϵ�ѹ)��

;ͨ���жϻ��߲�ѯ��ʽ��ȡ�Ƚ����ȽϽ����CMP+�ĵ�ƽ����CMP-�ĵ�ƽP47��(LED10)����͵�ƽ����֮����ߵ�ƽ��


;��P1.7���16λ��PWM, �����PWM����RC�˲���ֱ����ѹ��P1.1��ADC�����������ʾ����.

;����1����Ϊ115200bps, 8,n, 1, �л���P3.0 P3.1, ���غ�Ϳ���ֱ�Ӳ���. ͨ������1����ռ�ձ�.

;��������ʹ��ASCII������֣��������õ�ֵΪ0~256.

;���4λ�������ʾPWM��ռ�ձ�ֵ���ұ�4λ�������ʾADCֵ��


;ʵ��PWMռ�ձ�����Ϊ120����ʱ��P1.1 �� ADC ��ģ������CMP+�ĵ�ƽ����CMP-�ĵ�ƽ��1.19V����P47������͵�ƽ(LED10����)����֮����ߵ�ƽ(LED10����)��

;����ʱ, ѡ��ʱ�� 22.1184MHZ (�û��������޸�Ƶ��).

;******************************************

;/****************************** �û������ ***********************************/

Fosc_KHZ    EQU 22118   ;22.118MHZ

STACK_POIRTER   EQU     0D0H    ; ��ջ��ʼ��ַ

Timer0_Reload   EQU     (65536 - Fosc_KHZ)  ; Timer 0 �ж�Ƶ��, 1000��/��

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

ADC_CONTR   DATA 0BCH   ;��ADϵ��
ADC_RES     DATA 0BDH   ;��ADϵ��
ADC_RESL    DATA 0BEH   ;��ADϵ��
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

;*************  IO�ڶ���    **************/


;*************  ���ر�������    **************/
B_1ms           BIT     20H.0   ;   1ms��־

LED8            DATA    30H     ;   ��ʾ���� 30H ~ 37H
display_index   DATA    38H     ;   ��ʾλ����

msecond_H       DATA    39H     ;
msecond_L       DATA    3AH     ;

RX1_Lenth       EQU     16      ; ���ڽ��ջ��峤��

B_TX1_Busy      BIT     20H.1   ; ����æ��־
B_RX1_OK        BIT     20H.2   ; ���ս�����־
TX1_Cnt         DATA    3BH     ; ���ͼ���
RX1_Cnt         DATA    3CH     ; ���ռ���
RX1_TimeOut     DATA    3DH     ; ��ʱ����
RX1_Buffer      DATA    40H     ;40 ~ 4FH ���ջ���

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


;******************** ������ **************************/
        ORG     0100H       ;reset
F_Main:
    CLR     A
    MOV     P0M1, A     ;����Ϊ׼˫���
    MOV     P0M0, A
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

    MOV     P1M0, A
    MOV     P1M1, #02H  ;���� P1.1 Ϊ ADC �����
    
    MOV     SP, #STACK_POIRTER
    MOV     PSW, #0
    USING   0       ;ѡ���0��R0~R7

;================= �û���ʼ������ ====================================
    MOV     display_index, #0
    MOV     R0, #LED8
    MOV     R2, #8
L_ClearLoop:
    MOV     @R0, #DIS_BLACK     ;�ϵ�����
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
    SETB    EA          ; �����ж�
    
    MOV     LED8+7, #1  ;   //��ʾPWMĬ��ֵ
    MOV     LED8+6, #2  ;
    MOV     LED8+5, #8  ;
    MOV     LED8+4, #DIS_BLACK  ;   //��λ����ʾ

    LCALL   F_ADC_config    ; ADC��ʼ��
    LCALL   F_PWM_Init      ; PWM��ʼ��
    LCALL   F_CMP_Init
    
    MOV     A, #1
    LCALL   F_UART1_config  ; ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer1��������.

    MOV     DPTR, #StringText1  ;Load string address to DPTR
    LCALL   F_SendString1       ;Send string

;=================== ��ѭ�� ==================================
L_Main_Loop:
    JNB     B_1ms,  L_Main_Loop     ;1msδ��
    CLR     B_1ms
    
    MOV     A, RX1_TimeOut
    JNZ     L_CheckRx1TimeOut   ;���ڿ��г�ʱ������0, 
L_QuitCheckRxTimeOut:
    LJMP    L_QuitProcessUart1  ;���ڿ��г�ʱ����Ϊ0, �˳�
L_CheckRx1TimeOut:
    DJNZ    RX1_TimeOut, L_QuitCheckRxTimeOut   ;���ڿ��г�ʱ����-1��0�˳�

    SETB    B_RX1_OK            ;���ڿ��г�ʱ, ���ս���, ����־
    MOV     A, RX1_Cnt
    JNZ     L_RxCntNotZero      ;���յ��ַ����ǿ�, ����
    LJMP    L_QuitProcessUart1  ;���յ��ַ�����, ���˳�

L_RxCntNotZero:
    ANL     A, #NOT 3   ;����Ϊ3λ����
    JZ      L_NumberLengthOk
    MOV     DPTR, #StringError3 ;Load string address to DPTR    "����! �����ַ�����! ����ռ�ձ�Ϊ0~256!"
    LCALL   F_SendString1       ;Send string
    LJMP    L_QuitCalculatePWM  ;�˳�
    
L_NumberLengthOk:
    MOV     R2, #0
    MOV     R3, #0
    MOV     R0, #RX1_Buffer
    MOV     R1, RX1_Cnt

L_GetUartPwmLoop:
    MOV     A, @R0
    CLR     C
    SUBB    A, #'0'             ;�����Ƿ�С��ASCII����0
    JNC     L_RxdataLargeThan0
    MOV     DPTR, #StringError1 ;Load string address to DPTR    "����! ���յ�С��0�������ַ�! ռ�ձ�Ϊ0~256!"
    LCALL   F_SendString1       ;Send string
    LJMP    L_QuitCalculatePWM
    
L_RxdataLargeThan0:
    MOV     A, @R0
    CLR     C
    SUBB    A, #03AH            ;�����Ƿ����ASCII����9
    JC      L_RxdataLessThan03A
    MOV     DPTR, #StringError4 ;Load string address to DPTR    "����! ���յ�����9�������ַ�! ռ�ձ�Ϊ0~256!"
    LCALL   F_SendString1       ;Send string
    LJMP    L_QuitCalculatePWM

L_RxdataLessThan03A:
    MOV     AR4, R2
    MOV     AR5, R3
    MOV     R7, #10
    LCALL   F_MUL16x8   ;(R4,R5)* R7 -->(R5,R6,R7)
    MOV     AR2, R6
    MOV     AR3, R7
    MOV     A, @R0      ;���� [R2 R3] = [R2 R3] * 10 + RX1_Buffer[i] - '0';
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
    MOV     DPTR, #StringError2 ;Load string address to DPTR    ����! ����ռ�ձȹ���, �벻Ҫ����256!
    LCALL   F_SendString1       ;Send string
    LJMP    L_QuitCalculatePWM

L_SetPWM_Right:

    LCALL   F_UpdatePwm     ;����ռ�ձ�
    MOV     A, R2
    JZ      L_PWM_LessTan256
    MOV     LED8+7, #2      ;ռ�ձ�Ϊ256,��ʾ256
    MOV     LED8+6, #5
    MOV     LED8+5, #6
    SJMP    L_SetPWM_OK
L_PWM_LessTan256:
    MOV     A, R3
    MOV     B, #100     ;��ʾ0~255ռ�ձ�
    DIV     AB
    MOV     LED8+7, A
    MOV     A, B
    MOV     B, #10
    DIV     AB
    MOV     LED8+6, A
    MOV     LED8+5, B
    MOV     A, LED8+7
    JNZ     L_SetPWM_OK
    MOV     LED8+7, #DIS_BLACK    ;��λ��0

L_SetPWM_OK:
    MOV     DPTR, #StringText2  ;Load string address to DPTR    "�Ѹ���ռ�ձ�!"
    LCALL   F_SendString1       ;Send string
L_QuitCalculatePWM:
    MOV     RX1_Cnt, #0
    CLR     B_RX1_OK
L_QuitProcessUart1:

;=================== ���300ms�Ƿ� ==================================
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

;================= 300ms�� ====================================
    MOV     msecond_L, #0   ;if(msecond >= 1000)
    MOV     msecond_H, #0

    MOV     A, #01H
    LCALL   F_Get_ADC12bitResult    ; ���ⲿ��ѹADC, ��ѯ��ʽ��һ��ADC, ����ֵ(R6 R7)����ADC���, == 4096 Ϊ����

    LCALL   F_HEX2_DEC      ;(R6 R7) HEX Change to DEC ---> (R3 R4 R5), use (R2~R7)
    
    MOV     A, R4           ;��ʾADCֵ
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

    MOV     A, LED8+3           ;��ʾ����Ч0
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
; ����: F_PWM_Init
; ����: PWM��ʼ������.
; ����: none
; ����: none.
; �汾: V1.0, 2021-3-3
;========================================================================
F_PWM_Init:
    ORL     P_SW2, #080H        ;ʹ�ܷ���XFR

    MOV     A,#00H              ;д CCMRx ǰ���������� CCxE �ر�ͨ��
    MOV     DPTR,#PWMB_CCER1
    MOVX    @DPTR,A
    MOV     A,#068H             ;���� CC1 Ϊ PWMģʽ1 ���
    MOV     DPTR,#PWMB_CCMR1
    MOVX    @DPTR,A
    MOV     A,#01H              ;ʹ�� CC5 ͨ��
    MOV     DPTR,#PWMB_CCER1
    MOVX    @DPTR,A

    MOV     A,#2                ;��������ʱ��
    MOV     DPTR,#PWMB_ARRH
    MOVX    @DPTR,A
    MOV     A,#0
    MOV     DPTR,#PWMB_ARRL
    MOVX    @DPTR,A
    MOV     A,#0                ;����ռ�ձ�ʱ��
    MOV     DPTR,#PWMB_CCR1H
    MOVX    @DPTR,A
    MOV     A,#128
    MOV     DPTR,#PWMB_CCR1L
    MOVX    @DPTR,A

    MOV     A,#01H              ;ʹ�� PWM5 ���
    MOV     DPTR,#PWMB_ENO
    MOVX    @DPTR,A
    MOV     A,#01H              ;�߼� PWM ͨ�� 5 �����ѡ��λ, 0x00:P2.0, 0x01:P1.7, 0x02:P0.0, 0x03:P7.4
    MOV     DPTR,#PWMB_PS
    MOVX    @DPTR,A
    MOV     A,#080H             ;ʹ�������
    MOV     DPTR,#PWMB_BKR
    MOVX    @DPTR,A

    MOV     DPTR,#PWMB_CR1
    MOVX    A,@DPTR
    ORL     A,#01H              ;��ʼ��ʱ
    MOVX    @DPTR,A

    ANL     P_SW2, #NOT 080H    ;��ֹ����XFR
    RET

;========================================================================
; ����: F_CMP_Init
; ����: CMP��ʼ������.
; ����: none
; ����: none.
; �汾: V1.0, 2021-3-3
;========================================================================
F_CMP_Init:
    ANL     P_SW2, #NOT 08H     ; ѡ��P3.4��Ϊ�Ƚ��������
    CLR     A
    ANL     A, #NOT 080H        ;�Ƚ����������
;    ORL     A, #080H            ;�Ƚ����������
    ANL     A, #NOT 040H        ;ʹ��0.1us�˲�
;    ORL     A, #040H            ;��ֹ0.1us�˲�
;    ANL     A, #NOT 03FH        ;�Ƚ������ֱ�����
    ORL     A, #010H            ;�Ƚ����������16��ȥ��ʱ�Ӻ����
    MOV     CMPCR2, A

    CLR     A
    ORL     A, #030H            ;ʹ�ܱȽ��������ж�
;    ANL     A, #NOT 020H        ;��ֹ�Ƚ����������ж�
;    ORL     A, #020H            ;ʹ�ܱȽ����������ж�
;    ANL     A, #NOT 010H        ;��ֹ�Ƚ����½����ж�
;    ORL     A, #010H            ;ʹ�ܱȽ����½����ж�
;    ANL     A, #NOT 08H         ;P3.7ΪCMP+�����
    ORL     A, #08H             ;ADC�����ΪCMP+�����
    ANL     A, #NOT 04H         ;�ڲ��ο���ѹΪCMP-�����
;    ORL     A, #04H             ;P3.6ΪCMP-�����
;    ANL     A, #NOT 02H         ;��ֹ�Ƚ������
    ORL     A, #02H             ;ʹ�ܱȽ������
    ORL     A, #080H            ;ʹ�ܱȽ���ģ��
    MOV     CMPCR1, A

;    MOV     CMPCR2, #10H        ; �Ƚ����������,ʹ��0.1us�˲�,�Ƚ����������16��ȥ��ʱ�Ӻ����
;    MOV     CMPCR1, #0B2H       ; ʹ�ܱȽ���ģ��,ʹ�ܱȽ��������ж�,P3.7ΪCMP+�����,�ڲ��ο���ѹΪCMP-�����,ʹ�ܱȽ������
    SETB    EA          ; �����ж�
    
    RET

;========================================================================
; ����: F_UpdatePwm
; ����: ����PWMռ�ձ�ֵ. 
; ����: [R2 R3]: pwmռ�ձ�ֵ.
; ����: none.
; �汾: V1.0, 2021-3-3
;========================================================================
F_UpdatePwm:
    ORL     P_SW2, #080H        ;ʹ�ܷ���XFR

    MOV     A, R2               ;����ռ�ձ�ʱ��
    MOV     DPTR, #PWMB_CCR1H
    MOVX    @DPTR, A
    MOV     A, R3
    MOV     DPTR, #PWMB_CCR1L
    MOVX    @DPTR, A

    ANL     P_SW2, #NOT 080H    ;��ֹ����XFR
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
;// ����: F_HEX2_DEC
;// ����: ��˫�ֽ�ʮ��������ת��ʮ����BCD��.
;// ����: (R6 R7): Ҫת����˫�ֽ�ʮ��������.
;// ����: (R3 R4 R5) = BCD��.
;// �汾: V1.0, 2013-10-22
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
    ORL     P_SW2, #080H        ; ʹ�ܷ���XFR
    MOV     A,#03FH             ; ���� ADC �ڲ�ʱ��ADC����ʱ�佨�������ֵ
    MOV     DPTR,#ADCTIM
    MOVX    @DPTR,A
    ANL     P_SW2, #NOT 080H    ; ��ֹ����XFR

    MOV     ADCCFG, #02FH       ; ����ת������Ҷ���ģʽ�� ADC ʱ��Ϊϵͳʱ��/2/16
    MOV     ADC_CONTR, #080H    ; ʹ�� ADC ģ��
    RET

;========================================================================
; ����: F_Get_ADC12bitResult
; ����: ��ѯ����һ��ADC���.
; ����: ACC: ѡ��Ҫת����ADC.
; ����: (R6 R7) = 12λADC���.
; �汾: V1.0, 2020-11-09
;========================================================================
F_Get_ADC12bitResult:   ;ACC - ͨ��0~7, ��ѯ��ʽ��һ��ADC, ����ֵ(R6 R7)����ADC���, == 4096 Ϊ����
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

    ANL     ADC_CONTR, #NOT 020H    ;�����ɱ�־

    MOV     A, ADC_RES      ;12λAD����ĸ�4λ��ADC_RES�ĵ�4λ����8λ��ADC_RESL��
    ANL     A, #0FH
    MOV     R6, A
    MOV     R7, ADC_RESL

L_QuitAdc:
    RET

;====================================================================================
StringText1:
    DB  "PWM��ADC���Գ���, ����ռ�ձ�Ϊ 0~256!",0DH,0AH,0
StringText2:
    DB  "�Ѹ���ռ�ձ�!",0DH,0AH,0
StringError1:
    DB  "����! ���յ�С��0�������ַ�! ռ�ձ�Ϊ0~256!",0DH,0AH,0
StringError2:
    DB  "����! ����ռ�ձȹ���, �벻Ҫ����256!",0DH,0AH,0
StringError3:
    DB  "����! �����ַ�����! ����ռ�ձ�Ϊ0~256!",0DH,0AH,0
StringError4:
    DB  "����! ���յ�����9�������ַ�! ռ�ձ�Ϊ0~256!",0DH,0AH,0


;========================================================================
; ����: F_SendString1
; ����: ����1�����ַ���������
; ����: DPTR: �ַ����׵�ַ.
; ����: none.
; �汾: VER1.0
; ����: 2014-11-28
; ��ע: 
;========================================================================
F_SendString1:
    CLR     A
    MOVC    A, @A+DPTR      ;Get current char
    JZ      L_SendEnd1      ;Check the end of the string
    SETB    B_TX1_Busy      ;��־����æ
    MOV     SBUF, A         ;����һ���ֽ�
    JB      B_TX1_Busy, $   ;�ȴ��������
    INC     DPTR            ;increment string ptr
    SJMP    F_SendString1       ;Check next
L_SendEnd1:
    RET

;========================================================================
; ����: F_SetTimer2Baudraye
; ����: ����Timer2�������ʷ�������
; ����: DPTR: Timer2����װֵ.
; ����: none.
; �汾: VER1.0
; ����: 2014-11-28
; ��ע: 
;========================================================================
F_SetTimer2Baudraye:    ; ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer1��������.
    ANL     AUXR, #NOT (1 SHL 4)    ; Timer stop    ������ʹ��Timer2����
    ANL     AUXR, #NOT (1 SHL 3)    ; Timer2 set As Timer
    ORL     AUXR, #(1 SHL 2)        ; Timer2 set as 1T mode
    MOV     T2H, DPH
    MOV     T2L, DPL
    ANL     IE2, #NOT (1 SHL 2)     ; ��ֹ�ж�
    ORL     AUXR, #(1 SHL 4)        ; Timer run enable
    RET

;========================================================================
; ����: F_UART1_config
; ����: UART1��ʼ��������
; ����: ACC: ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer1��������.
; ����: none.
; �汾: VER1.0
; ����: 2014-11-28
; ��ע: 
;========================================================================
F_UART1_config:
    CJNE    A, #2, L_Uart1NotUseTimer2
    ORL     AUXR, #0x01     ; S1 BRT Use Timer2;
    MOV     DPTR, #UART1_Baudrate
    LCALL   F_SetTimer2Baudraye
    SJMP    L_SetupUart1

L_Uart1NotUseTimer2:
    CLR     TR1                 ; Timer Stop    ������ʹ��Timer1����
    ANL     AUXR, #NOT 0x01     ; S1 BRT Use Timer1;
    ORL     AUXR, #(1 SHL 6)    ; Timer1 set as 1T mode
    ANL     TMOD, #NOT (1 SHL 6); Timer1 set As Timer
    ANL     TMOD, #NOT 0x30     ; Timer1_16bitAutoReload;
    MOV     TH1, #HIGH UART1_Baudrate
    MOV     TL1, #LOW  UART1_Baudrate
    CLR     ET1                 ; ��ֹ�ж�
    ANL     INT_CLKO, #NOT 0x02 ; �����ʱ��
    SETB    TR1

L_SetupUart1:
    SETB    REN     ; �������
    SETB    ES      ; �����ж�

    ANL     SCON, #0x3f
    ORL     SCON, #0x40     ; UART1ģʽ, 0x00: ͬ����λ���, 0x40: 8λ����,�ɱ䲨����, 0x80: 9λ����,�̶�������, 0xc0: 9λ����,�ɱ䲨����
;   SETB    PS      ; �����ȼ��ж�
    SETB    REN     ; �������
    SETB    ES      ; �����ж�

    ANL     P_SW1, #0x3f
    ORL     P_SW1, #0x00        ; UART1 switch to, 0x00: P3.0 P3.1, 0x40: P3.6 P3.7, 0x80: P1.6 P1.7, 0xC0: P4.3 P4.4
;   ORL     PCON2, #(1 SHL 4)   ; �ڲ���·RXD��TXD, ���м�, ENABLE,DISABLE

    CLR     B_TX1_Busy
    MOV     RX1_Cnt, #0;
    MOV     TX1_Cnt, #0;
    RET


;========================================================================
; ����: F_UART1_Interrupt
; ����: UART2�жϺ�����
; ����: nine.
; ����: none.
; �汾: VER1.0
; ����: 2014-11-28
; ��ע: 
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
    MOV     RX1_Cnt, #0     ; �����������
L_RxCntNotOver:
    MOV     A, #RX1_Buffer
    ADD     A, RX1_Cnt
    MOV     R0, A
    MOV     @R0, SBUF   ;����һ���ֽ�
    INC     RX1_Cnt
    MOV     RX1_TimeOut, #5
L_QuitUartRx:

    JNB     TI, L_QuitUartTx
    CLR     TI
    CLR     B_TX1_Busy      ; �������æ��־
L_QuitUartTx:

    POP     AR0
    POP     ACC
    POP     PSW
    RETI


; *********************** ��ʾ��س��� ****************************************
T_Display:                      ;��׼�ֿ�
;    0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
DB  03FH,006H,05BH,04FH,066H,06DH,07DH,007H,07FH,06FH,077H,07CH,039H,05EH,079H,071H
;  black  -    H    J    K    L    N    o    P    U    t    G    Q    r    M    y
DB  000H,040H,076H,01EH,070H,038H,037H,05CH,073H,03EH,078H,03dH,067H,050H,037H,06EH
;    0.   1.   2.   3.   4.   5.   6.   7.   8.   9.   -1
DB  0BFH,086H,0DBH,0CFH,0E6H,0EDH,0FDH,087H,0FFH,0EFH,046H

T_COM:
DB  001H,002H,004H,008H,010H,020H,040H,080H     ;   λ��


;//========================================================================
;// ����: F_DisplayScan
;// ����: ��ʾɨ���ӳ���
;// ����: none.
;// ����: none.
;// �汾: VER1.0
;// ����: 2013-4-1
;// ��ע: ����ACCC��PSW��, ���õ���ͨ�üĴ�������ջ
;//========================================================================
F_DisplayScan:
    PUSH    DPH     ;DPH��ջ
    PUSH    DPL     ;DPL��ջ
    PUSH    00H     ;R0 ��ջ
    
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
    MOV     display_index, #0;  ;8λ������0
L_QuitDisplayScan:
    POP     00H     ;R0 ��ջ
    POP     DPL     ;DPL��ջ
    POP     DPH     ;DPH��ջ
    RET


;*******************************************************************
;**************** �жϺ��� ***************************************************

F_Timer0_Interrupt: ;Timer0 1ms�жϺ���
    PUSH    PSW     ;PSW��ջ
    PUSH    ACC     ;ACC��ջ

    LCALL   F_DisplayScan   ; 1msɨ����ʾһλ
    SETB    B_1ms           ; 1ms��־
    
    POP     ACC     ;ACC��ջ
    POP     PSW     ;PSW��ջ
    RETI

;========================================================================
; ����: F_CMP_Interrupt
; ����: �Ƚ����жϺ���.
; ����: non.
; ����: non.
; �汾: V1.0, 2021-3-3
;========================================================================
F_CMP_Interrupt:
    PUSH    PSW
    PUSH    ACC
    PUSH    AR2

    ANL     CMPCR1, #NOT 040H      ; ���жϱ�־
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

