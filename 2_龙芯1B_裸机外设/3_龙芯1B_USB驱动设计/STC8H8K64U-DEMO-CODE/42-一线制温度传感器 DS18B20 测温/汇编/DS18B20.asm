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

;ͨ��һ��IO�ڻ�ȡһ�����¶ȴ����� DS18B20 �¶�ֵ.

;ʹ��Timer0��16λ�Զ���װ������1ms����,�������������������, �û��޸�MCU��ʱ��Ƶ��ʱ,�Զ���ʱ��1ms.

;��STC��MCU��IO��ʽ����8λ����ܣ�ͨ���������ʾ�������¶�ֵ.

;����ʱ, ѡ��ʱ�� 24MHZ (�û��������޸�Ƶ��).

;******************************************/

;/****************************** �û������ ***********************************/

Fosc_KHZ    EQU 24000   ;24000KHZ

STACK_POIRTER   EQU     0D0H    ;��ջ��ʼ��ַ

Timer0_Reload   EQU     (65536 - Fosc_KHZ)  ; Timer 0 �ж�Ƶ��, 1000��/��

DIS_DOT         EQU     020H
DIS_BLACK       EQU     010H
DIS_            EQU     011H

;*******************************************************************
;*******************************************************************
AUXR    DATA    08EH
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

;*************  IO�ڶ���    **************/
DQ  BIT P3.3                ;DS18B20�����ݿ�λP3.3

;*************  ���ر�������    **************/
Flag0           DATA    20H
B_1ms           BIT     Flag0.0 ;   1ms��־

LED8            DATA    30H     ;   ��ʾ���� 30H ~ 37H
display_index   DATA    38H     ;   ��ʾλ����

msecond_H       DATA    39H     ;
msecond_L       DATA    3AH     ;

;*******************************************************************
;*******************************************************************

        ORG     0000H               ;reset
        LJMP    F_Main

        ORG     000BH               ;1  Timer0 interrupt
        LJMP    F_Timer0_Interrupt

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
    
    CLR     TR0
    ORL     AUXR, #(1 SHL 7)    ; Timer0_1T();
    ANL     TMOD, #NOT 04H      ; Timer0_AsTimer();
    ANL     TMOD, #NOT 03H      ; Timer0_16bitAutoReload();
    MOV     TH0, #Timer0_Reload / 256   ;Timer0_Load(Timer0_Reload);
    MOV     TL0, #Timer0_Reload MOD 256
    SETB    ET0         ; Timer0_InterruptEnable();
    SETB    TR0         ; Timer0_Run();
    SETB    EA          ; �����ж�

;=====================================================

L_Main_Loop:
    JNB     B_1ms,  L_Main_Loop     ;1msδ��
    CLR     B_1ms
    
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
    JC      L_Main_Loop     ;if(msecond < 300), jmp
    
;================= 300ms�� ====================================
    MOV     msecond_L, #0   ;if(msecond >= 1000)
    MOV     msecond_H, #0

    CALL    DS18B20_Reset       ;�豸��λ
    MOV     A,#0CCH             ;����ROM����
    CALL    DS18B20_WriteByte   ;�ͳ�����
    MOV     A,#044H             ;��ʼת��
    CALL    DS18B20_WriteByte   ;�ͳ�����
    JNB     DQ,$                ;�ȴ�ת�����

    CALL    DS18B20_Reset       ;�豸��λ
    MOV     A,#0CCH             ;����ROM����
    CALL    DS18B20_WriteByte   ;�ͳ�����
    MOV     A,#0BEH             ;���ݴ�洢��
    CALL    DS18B20_WriteByte   ;�ͳ�����
    CALL    DS18B20_ReadByte    ;���¶ȵ��ֽ�
    MOV     R5,A                ;�洢����
    CALL    DS18B20_ReadByte    ;���¶ȸ��ֽ�
    MOV     R4,A                ;�洢����

    MOV     R7, #5          ; 0.0625 * 10������1λС����
    LCALL   F_MUL16x8       ;(R4,R5)* R7 -->(R5,R6,R7)
    MOV     R4, #0
    MOV     R2, #0
    MOV     R3, #8          ; �¶� * 0.625 = �¶� * 5/8
    LCALL   F_ULDIV         ;(R4,R5,R6,R7)/(R2,R3)=(R4,R5,R6,R7),������(R2,R3),use R0~R7,B,DPL

    LCALL   F_HEX2_DEC      ;(R6 R7) HEX Change to DEC ---> (R3 R4 R5), use (R2~R7)
    MOV     A, R4
    SWAP    A
    ANL     A, #0x0F
    MOV     LED8+3, A       ;��ʾ�¶�ֵ
    MOV     A, R4
    ANL     A, #0x0F
    MOV     LED8+2, A
    MOV     A, R5
    SWAP    A
    ANL     A, #0x0F
    ADD     A, #DIS_DOT
    MOV     LED8+1, A
    MOV     A, R5
    ANL     A, #0x0F
    MOV     LED8, A
    MOV     A, LED8+3
    JNZ     L_LED8_3_Not_0
    MOV     LED8+3, #DIS_BLACK      ;ǧλΪ0������
L_LED8_3_Not_0:
    JNB     F0, L_QuitRead_Temp
    MOV     LED8+3, #DIS_   ;���¶�, ��ʾ-
L_QuitRead_Temp:

    LJMP    L_Main_Loop

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

;***************************************************************************
F_ULDIV:
F_DIV32:                            ; (R4,R5,R6,R7)/(R2,R3)=(R4,R5,R6,R7),������(R2,R3),use R0~R7,B,DPL
    CJNE    R2,#0,F_DIV32_16    ; L_0075
        
