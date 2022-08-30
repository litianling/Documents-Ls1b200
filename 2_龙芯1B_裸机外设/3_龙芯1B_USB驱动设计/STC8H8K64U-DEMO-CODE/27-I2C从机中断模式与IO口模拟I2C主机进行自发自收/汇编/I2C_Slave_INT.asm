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

;�ڲ�����I2C�������߿��������ӻ�ģʽ��SCL->P3.2, SDA->P3.3;
;IO��ģ��I2C������ģʽ��SCL->P0.0, SDA->P0.1;
;ͨ���ⲿ�������� P0.0->P3.2, P0.1->P3.3��ʵ��I2C�Է����չ��ܡ�

;��STC��MCU��IO��ʽ����8λ����ܡ�
;ʹ��Timer0��16λ�Զ���װ������1ms����,�������������������,�û��޸�MCU��ʱ��Ƶ��ʱ,�Զ���ʱ��1ms.
;������ÿ���Ӽ�1, ������ΧΪ0~9999.

;��ʾЧ��Ϊ: �ϵ������ÿ���ӷ���һ�μ������ݣ��������4�����������ʾ�������ݣ��ӻ����յ����ݺ����ұ�4���������ʾ��

;����ʱ, ѡ��ʱ�� 24MHZ (�û��������޸�Ƶ��).

;******************************************/

;/****************************** �û������ ***********************************/

Fosc_KHZ    EQU 24000   ;24000KHZ

STACK_POIRTER   EQU     0D0H    ;��ջ��ʼ��ַ

Timer0_Reload   EQU     (65536 - Fosc_KHZ)  ; Timer 0 �ж�Ƶ��, 1000��/��

DIS_DOT         EQU     020H
DIS_BLACK       EQU     010H
DIS_            EQU     011H

SLAW            EQU     05AH
SLAR            EQU     05BH

SDA             BIT     P0.1
SCL             BIT     P0.0
;*******************************************************************
;*******************************************************************
P_SW2       DATA 0BAH
AUXR        DATA 08EH
P4          DATA 0C0H
P5          DATA 0C8H
P6          DATA 0E8H
P7          DATA 0F8H

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

I2CCFG       XDATA   0FE80H
I2CMSCR      XDATA   0FE81H
I2CMSST      XDATA   0FE82H
I2CSLCR      XDATA   0FE83H
I2CSLST      XDATA   0FE84H
I2CSLADR     XDATA   0FE85H
I2CTXD       XDATA   0FE86H
I2CRXD       XDATA   0FE87H
I2CMSAUX     XDATA   0FE88H


;*************  IO�ڶ���    **************/


;*************  ���ر�������    **************/
B_1ms           BIT     20H.0   ;   1ms��־
DisplayFlag     BIT     20H.1   ;   ��ʾ��־
ISDA            BIT     20H.6
ISMA            BIT     20H.7

ADDR            DATA    21H

LED8            DATA    30H     ;   ��ʾ���� 30H ~ 37H
display_index   DATA    38H     ;   ��ʾλ����

msecond_H       DATA    39H     ;
msecond_L       DATA    3AH     ;
Test_cnt_H      DATA    3BH     ;
Test_cnt_L      DATA    3CH     ;


;*******************************************************************
;*******************************************************************
        ORG     0000H               ;reset
        LJMP    F_Main

        ORG     000BH               ;1  Timer0 interrupt
        LJMP    F_Timer0_Interrupt

        ORG     00C3H               ;24  I2C interrupt
        LJMP    F_I2C_Interrupt


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
    MOV     P4M1, A     ;����Ϊ׼˫���
    MOV     P4M0, A
    MOV     P5M1, A     ;����Ϊ׼˫���
    MOV     P5M0, A
    MOV     P6M1, A     ;����Ϊ׼˫���
    MOV     P6M0, A
    MOV     P7M1, A     ;����Ϊ׼˫���
    MOV     P7M0, A

    MOV     P3M0, A
    MOV     A, #04H
    MOV     P3M1, A     ;SCL����Ϊ����ڣ�SDA����Ϊ׼˫���
    
    MOV     SP, #STACK_POIRTER
    MOV     PSW, #0
    USING   0       ;ѡ���0��R0~R7

;================= �û���ʼ������ ====================================

    MOV     display_index, #0
    CLR     DisplayFlag
    MOV     R0, #LED8
    MOV     R2, #8
L_ClearLoop:
    MOV     @R0, #DIS_          ;�ϵ�ȫ����ʾ-
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
    SETB    EA          ; �����ж�

;=================== ��ѭ�� ==================================
L_Main_Loop:
    JNB     DisplayFlag, L_Is1ms    ;�յ�����
    CLR     DisplayFlag
    
    CLR     A
    MOV     R0,A
    MOVX    A,@R0
    MOV     LED8+0, A
    INC     R0
    MOVX    A,@R0
    MOV     LED8+1, A
    INC     R0
    MOVX    A,@R0
    MOV     LED8+2, A
    INC     R0
    MOVX    A,@R0
    MOV     LED8+3, A
	
