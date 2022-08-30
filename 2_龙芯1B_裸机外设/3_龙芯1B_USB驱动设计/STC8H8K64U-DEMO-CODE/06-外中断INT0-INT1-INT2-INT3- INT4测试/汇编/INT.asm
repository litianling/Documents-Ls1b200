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

;�����̻���STC8H8K64UΪ����оƬ��ʵ����8���б�д���ԣ�STC8G��STC8Hϵ��оƬ��ͨ�òο�.

;��STC��MCU��IO��ʽ����8λ����ܡ�

;��ʾЧ��Ϊ: ���ΪINT0(SW17)�жϼ���, �ұ�ΪINT1(SW18)�жϼ���, ������ΧΪ0~255.

;��ʾ5���, ˯��. �����ϵ�AW17 SW18����, ����������ʾ. 5�����˯��.

;���ڰ����ǻ�е����, �����ж���, ��������û��ȥ��������, ���԰�һ���ж������Ҳ��������.

;INT2, INT3, INT4 ʵ�����û���������԰���������Ҫʱ�ο�ʹ��.

;����ʱ, ѡ��ʱ�� 24MHZ (�û��������޸�Ƶ��).

;******************************************

;****************************** �û������ ***********************************/

Fosc_KHZ    EQU 24000   ;24000KHZ

STACK_POIRTER   EQU     0D0H    ; ��ջ��ʼ��ַ

DIS_DOT         EQU     020H
DIS_BLACK       EQU     010H
DIS_            EQU     011H

;*******************************************************************
;*******************************************************************

AUXR    DATA    08EH
INTCLKO DATA    08FH
EX2     EQU     010H
EX3     EQU     020H
EX4     EQU     040H

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
        
;*************  IO�ڶ���    **************/


;*************  ���ر�������    **************/
LED8            DATA    30H     ; ��ʾ���� 30H ~ 37H
display_index   DATA    38H     ; ��ʾλ����

INT0_cnt        DATA    39H     ; �����õļ�������
INT1_cnt        DATA    3AH     ;
INT2_cnt        DATA    3BH     ;
INT3_cnt        DATA    3CH     ;
INT4_cnt        DATA    3DH     ;


;*******************************************************************
;*******************************************************************
        ORG     0000H               ;reset
        LJMP    F_Main


        ORG     0003H               ;0  INT0 interrupt
        LJMP    F_INT0_Interrupt      

        ORG     0013H               ;2  INT1 interrupt
        LJMP    F_INT1_Interrupt      

        ORG     0053H               ;10 INT2 interrupt
        LJMP    F_INT2_Interrupt      

        ORG     005BH               ;11 INT3 interrupt
        LJMP    F_INT3_Interrupt      

        ORG     0083H               ;16 INT4 interrupt
        LJMP    F_INT4_Interrupt      


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

    MOV     display_index, #0
    MOV     R0, #LED8
    MOV     R2, #8
L_ClearLoop:
    MOV     @R0, #DIS_BLACK     ;�ϵ�����
    INC     R0
    DJNZ    R2, L_ClearLoop
    
    CLR     IE1     ;���ж�1��־λ
    CLR     IE0     ;���ж�0��־λ
    SETB    EX1     ;INT1 Enable
    SETB    EX0     ;INT0 Enable

    SETB    IT0     ;INT0 �½����ж�        
;   CLR     IT0     ;INT0 ����,�½����ж�   
    SETB    IT1     ;INT1 �½����ж�        
;   CLR     IT1     ;INT1 ����,�½����ж�

    ;INT2, INT3, INT4 ʵ�����û���������԰���������Ҫʱ�ο�ʹ��
    MOV     INTCLKO, #EX2  ;ʹ�� INT2 �½����ж�
    ORL     INTCLKO, #EX3  ;ʹ�� INT3 �½����ж�
    ORL     INTCLKO, #EX4  ;ʹ�� INT4 �½����ж�

    SETB    EA      ;�������ж�
    
    MOV     INT0_cnt, #0
    MOV     INT1_cnt, #0

;=================== ��ѭ�� ==================================
L_MainLoop:
    MOV     A, #1           ;��ʱ1ms
    LCALL   F_delay_ms  
    LCALL   F_DisplayScan
    LJMP    L_MainLoop

;**********************************************/


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
    
    MOV     A, INT0_cnt
    MOV     B, #100
    DIV     AB
    MOV     LED8+7, A
    MOV     A, #10
    XCH     A, B
    DIV     AB
    MOV     LED8+6, A
    MOV     LED8+5, B
    MOV     LED8+4, #DIS_BLACK;

    MOV     LED8+3, #DIS_BLACK;
    MOV     A, INT1_cnt
    MOV     B, #100
    DIV     AB
    MOV     LED8+2, A
    MOV     A, #10
    XCH     A, B
    DIV     AB
    MOV     LED8+1, A
    MOV     LED8+0, B
L_QuitDisplayScan:
    POP     00H     ;R0 ��ջ
    POP     DPL     ;DPL��ջ
    POP     DPH     ;DPH��ջ
    RET


;========================================================================
; ����: F_INT0_Interrupt
; ����: INT0�жϺ���.
; ����: none.
; ����: none.
; �汾: VER1.0
; ����: 2013-4-1
; ��ע: ���õ���ͨ�üĴ�������ջ����, �˳�ʱ�ָ�ԭ�����ݲ��ı�.
;========================================================================
F_INT0_Interrupt:
    INC     INT0_cnt    ; �ж�+1
    RETI
    
;========================================================================
; ����: F_INT1_Interrupt
; ����: INT1�жϺ���.
; ����: none.
; ����: none.
; �汾: VER1.0
; ����: 2013-4-1
; ��ע: ���õ���ͨ�üĴ�������ջ����, �˳�ʱ�ָ�ԭ�����ݲ��ı�.
;========================================================================
F_INT1_Interrupt:
    INC     INT1_cnt    ; �ж�+1
    RETI

;========================================================================
; ����: F_INT2_Interrupt
; ����: INT2�жϺ���.
; ����: none.
; ����: none.
; �汾: VER1.0
; ����: 2020-11-4
; ��ע: ���õ���ͨ�üĴ�������ջ����, �˳�ʱ�ָ�ԭ�����ݲ��ı�.
;========================================================================
F_INT2_Interrupt:
    INC     INT2_cnt    ; �ж�+1
    RETI

;========================================================================
; ����: F_INT3_Interrupt
; ����: INT3�жϺ���.
; ����: none.
; ����: none.
; �汾: VER1.0
; ����: 2020-11-4
; ��ע: ���õ���ͨ�üĴ�������ջ����, �˳�ʱ�ָ�ԭ�����ݲ��ı�.
;========================================================================
F_INT3_Interrupt:
    INC     INT3_cnt    ; �ж�+1
    RETI

;========================================================================
; ����: F_INT4_Interrupt
; ����: INT4�жϺ���.
; ����: none.
; ����: none.
; �汾: VER1.0
; ����: 2020-11-4
; ��ע: ���õ���ͨ�üĴ�������ջ����, �˳�ʱ�ָ�ԭ�����ݲ��ı�.
;========================================================================
F_INT4_Interrupt:
    INC     INT4_cnt    ; �ж�+1
    RETI


    END
