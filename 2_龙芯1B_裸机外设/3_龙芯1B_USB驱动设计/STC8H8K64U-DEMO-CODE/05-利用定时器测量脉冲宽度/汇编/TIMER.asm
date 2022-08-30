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

; �����̻���STC8H8K64UΪ����оƬ��ʵ����8���б�д���ԣ�STC8G��STC8Hϵ��оƬ��ͨ�òο�.

; ��ʱ��0��16λ�Զ���װ, �ж�Ƶ��Ϊ1000HZ����Ϊ�����ɨ����ʾ.

; ��ʱ��1��16λ�Զ���װ, �ж�Ƶ��Ϊ10000HZ����Ϊ�����źſ�ȼ��.

; ��STC��MCU��IO��ʽ����8λ����ܡ�

; P33�ڲ����͵�ƽ(�û����Զ����޸Ķ˿ڸ�����ƽ)ʱ���м�ʱ����ƽ�仯��ֹͣ��ʱ��

; �������ʾ�����źſ�ȣ���λ0.1ms��������Χ0.1ms~6553.5ms.

; ����ʱ, ѡ��ʱ�� 24MHZ (�û��������޸�Ƶ��).

;******************************************/

;/****************************** �û������ ***********************************/


Fosc_KHZ    EQU 24000   ;24000KHZ

STACK_POIRTER   EQU     0D0H    ;��ջ��ʼ��ַ

DIS_DOT         EQU     020H
DIS_BLACK       EQU     010H
DIS_            EQU     011H

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

AUXR    DATA    08EH
INT_CLKO DATA   0x8F

P4      DATA    0xC0
P5      DATA    0C8H
P6      DATA    0E8H
P7      DATA    0F8H

;*************  ���ر�������    **************/
Flag0           DATA    20H
B_1ms           BIT     Flag0.0 ;   1ms��־

LED8            DATA    30H     ; ��ʾ���� 30H ~ 37H
display_index   DATA    38H     ; ��ʾλ����

Test_cntH       DATA    39H     ; ��ʾ�õļ�������
Test_cntL       DATA    3AH     ; ��ʾ�õļ�������
Temp_cntH       DATA    3BH     ; �����õļ�������
Temp_cntL       DATA    3CH     ; �����õļ�������
msecond_H       DATA    3DH     ;
msecond_L       DATA    3EH     ;

;*******************************************************************
;*******************************************************************
        ORG     0000H               ;reset
        LJMP    F_Main

        ORG     000BH               ;1  Timer0 interrupt
        LJMP    F_Timer0_Interrupt

        ORG     001BH               ;3  Timer1 interrupt
        LJMP    F_Timer1_Interrupt

;*******************************************************************
;*******************************************************************


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

    MOV     Test_cntH, #0
    MOV     Test_cntL, #0
    MOV     Temp_cntH, #0
    MOV     Temp_cntL, #0
    MOV     display_index, #0
    MOV     R0, #LED8
    MOV     R2, #8
L_ClearLoop:
    MOV     @R0, #DIS_BLACK     ;�ϵ�����
    INC     R0
    DJNZ    R2, L_ClearLoop
    
    LCALL   F_Timer0_init
    LCALL   F_Timer1_init
    SETB    EA
    
;=================== ��ѭ�� ==================================
L_Main_Loop:
    JNB     B_1ms,  L_Main_Loop     ;1msδ��
    CLR     B_1ms

;=================== ���1000ms�Ƿ� ==================================
    INC     msecond_L       ;msecond + 1
    MOV     A, msecond_L
    JNZ     $+4
    INC     msecond_H
    
    CLR     C
    MOV     A, msecond_L    ;msecond - 1000
    SUBB    A, #LOW 1000
    MOV     A, msecond_H
    SUBB    A, #HIGH 1000
    JNC     L_1sIsGood      ;if(msecond >= 1000), jmp
    LJMP    L_Main_Loop     ;if(msecond < 1000), jmp
L_1sIsGood:
    MOV     msecond_L, #0   ;if(msecond >= 1000)
    MOV     msecond_H, #0

    LCALL   F_Display
    LJMP    L_Main_Loop

;**********************************************/

;========================================================================
; ����: F_Timer0_init
; ����: timer0��ʼ������.
; ����: none.
; ����: none.
; �汾: V1.0, 2015-1-12
;========================================================================
F_Timer0_init:
    CLR     TR0 ; ֹͣ����
    SETB    ET0 ; �����ж�
;   SETB    PT0 ; �����ȼ��ж�
    ANL     TMOD, #NOT 0x03     ;
    ORL     TMOD, #0            ; ����ģʽ, 0: 16λ�Զ���װ, 1: 16λ��ʱ/����, 2: 8λ�Զ���װ, 3: 16λ�Զ���װ, ���������ж�
;   ORL     TMOD, #0x04         ; ����������Ƶ
    ANL     TMOD, #NOT 0x04     ; ��ʱ
;   ORL     INT_CLKO, #0x01     ; ���ʱ��
    ANL     INT_CLKO, #NOT 0x01 ; �����ʱ��

;   ANL     AUXR, #NOT 0x80     ; 12T mode
    ORL     AUXR, #0x80         ; 1T mode
    MOV     TH0, #HIGH (-Fosc_KHZ) ; - 24000000 / 1000
    MOV     TL0, #LOW  (-Fosc_KHZ) ;
    SETB    TR0 ; ��ʼ����
    RET


;========================================================================
; ����: F_Timer1_init
; ����: timer1��ʼ������.
; ����: none.
; ����: none.
; �汾: V1.0, 2015-1-12
;========================================================================
F_Timer1_init:
    CLR     TR1 ; ֹͣ����
    SETB    ET1 ; �����ж�
;   SETB    PT1 ; �����ȼ��ж�
    ANL     TMOD, #NOT 0x30     ;
    ORL     TMOD, #(0 SHL 4)    ; ����ģʽ, 0: 16λ�Զ���װ, 1: 16λ��ʱ/����, 2: 8λ�Զ���װ, 3: 16λ�Զ���װ, ���������ж�
;   ORL     TMOD, #0x40         ; ����������Ƶ
    ANL     TMOD, #NOT 0x40     ; ��ʱ
;   ORL     INT_CLKO, #0x02     ; ���ʱ��
    ANL     INT_CLKO, #NOT 0x02 ; �����ʱ��

;   ANL     AUXR, #NOT 0x40     ; 12T mode
    ORL     AUXR, #0x40         ; 1T mode
    MOV     TH1, #HIGH (-(Fosc_KHZ / 10)) ; - 24000000 / 10000
    MOV     TL1, #LOW  (-(Fosc_KHZ / 10)) ;
    SETB    TR1 ; ��ʼ����
    RET


;**************** �жϺ��� ***************************************************
F_Timer0_Interrupt: ;Timer0 �жϺ���
    PUSH    PSW     ;PSW��ջ
    PUSH    ACC     ;ACC��ջ

    LCALL   F_DisplayScan   ; 1msɨ����ʾһλ
    SETB    B_1ms           ; 1ms��־

    POP     ACC     ;ACC��ջ
    POP     PSW     ;PSW��ջ
    RETI
    
F_Timer1_Interrupt: ;Timer1 �жϺ���
    PUSH    PSW     ;PSW��ջ
    PUSH    ACC     ;ACC��ջ

    JB     P3.3, F_Timer1_Next
	
    INC     Temp_cntL       ;Temp_cnt + 1
    MOV     A, Temp_cntL
    JNZ     F_Timer1_Exit
    INC     Temp_cntH
    LJMP    F_Timer1_Exit
	
F_Timer1_Next:
    MOV     A, Temp_cntH
    JNZ     F_Timer1_Save
    MOV     A, Temp_cntL
    SUBB    A, #10
    JC      F_Timer1_Clear	;if(Temp_cnt<10)
F_Timer1_Save:
    MOV     A, Temp_cntL
    MOV     Test_cntL, A
    MOV     A, Temp_cntH
    MOV     Test_cntH, A
F_Timer1_Clear:
    MOV     Temp_cntL, #0
    MOV     Temp_cntH, #0

F_Timer1_Exit:
    POP     ACC     ;ACC��ջ
    POP     PSW     ;PSW��ջ
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


;========================================================================
; ����: F_DisplayScan
; ����: ��ʾɨ���ӳ���
; ����: none.
; ����: none.
; �汾: VER1.0
; ����: 2013-4-1
; ��ע: ����ACCC��PSW��, ���õ���ͨ�üĴ�������ջ
;========================================================================
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

;========================================================================
; ����: F_HEX2_DEC
; ����: ��˫�ֽ�ʮ��������ת��ʮ����BCD��.
; ����: (R6 R7): Ҫת����˫�ֽ�ʮ��������.
; ����: (R3 R4 R5) = BCD��.
; �汾: V1.0, 2013-10-22
;========================================================================
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

;========================================================================
; ����: F_Display
; ����: ��ʾ��������.
; ����: none.
; ����: none.
; �汾: V1.0, 2021-3-1
;========================================================================
F_Display:
    MOV     R6, Test_cntH
    MOV     R7, Test_cntL
    LCALL   F_HEX2_DEC      ;(R6 R7) HEX Change to DEC ---> (R3 R4 R5), use (R2~R7)
    
    MOV     A, R3           ;��ʾ����ֵ
    ANL     A, #0x0F
    MOV     LED8+4, A
    MOV     A, R4
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
    MOV     LED8, A

    MOV     A, LED8+4           ;��ʾ����Ч0
    JNZ     F_QuitDisplay
    MOV     LED8+4, #DIS_BLACK
    MOV     A, LED8+3
    JNZ     F_QuitDisplay
    MOV     LED8+3, #DIS_BLACK
    MOV     A, LED8+2
    JNZ     F_QuitDisplay
    MOV     LED8+2, #DIS_BLACK
    MOV     A, LED8+1
    JNZ     F_QuitDisplay
    MOV     LED8+1, #DIS_BLACK
F_QuitDisplay:
    RET




    END