L_Is1ms:
    JNB     B_1ms, L_Main_Loop     ;1msδ��
    CLR     B_1ms
    
;=================== ���1s�Ƿ� ==================================
    INC     msecond_L       ;msecond + 1
    MOV     A, msecond_L
    JNZ     $+4
    INC     msecond_H
    
    CLR     C
    MOV     A, msecond_L    ;msecond - 1000
    SUBB    A, #LOW 1000
    MOV     A, msecond_H
    SUBB    A, #HIGH 1000
    JNC     L_1sIsGood
    LJMP    L_Main_Loop
L_1sIsGood:

;================= 1s�� ====================================
    MOV     msecond_L, #0   ;if(msecond >= 1000)
    MOV     msecond_H, #0

;=================== ����Ƿ�>= 10000 ==============================
    INC     Test_cnt_L       ;Test_cnt + 1
    MOV     A, Test_cnt_L
    JNZ     $+4
    INC     Test_cnt_H
    
    CLR     C
    MOV     A, Test_cnt_L    ;Test_cnt - 10000
    SUBB    A, #LOW 10000
    MOV     A, Test_cnt_H
    SUBB    A, #HIGH 10000
    JC      L_LessThen10000
    MOV     Test_cnt_L, #0   ;if(Test_cnt >= 10000)
    MOV     Test_cnt_H, #0
L_LessThen10000:
    MOV     R6, Test_cnt_H
    MOV     R7, Test_cnt_L
    LCALL   F_HEX2_DEC      ;(R6 R7) HEX Change to DEC ---> (R3 R4 R5), use (R2~R7)
    
    MOV     A, R4           ;��ʾ
    SWAP    A
    ANL     A, #0x0F
    MOV     LED8+7, A
    MOV     A, R4
    ANL     A, #0x0F
    MOV     LED8+6, A
    MOV     A, R5
    SWAP    A
    ANL     A, #0x0F
    MOV     LED8+5, A
    MOV     A, R5
    ANL     A, #0x0F
    MOV     LED8+4, A

    MOV     A, LED8+7           ;��ʾ����Ч0
    JNZ     L_QuitProcessADC
    MOV     LED8+7, #DIS_BLACK
    MOV     A, LED8+6
    JNZ     L_QuitProcessADC
    MOV     LED8+6, #DIS_BLACK
    MOV     A, LED8+5
    JNZ     L_QuitProcessADC
    MOV     LED8+5, #DIS_BLACK
L_QuitProcessADC:
    LCALL   WriteNbyte
    LJMP    L_Main_Loop

;========================================================================
; ����: F_I2C_Init
; ����: I2C��ʼ������.
; ����: none
; ����: none.
; �汾: V1.0, 2021-3-4
;========================================================================
F_I2C_Init:
    ORL     P_SW2, #080H        ;ʹ�ܷ���XFR

    ORL     P_SW2, #030H        ;I2C���ܽ�ѡ��00:P1.5,P1.4; 01:P2.5,P2.4; 11:P3.2,P3.3

    SETB    ISDA
    SETB    ISMA
    CLR     A
    MOV     ADDR, A
    MOV     R0,A
    MOVX    A,@R0
    MOV     DPTR,#I2CTXD
    MOVX    @DPTR,A

    MOV     A,#080H
    MOV     DPTR,#I2CCFG        ;ʹ��I2C�ӻ�ģʽ
    MOVX    @DPTR,A

    MOV     A,#05AH
    MOV     DPTR,#I2CSLADR      ;���ôӻ��豸��ַΪ5A
    MOVX    @DPTR,A

    MOV     A,#00H
    MOV     DPTR,#I2CSLST       ;
    MOVX    @DPTR,A

    MOV     A,#078H
    MOV     DPTR,#I2CSLCR       ;ʹ�ܴӻ�ģʽ�ж�
    MOVX    @DPTR,A

    ANL     P_SW2, #NOT 080H    ;��ֹ����XFR
    RET

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

;**************** �жϺ��� ***************************************************

F_Timer0_Interrupt: ;Timer0 1ms�жϺ���
    PUSH    PSW     ;PSW��ջ
    PUSH    ACC     ;ACC��ջ

    LCALL   F_DisplayScan   ; 1msɨ����ʾһλ
    SETB    B_1ms           ; 1ms��־
    
    POP     ACC     ;ACC��ջ
    POP     PSW     ;PSW��ջ
    RETI

;*******************************************************************
F_I2C_Interrupt:
    PUSH    ACC
    PUSH    PSW
    PUSH    DPL
    PUSH    DPH
    PUSH    P_SW2
    ORL     P_SW2, #080H  ;ʹ�ܷ���XFR

    MOV     DPTR,#I2CSLST ;���ӻ�״̬
    MOVX    A,@DPTR
    JB      ACC.6,STARTIF
    JB      ACC.5,RXIF
    JB      ACC.4,TXIF
    JB      ACC.3,STOPIF
