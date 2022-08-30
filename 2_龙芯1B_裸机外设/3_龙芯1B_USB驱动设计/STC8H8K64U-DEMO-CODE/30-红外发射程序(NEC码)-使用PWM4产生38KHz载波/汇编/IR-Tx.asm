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

;�û������ں궨���иı�MCU��ʱ��Ƶ��. ��Χ 8MHZ ~ 33MHZ.

;���ⷢ�����ģ���г�����������NEC�ı��롣

;�û������ں궨����ָ���û���.

;ʹ��PWM4����38KHZ�ز�, 1/3ռ�ձ�, ÿ��38KHZ���ڷ���ܷ���9us,�ر�16.3us.

;ʹ�ÿ������ϵ�16��IOɨ�谴��, MCU��˯��, ����ɨ�谴��.

;��������, ��һ֡Ϊ����, �����֡Ϊ�ظ�֡,��������, ���嶨�������вο�NEC�ı�������.

;���ͷź�, ֹͣ����.

;******************************************/

;/****************************** �û������ ***********************************/

STACK_POIRTER   EQU     0D0H    ;��ջ��ʼ��ַ
Fosc_KHZ        EQU     24000   ;24000KHZ

;*******************************************************************
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

;*******************************************************************
AUXR        DATA 08EH
P4          DATA 0C0H
P5          DATA 0C8H
P6          DATA 0E8H
P7          DATA 0F8H

P_SW1       DATA    0A2H
P_SW2       DATA    0BAH
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

;*************  IO�ڶ���    **************/


;*************  ���ر�������    **************/

IO_KeyState     DATA    30H ; IO���м�״̬����
IO_KeyState1    DATA    31H
IO_KeyHoldCnt   DATA    32H ; IO�����¼�ʱ
KeyCode         DATA    33H ; ���û�ʹ�õļ���, 1~16ΪADC���� 17~32ΪIO��


/*************  ���ⷢ����ر���    **************/
#define User_code   0xFF00      //��������û���

P_IR_TX   BIT P1.6  ;������ⷢ�Ͷ˿�

FLAG0   DATA    0x20
B_Space BIT FLAG0.0 ;���Ϳ���(��ʱ)��־

tx_cntH     DATA    0x36    ;���ͻ���е��������(����38KHZ������������Ӧʱ��), ����Ƶ��Ϊ38KHZ, ����26.3us
tx_cntL     DATA    0x37    
TxTime      DATA    0x38    ;����ʱ��


;*******************************************************************
;*******************************************************************
        ORG     0000H               ;reset
        LJMP    F_Main

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

    LCALL   F_PWM_Init      ;��ʼ��PWM
    SETB    P_IR_TX
    
    SETB    EA              ;�����ж�
    
    MOV     KeyCode, #0

;=================== ��ѭ�� ==================================
L_Main_Loop:
    MOV     A, #30
    LCALL   F_delay_ms      ;��ʱ30ms
    LCALL   F_IO_KeyScan    ;ɨ�����

    MOV     A, KeyCode
    JZ      L_Main_Loop     ;�޼�ѭ��
    MOV     TxTime, #0      ;
                            ;һ֡������С���� = 9 + 4.5 + 0.5625 + 24 * 1.125 + 8 * 2.25 = 59.0625 ms
                            ;һ֡������󳤶� = 9 + 4.5 + 0.5625 + 8 * 1.125 + 24 * 2.25 = 77.0625 ms
    MOV     tx_cntH, #HIGH 342      ;��Ӧ9ms��ͬ��ͷ        9ms
    MOV     tx_cntL, #LOW  342
    LCALL   F_IR_TxPulse

    MOV     tx_cntH, #HIGH 171      ;��Ӧ4.5ms��ͬ��ͷ���  4.5ms
    MOV     tx_cntL, #LOW  171
    LCALL   F_IR_TxSpace

    MOV     tx_cntH, #HIGH 21       ;��������           0.5625ms
    MOV     tx_cntL, #LOW  21
    LCALL   F_IR_TxPulse

    MOV     A, #LOW  User_code  ;���û�����ֽ�
    LCALL   F_IR_TxByte
    MOV     A, #HIGH User_code  ;���û�����ֽ�
    LCALL   F_IR_TxByte
    MOV     A, KeyCode          ;������
    LCALL   F_IR_TxByte
    MOV     A, KeyCode          ;�����ݷ���
    CPL     A
    LCALL   F_IR_TxByte

    MOV     A, TxTime
    CLR     C
    SUBB    A, #56      ;һ֡�����77ms����, �����Ļ�,����ʱ��      108ms
    JNC     L_ADJ_Time

    CLR     c
    MOV     A, #56
    SUBB    A, TxTime
    MOV     TxTime, A
    RRC     A
    RRC     A
    RRC     A
    ANL     A, #0x1F
    ADD     A, TxTime
    LCALL   F_delay_ms      ;TxTime = 56 - TxTime;  TxTime = TxTime + TxTime / 8;   delay_ms(TxTime);
