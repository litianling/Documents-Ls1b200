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

;����3ȫ˫���жϷ�ʽ�շ�ͨѶ����.

;��������Ϊ��115200,8,n,1.

;ͨ��PC��MCU��������, MCU�յ���ͨ�����ڰ��յ�������ԭ������.

;�ö�ʱ���������ʷ�����������ʹ��1Tģʽ(���ǵͲ�������12T)����ѡ��ɱ�������������ʱ��Ƶ�ʣ�����߾��ȡ�

;����ʱ, ѡ��ʱ�� 22.1184MHz����Ҫ�ı�, ���޸������"������ʱ��"��ֵ�����±���.

;******************************************/

;/****************************** �û������ ***********************************/

;UART3_Baudrate EQU     (-4608) ;   600bps @ 11.0592MHz
;UART3_Baudrate EQU     (-2304) ;  1200bps @ 11.0592MHz UART3_Baudrate = 65536UL - ((MAIN_Fosc / 4) / Baudrate)
;UART3_Baudrate EQU     (-1152) ;  2400bps @ 11.0592MHz
;UART3_Baudrate EQU     (-576)  ;  4800bps @ 11.0592MHz
;UART3_Baudrate EQU     (-288)  ;  9600bps @ 11.0592MHz
;UART3_Baudrate EQU     (-144)  ; 19200bps @ 11.0592MHz
;UART3_Baudrate EQU     (-72)   ; 38400bps @ 11.0592MHz
;UART3_Baudrate EQU     (-48)   ; 57600bps @ 11.0592MHz
;UART3_Baudrate EQU      (-24)   ;115200bps @ 11.0592MHz

;UART3_Baudrate EQU     (-7680) ;   600bps @ 18.432MHz
;UART3_Baudrate EQU     (-3840) ;  1200bps @ 18.432MHz
;UART3_Baudrate EQU     (-1920) ;  2400bps @ 18.432MHz
;UART3_Baudrate EQU     (-960)  ;  4800bps @ 18.432MHz
;UART3_Baudrate EQU     (-480)  ;  9600bps @ 18.432MHz
;UART3_Baudrate EQU     (-240)  ; 19200bps @ 18.432MHz
;UART3_Baudrate EQU     (-120)  ; 38400bps @ 18.432MHz
;UART3_Baudrate EQU     (-80)   ; 57600bps @ 18.432MHz
;UART3_Baudrate EQU     (-40)   ;115200bps @ 18.432MHz

;UART3_Baudrate EQU     (-9216) ;   600bps @ 22.1184MHz
;UART3_Baudrate EQU     (-4608) ;  1200bps @ 22.1184MHz
;UART3_Baudrate EQU     (-2304) ;  2400bps @ 22.1184MHz
;UART3_Baudrate EQU     (-1152) ;  4800bps @ 22.1184MHz
;UART3_Baudrate EQU     (-576)  ;  9600bps @ 22.1184MHz
;UART3_Baudrate EQU     (-288)  ; 19200bps @ 22.1184MHz
;UART3_Baudrate EQU     (-144)  ; 38400bps @ 22.1184MHz
;UART3_Baudrate EQU     (-96)   ; 57600bps @ 22.1184MHz
UART3_Baudrate  EQU    (-48)   ;115200bps @ 22.1184MHz

;UART3_Baudrate EQU     (-6912) ; 1200bps @ 33.1776MHz
;UART3_Baudrate EQU     (-3456) ; 2400bps @ 33.1776MHz
;UART3_Baudrate EQU     (-1728) ; 4800bps @ 33.1776MHz
;UART3_Baudrate EQU     (-864)  ; 9600bps @ 33.1776MHz
;UART3_Baudrate EQU     (-432)  ;19200bps @ 33.1776MHz
;UART3_Baudrate EQU     (-216)  ;38400bps @ 33.1776MHz
;UART3_Baudrate EQU     (-144)  ;57600bps @ 33.1776MHz
;UART3_Baudrate EQU     (-72)   ;115200bps @ 33.1776MHz


;*******************************************************************
;*******************************************************************
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


INT_CLKO    DATA    0x8F
P_SW1       DATA    0A2H
P_SW2       DATA    0BAH
IE2         DATA    0AFH
AUXR        DATA    08EH
T2H         DATA    0D6H
T2L         DATA    0D7H
T3H         DATA    0D4H
T3L         DATA    0D5H
T4T3M       DATA    0D1H

S3CON       DATA    0ACH
S3BUF       DATA    0ADH

RX3_Lenth   EQU     32      ; ���ڽ��ջ��峤��

B_TX3_Busy  BIT     20H.0   ; ����æ��־
TX3_Cnt     DATA    30H     ; ���ͼ���
RX3_Cnt     DATA    31H     ; ���ռ���
RX3_Buffer  DATA    40H     ;40 ~ 5FH ���ջ���

STACK_POIRTER   EQU     0D0H    ;��ջ��ʼ��ַ

;*******************************************************************
;*******************************************************************
        ORG     0000H       ;reset
        LJMP    F_Main

        ORG     008BH       ;17  UART3 interrupt
        LJMP    F_UART3_Interrupt


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
    MOV     A, #1
    LCALL   F_UART3_config  ; ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer1��������.
    SETB    EA      ; ����ȫ���ж�
    
    MOV     DPTR, #TestString3  ;Load string address to DPTR
    LCALL   F_SendString3       ;Send string

;=================== ��ѭ�� ==================================
L_MainLoop:
    MOV     A, TX3_Cnt
    CJNE    A, RX3_Cnt, L_ReturnData
    SJMP    L_QuitCheckRx3
L_ReturnData:                   ; �յ�������
    JB      B_TX3_Busy, L_QuitCheckRx3      ; ����æ�� �˳�
    SETB    B_TX3_Busy          ; ��־����æ
    MOV     A, #RX3_Buffer
    ADD     A, TX3_Cnt
    MOV     R0, A
    MOV     S3BUF, @R0       ; ��һ���ֽ�
    INC     TX3_Cnt
    MOV     A, TX3_Cnt
    CJNE    A, #RX3_Lenth, L_QuitCheckRx3
    MOV     TX3_Cnt, #0     ; �����������
L_QuitCheckRx3:
    LJMP    L_MainLoop
;===================================================================


TestString3:                    ;Test string
    DB  "STC8H8K64U Uart3 Test !",0DH,0AH,0

;========================================================================
; ����: F_SendString3
; ����: ����3�����ַ���������
; ����: DPTR: �ַ����׵�ַ.
; ����: none.
; �汾: VER1.0
; ����: 2020-11-05
; ��ע: 
;========================================================================
F_SendString3:
    CLR     A
    MOVC    A, @A+DPTR      ;Get current char
    JZ      L_SendEnd3      ;Check the end of the string
    SETB    B_TX3_Busy      ;��־����æ
    MOV     S3BUF, A         ;����һ���ֽ�
    JB      B_TX3_Busy, $   ;�ȴ��������
    INC     DPTR            ;increment string ptr
    SJMP    F_SendString3   ;Check next
L_SendEnd3:
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
; ����: F_UART3_config
; ����: UART3��ʼ��������
; ����: ACC: ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer3��������.
; ����: none.
; �汾: VER1.0
; ����: 2020-11-05
; ��ע: 
;========================================================================
F_UART3_config:
    CJNE    A, #2, L_Uart1NotUseTimer2
    ORL     S3CON, #0x10     ; S3 BRT Use Timer2;
    MOV     DPTR, #UART3_Baudrate
    LCALL   F_SetTimer2Baudraye
    SJMP    L_SetupUart1

L_Uart1NotUseTimer2:
    MOV     S3CON, #0x50     ; 8λ����, ʹ��Timer3�������ʷ�����, �������
    MOV     T3H, #HIGH UART3_Baudrate
    MOV     T3L, #LOW  UART3_Baudrate
    MOV     T4T3M, #0x0a

L_SetupUart1:
    ANL     P_SW2, #NOT 0x02  ; UART3 switch to P0.0 P0.1
;    ORL     P_SW2, #0x02     ; UART3 switch to P5.0 P5.1
    ORL     IE2, #0x08        ; ����UART3�ж�

    CLR     B_TX3_Busy
    MOV     RX3_Cnt, #0;
    MOV     TX3_Cnt, #0;
    RET


;========================================================================
; ����: F_UART3_Interrupt
; ����: UART3�жϺ�����
; ����: nine.
; ����: none.
; �汾: VER1.0
; ����: 2020-11-05
; ��ע: 
;========================================================================
F_UART3_Interrupt:
    PUSH    PSW
    PUSH    ACC
    PUSH    AR0
    
    MOV     A, S3CON
    JNB     ACC.0, L_QuitUartRx
    ANL     S3CON, #NOT 1;
    MOV     A, #RX3_Buffer
    ADD     A, RX3_Cnt
    MOV     R0, A
    MOV     @R0, S3BUF   ;����һ���ֽ�
    INC     RX3_Cnt
    MOV     A, RX3_Cnt
    CJNE    A, #RX3_Lenth, L_QuitUartRx
    MOV     RX3_Cnt, #0     ; �����������
L_QuitUartRx:

    MOV     A, S3CON
    JNB     ACC.1, L_QuitUartTx
    ANL     S3CON, #NOT 2;
    CLR     B_TX3_Busy      ; �������æ��־
L_QuitUartTx:

    POP     AR0
    POP     ACC
    POP     PSW
    RETI

    END