F_I2C_EXIT:
    POP     P_SW2
    POP     DPH
    POP     DPL
    POP     PSW
    POP     ACC
    RETI

STARTIF:
    ANL     A,#NOT 40H  ;���� START �¼�
    MOVX    @DPTR,A
    JMP     F_I2C_EXIT
RXIF:
    ANL     A,#NOT 20H  ;���� RECV �¼�
    MOVX    @DPTR,A
    MOV     DPTR,#I2CRXD
    MOVX    A,@DPTR
    JBC     ISDA,RXDA
    JBC     ISMA,RXMA
    MOV     R0,ADDR     ;���� RECV �¼���RECV DATA��
    MOVX    @R0,A
    INC     ADDR
    JMP     F_I2C_EXIT
RXDA:
    JMP     F_I2C_EXIT  ;���� RECV �¼���RECV DEVICE ADDR��
RXMA:
    MOV     ADDR,A      ;���� RECV �¼���RECV MEMORY ADDR��
    MOV     R0,A
    MOVX    A,@R0
    MOV     DPTR,#I2CTXD
    MOVX    @DPTR,A
    JMP     F_I2C_EXIT
TXIF:
    ANL     A,#NOT 10H  ;���� SEND �¼�
    MOVX    @DPTR,A
    JB      ACC.1,RXNAK
    INC     ADDR
    MOV     R0,ADDR
    MOVX    A,@R0
    MOV     DPTR,#I2CTXD
    MOVX    @DPTR,A
    JMP     F_I2C_EXIT
RXNAK:
    MOV     A,#0FFH
    MOV     DPTR,#I2CTXD
    MOVX    @DPTR,A
    JMP     F_I2C_EXIT
STOPIF:
    ANL     A,#NOT 08H  ;���� STOP �¼�
    MOVX    @DPTR,A
    SETB    ISDA
    SETB    ISMA
    SETB    DisplayFlag
    JMP     F_I2C_EXIT

;========================================================================
; ���ģ��I2C����
;========================================================================
I2C_Delay:
    PUSH    0
    MOV     R0,#0CH
    DJNZ    R0,$
    POP     0
    RET

I2C_Start:
    SETB    SDA
    LCALL   I2C_Delay
    SETB    SCL
    LCALL   I2C_Delay
    CLR     SDA
    LCALL   I2C_Delay
    CLR     SCL
    LCALL   I2C_Delay
    RET

I2C_Stop:
    CLR     SDA
    LCALL   I2C_Delay
    SETB    SCL
    LCALL   I2C_Delay
    SETB    SDA
    LCALL   I2C_Delay
    RET

S_ACK:
    CLR     SDA
    LCALL   I2C_Delay
    SETB    SCL
    LCALL   I2C_Delay
    CLR     SCL
    LCALL   I2C_Delay
    RET

S_NoACK:
    SETB    SDA
    LCALL   I2C_Delay
    SETB    SCL
    LCALL   I2C_Delay
    CLR     SCL
    LCALL   I2C_Delay
    RET

I2C_Check_ACK:
    SETB    SDA
    LCALL   I2C_Delay
    SETB    SCL
    LCALL   I2C_Delay
	MOV     C, SDA
    CLR     SCL
    LCALL   I2C_Delay
    RET

I2C_WriteAbyte:
    MOV     R7, #08H
TXNEXT:
    RLC     A          ;�Ƴ�����λ
    MOV     SDA, C     ;���������ݿ�
    SETB    SCL        ;ʱ��->��
    LCALL   I2C_Delay  ;��ʱ
    CLR     SCL        ;ʱ��->��
    LCALL   I2C_Delay  ;��ʱ
    DJNZ    R7, TXNEXT ;����һλ
    RET

I2C_ReadAbyte:
    MOV     R7, #08H
RXNEXT:
    SETB    SCL        ;ʱ��->��
    LCALL   I2C_Delay  ;��ʱ
    MOV     C, SDA
    RLC     A
    CLR     SCL        ;ʱ��->��
    LCALL   I2C_Delay  ;��ʱ
    DJNZ    R7, RXNEXT ;����һλ
    RET

WriteNbyte:
    LCALL   I2C_Start
    MOV     A, #SLAW
    LCALL   I2C_WriteAbyte
    LCALL   I2C_Check_ACK
    JC      Write_Exit
    CLR     A
    LCALL   I2C_WriteAbyte ;addr
    LCALL   I2C_Check_ACK
    JC      Write_Exit

    MOV     R6, #04H
    MOV     A, #LED8+4  ;���� LED8[4]~LED8[7] ����
    MOV     R0,A
Write_Loop:
    MOV     A,@R0
    LCALL   I2C_WriteAbyte
    LCALL   I2C_Check_ACK
    JC      Write_Exit
    INC     R0
    DJNZ    R6, Write_Loop

Write_Exit:
    LCALL   I2C_Stop
    RET


    END