L_ADJ_Time:
    MOV     A, #31          ;delay_ms(31)
    LCALL   F_delay_ms
        
L_WaitKeyRelease:
    MOV     A, IO_KeyState
    JZ      L_ClearKeyCode

                            ;��δ�ͷ�, �����ظ�֡(������)
    MOV     tx_cntH, #HIGH 342      ;��Ӧ9ms��ͬ��ͷ        9ms
    MOV     tx_cntL, #LOW  342
    LCALL   F_IR_TxPulse

    MOV     tx_cntH, #HIGH 86       ;��Ӧ2.25ms��ͬ��ͷ��� 2.25ms
    MOV     tx_cntL, #LOW  86
    LCALL   F_IR_TxSpace

    MOV     tx_cntH, #HIGH 21       ;��������           0.5625ms
    MOV     tx_cntL, #LOW  21
    LCALL   F_IR_TxPulse

    MOV     A, #96          ;delay_ms(96)
    LCALL   F_delay_ms
    LCALL   F_IO_KeyScan    ;ɨ�����
    SJMP    L_WaitKeyRelease

L_ClearKeyCode:
    MOV KeyCode, #0

    LJMP    L_Main_Loop
;===================================================================


;========================================================================
; ����: F_delay_ms
; ����: ��ʱ�ӳ���
; ����: ACC: ��ʱms��.
; ����: none.
; �汾: VER1.0
; ����: 2013-4-1
; ��ע: ����ACCC��PSW��, ���õ���ͨ�üĴ�������ջ����, �˳�ʱ�ָ�ԭ�����ݲ��ı�.
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
; ����: F_IO_KeyDelay
; ����: ���м�ɨ�����.
; ����: none
; ����: ��������, KeyCodeΪ��0����.
; �汾: V1.0, 2013-11-22
;========================================================================
;/*****************************************************
;   ���м�ɨ�����
;   ʹ��XY����4x4���ķ�����ֻ�ܵ������ٶȿ�
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
    PUSH    03H     ;R3��ջ
    MOV     R3, #60
    DJNZ    R3, $   ; (n * 4) T
    POP     03H     ;R3��ջ
    RET

F_IO_KeyScan:
    PUSH    06H     ;R6��ջ
    PUSH    07H     ;R7��ջ

    MOV     R6, IO_KeyState1    ; ������һ��״̬

    MOV     P0, #0F0H       ;X�ͣ���Y
    LCALL   F_IO_KeyDelay       ;delay about 250T
    MOV     A, P0
    ANL     A, #0F0H
    MOV     IO_KeyState1, A     ; IO_KeyState1 = P0 & 0xf0

    MOV     P0, #0FH        ;Y�ͣ���X
    LCALL   F_IO_KeyDelay       ;delay about 250T
    MOV     A, P0
    ANL     A, #0FH
    ORL     A, IO_KeyState1         ; IO_KeyState1 |= (P0 & 0x0f)
    MOV     IO_KeyState1, A
    XRL     IO_KeyState1, #0FFH     ; IO_KeyState1 ^= 0xff; ȡ��

    MOV     A, R6                   ;if(j == IO_KeyState1), �������ζ����
    CJNE    A, IO_KeyState1, L_QuitCheckIoKey   ;�����, jmp
    
    MOV     R6, IO_KeyState     ;�ݴ�IO_KeyState
    MOV     IO_KeyState, IO_KeyState1
    MOV     A, IO_KeyState
    JZ      L_NoIoKeyPress      ; if(IO_KeyState != 0), �м�����

    MOV     A, R6   
    JZ      L_CalculateIoKey    ;if(R6 == 0)    F0 = 1; ��һ�ΰ���
    MOV     A, R6   
    CJNE    A, IO_KeyState, L_QuitCheckIoKey    ; if(j != IO_KeyState), jmp
    
    INC     IO_KeyHoldCnt   ; if(++IO_KeyHoldCnt >= 20),    1����ؼ�
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
    
    MOV     A, R6       ;KeyCode = (j - 1) * 4 + T_KeyTable[IO_KeyState & 0x0f] + 16;   //������룬17~32
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
    POP     07H     ;R7��ջ
    POP     06H     ;R6��ջ
    RET

;========================================================================
; ����: F_IR_TxPulse
; ����: �������庯��.
; ����: tx_cntH, tx_cntL: Ҫ���͵�38K������
; ����: none.
; �汾: V1.0, 2013-11-22
;========================================================================
F_IR_TxPulse:
    ORL     P_SW2, #080H        ;ʹ�ܷ���XFR
    CLR     B_Space

    MOV     A,#00                 ;д CCMRx ǰ���������� CCxE �ر�ͨ��
    MOV     DPTR,#PWMA_CCER2
    MOVX    @DPTR,A
    MOV     A,#060H               ;���� PWM4 ģʽ1 ���
    MOV     DPTR,#PWMA_CCMR4
    MOVX    @DPTR,A
    MOV     A,#0B0H               ;ʹ�� CC4E ͨ��, �͵�ƽ��Ч
    MOV     DPTR,#PWMA_CCER2
    MOVX    @DPTR,A
    MOV     A,#010H               ;ʹ�ܲ���/�Ƚ� 4 �ж�
    MOV     DPTR,#PWMA_IER
    MOVX    @DPTR,A

    JNB     B_Space, $   ;�ȴ�����
    ANL     P_SW2, #NOT 080H    ;��ֹ����XFR
    RET

;========================================================================
; ����: F_IR_TxSpace
; ����: ���Ϳ��к���.
; ����: tx_cntH, tx_cntL: Ҫ���͵�38K������
; ����: none.
; �汾: V1.0, 2013-11-22
;========================================================================
F_IR_TxSpace:
    ORL     P_SW2, #080H        ;ʹ�ܷ���XFR
    CLR     B_Space

    MOV     A,#00                 ;д CCMRx ǰ���������� CCxE �ر�ͨ��
    MOV     DPTR,#PWMA_CCER2
    MOVX    @DPTR,A
    MOV     A,#040H               ;���� PWM4 ǿ��Ϊ��Ч��ƽ
    MOV     DPTR,#PWMA_CCMR4
    MOVX    @DPTR,A
    MOV     A,#0B0H               ;ʹ�� CC4E ͨ��, �͵�ƽ��Ч
    MOV     DPTR,#PWMA_CCER2
    MOVX    @DPTR,A
    MOV     A,#010H               ;ʹ�ܲ���/�Ƚ� 4 �ж�
    MOV     DPTR,#PWMA_IER
    MOVX    @DPTR,A

    JNB     B_Space, $   ;�ȴ�����
    ANL     P_SW2, #NOT 080H    ;��ֹ����XFR
    RET

;========================================================================
; ����: F_IR_TxByte
; ����: ����һ���ֽں���.
; ����: ACC: Ҫ���͵��ֽ�
; ����: none.
; �汾: V1.0, 2013-11-22
;========================================================================
F_IR_TxByte:
    PUSH    AR4
    PUSH    AR5

    MOV     R4, #8
    MOV     R5, A
L_IR_TxByteLoop:
    MOV     A, R5
    JNB     ACC.0, L_IR_TxByte_0
    MOV     tx_cntH, #HIGH 63       ;��������1
    MOV     tx_cntL, #LOW  63
    LCALL   F_IR_TxSpace
    INC     TxTime          ;TxTime += 2;   //����1��Ӧ 1.6875 + 0.5625 ms
    INC     TxTime
    SJMP    L_IR_TxByte_Pause
L_IR_TxByte_0:
    MOV     tx_cntH, #HIGH 21       ;��������0
    MOV     tx_cntL, #LOW  21
    LCALL   F_IR_TxSpace
    INC     TxTime          ;����0��Ӧ 0.5625 + 0.5625 ms
L_IR_TxByte_Pause:
    MOV     tx_cntH, #HIGH 21       ;��������
    MOV     tx_cntL, #LOW  21
    LCALL   F_IR_TxPulse        ;���嶼��0.5625ms
    MOV     A, R5
    RR      A               ;��һ��λ
    MOV     R5, A
    DJNZ    R4, L_IR_TxByteLoop
    POP     AR5
    POP     AR4
    
    RET

;========================================================================
; ����: F_PWM_Init
; ����: PWM��ʼ������.
; ����: none
; ����: none.
; �汾: V1.0, 2013-11-22
;========================================================================
F_PWM_Init:
    ORL     P_SW2, #080H        ;ʹ�ܷ���XFR

    MOV     A,#00H              ;д CCMRx ǰ���������� CCxE �ر�ͨ��
    MOV     DPTR,#PWMA_CCER2
    MOVX    @DPTR,A
    MOV     A,#060H             ;���� PWM4 ģʽ1 ���
    MOV     DPTR,#PWMA_CCMR4
    MOVX    @DPTR,A

    MOV     A,#2                ;��������ʱ��
    MOV     DPTR,#PWMA_ARRH
    MOVX    @DPTR,A
    MOV     A,#077H
    MOV     DPTR,#PWMA_ARRL
    MOVX    @DPTR,A
    MOV     A,#0                ;����ռ�ձ�ʱ��
    MOV     DPTR,#PWMA_CCR4H
    MOVX    @DPTR,A
    MOV     A,#210
    MOV     DPTR,#PWMA_CCR4L
    MOVX    @DPTR,A

    MOV     A,#040H             ;ʹ�� PWM4P ���
    MOV     DPTR,#PWMA_ENO
    MOVX    @DPTR,A
    MOV     A,#00H              ;�߼� PWM ͨ�� 4P �����ѡ��λ, 0x00:P1.6, 0x40:P2.6, 0x80:P6.6, 0xC0:P3.4
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
; ����: F_PWMA_Interrupt
; ����: PWMA�жϴ������.
; ����: None
; ����: none.
; �汾: V1.0, 2012-11-22
;========================================================================
F_PWMA_Interrupt:
    PUSH    PSW
    PUSH    ACC
    PUSH    P_SW2
    ORL     P_SW2, #080H  ;ʹ�ܷ���XFR

    MOV     DPTR,#PWMA_SR1 ;���ӻ�״̬
    MOVX    A,@DPTR
    JNB     ACC.4,F_PWMA_QuitInt
    CLR     A
    MOVX    @DPTR,A

    MOV     A, tx_cntL
    DEC     tx_cntL       ;tx_cnt - 1
    JNZ     $+4
    DEC     tx_cntH
    
    MOV     A, tx_cntL
    JNZ     F_PWMA_QuitInt
    MOV     A, tx_cntH
    JNZ     F_PWMA_QuitInt

    MOV     A,#00                 ;д CCMRx ǰ���������� CCxE �ر�ͨ��
    MOV     DPTR,#PWMA_CCER2
    MOVX    @DPTR,A
    MOV     A,#040H               ;���� PWM4 ǿ��Ϊ��Ч��ƽ
    MOV     DPTR,#PWMA_CCMR4
    MOVX    @DPTR,A
    MOV     A,#0B0H               ;ʹ�� CC4E ͨ��, �͵�ƽ��Ч
    MOV     DPTR,#PWMA_CCER2
    MOVX    @DPTR,A
    MOV     A,#00H                ;�ر��ж�
    MOV     DPTR,#PWMA_IER
    MOVX    @DPTR,A

    SETB    B_Space        ;���ý�����־

F_PWMA_QuitInt:
    POP     P_SW2
    POP     ACC
    POP     PSW
    RETI

    END