F_DIV32_8:                          ;R3��0��(R4,R5,R6,R7)/R3=(R4,R5,R6,R7),������ R3,use R0~R7,B
    MOV     A,R4
    MOV     B,R3
    DIV     AB
    XCH     A,R7
    XCH     A,R6
    XCH     A,R5
    MOV     R4,A
    MOV     A,B
    XCH     A,R3
    MOV     R1,A
    MOV     R0,#24
L_0056:
    MOV     A,R7
    ADD     A,R7
    MOV     R7,A
    MOV     A,R6
    RLC     A
    MOV     R6,A
    MOV     A,R5
    RLC     A
    MOV     R5,A
    MOV     A,R4
    RLC     A
    MOV     R4,A
    MOV     A,R3
    RLC     A
    MOV     R3,A
    JBC     CY,L_006B
    SUBB    A,R1
    JC      L_006F
L_006B:
    MOV     A,R3
    SUBB    A,R1
    MOV     R3,A
    INC     R7
L_006F:
    DJNZ    R0,L_0056
    CLR     A
    MOV     R1,A
    MOV     R2,A
    RET

;***************************************************************************
F_DIV32_16:                     ;R2��0��(R4,R5,R6,R7)/(R2,R3)=(R5,R6,R7),������ (R2,R3),use R0~R7
L_0075:
    MOV     R0,#24          ;��ʼR1=0
L_0077:
    MOV     A,R7            ;����һλ
    ADD     A,R7
    MOV     R7,A
    MOV     A,R6
    RLC     A
    MOV     R6,A
    MOV     A,R5
    RLC     A
    MOV     R5,A
    MOV     A,R4
    RLC     A
    MOV     R4,A
    XCH     A,R1
    RLC     A
    XCH     A,R1
    JBC     CY,L_008E   ;���C=1���϶�����
    SUBB    A,R3
    MOV     A,R1        ;�����Ƿ񹻼�
    SUBB    A,R2
    JC      L_0095
L_008E:
    MOV     A,R4
    SUBB    A,R3
    MOV     R4,A
    MOV     A,R1
    SUBB    A,R2
    MOV     R1,A
    INC     R7
L_0095:
    DJNZ    R0,L_0077
    CLR     A
    XCH     A,R1
    MOV     R2,A
    CLR     A
    XCH     A,R4
    MOV     R3,A
    RET

;**************************************
;��ʱX΢��(12M)
;��ͬ�Ĺ�������,��Ҫ�����˺���
;��ڲ���:R7
;���ڲ���:��
;**************************************
DelayXus:                   ;6 ����ʱ������ʹ��1T��ָ�����ڽ��м���,�봫ͳ��12T��MCU��ͬ
    NOP                     ;1
    NOP                     ;1
    NOP                     ;1
    NOP                     ;1
    NOP                     ;1
    NOP                     ;1
    NOP                     ;1
    NOP                     ;1
    NOP                     ;1
    NOP                     ;1
    NOP                     ;1
    NOP                     ;1
    NOP                     ;1
    NOP                     ;1
    NOP                     ;1
    NOP                     ;1
    DJNZ R7,DelayXus        ;4
    RET                     ;4

;**************************************
;��λDS18B20,������豸�Ƿ����
;��ڲ���:��
;���ڲ���:��
;**************************************
DS18B20_Reset:
    CLR     DQ                 ;�ͳ��͵�ƽ��λ�ź�
    MOV     R7,#240            ;��ʱ����480us
    CALL    DelayXus
    MOV     R7,#240
    CALL    DelayXus
    SETB    DQ                 ;�ͷ�������
    MOV     R7,#60             ;�ȴ�60us
    CALL    DelayXus
    MOV     C,DQ               ;����������
    MOV     R7,#240            ;�ȴ��豸�ͷ�������
    CALL    DelayXus
    MOV     R7,#180
    CALL    DelayXus
    JC      DS18B20_Reset      ;����豸������,������ȴ�
    RET

;**************************************
;��DS18B20��1�ֽ�����
;��ڲ���:��
;���ڲ���:ACC
;**************************************
DS18B20_ReadByte:
    CLR     A
    PUSH    0
    MOV     0,#8               ;8λ������
ReadNext:
    CLR     DQ                 ;��ʼʱ��Ƭ
    MOV     R7,#1              ;��ʱ�ȴ�
    CALL    DelayXus
    SETB    DQ                 ;׼������
    MOV     R7,#1
    CALL    DelayXus
    MOV     C,DQ               ;��ȡ����
    RRC     A
    MOV     R7,#60             ;�ȴ�ʱ��Ƭ����
    CALL    DelayXus
    DJNZ    0,ReadNext
    POP     0
    RET

;**************************************
;��DS18B20д1�ֽ�����
;��ڲ���:ACC
;���ڲ���:��
;**************************************
DS18B20_WriteByte:
    PUSH    0
    MOV     0,#8               ;8λ������
WriteNext:
    CLR     DQ                 ;��ʼʱ��Ƭ
    MOV     R7,#1              ;��ʱ�ȴ�
    CALL    DelayXus
    RRC     A                  ;�������
    MOV     DQ,C
    MOV     R7,#60             ;�ȴ�ʱ��Ƭ����
    CALL    DelayXus
    SETB    DQ                 ;׼���ͳ���һλ����
    MOV     R7,#1
    CALL    DelayXus
    DJNZ    0,WriteNext
    POP     0
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

    END

