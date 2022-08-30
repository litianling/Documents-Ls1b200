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

;�����շ������������г�����������NEC���롣

;Ӧ�ò��ѯ B_IR_Press��־Ϊ,���ѽ��յ�һ���������IR_code��, ���������� �û��������B_IR_Press��־.

;���������4λ��ʾ�û���, ���ұ���λ��ʾ����, ��Ϊʮ������.

;�û������ں궨����ָ���û���.

;�û��ײ���򰴹̶���ʱ����(60~125us)���� "IR_RX_NEC()"����.

;����IO���м�����֧��ADC���̣�����ʾ���͡����յ��ļ�ֵ��

;����ʱ, ѡ��ʱ�� 24MHz (�û��������޸�Ƶ��).

;******************************************/

;/****************************** �û������ ***********************************/

Fosc_KHZ    EQU 24000   ;24000KHZ, �û�ֻ��Ҫ�Ķ����ֵ����Ӧ�Լ�ʵ�ʵ�Ƶ��

STACK_POIRTER   EQU     0D0H    ;��ջ��ʼ��ַ

Timer0_Reload   EQU     (65536 - (Fosc_KHZ/10))   ; Timer 0 �ж�Ƶ��, 10000��/��

DIS_DOT         EQU     020H
DIS_BLACK       EQU     010H
DIS_            EQU     011H

;*******************************************************************
;*******************************************************************

P_SW2     DATA 0BAH
AUXR      DATA 08EH
P4        DATA 0C0H
P5        DATA 0C8H
P6        DATA 0E8H
P7        DATA 0F8H

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


;*************  ���ر�������    **************/
Flag0           DATA    20H
B_1ms           BIT     Flag0.0     ;1ms��־
P_IR_RX_temp    BIT     Flag0.1     ;�û����ɲ���, Last sample
B_IR_Sync       BIT     Flag0.2     ;�û����ɲ���, ���յ�ͬ����־
B_Space         BIT     Flag0.3     ;���Ϳ���(��ʱ)��־
B_IR_Press      BIT     Flag0.4     ;�û�ʹ��, ������������

LED8            DATA    30H     ;   ��ʾ���� 30H ~ 37H
display_index   DATA    38H     ;   ��ʾλ����

cnt_1ms         DATA    39H     ;

;*************  ������ճ����������    **************
#define User_code   0xFF00      //��������û���

P_IR_TX         BIT P1.6    ;������ⷢ�Ͷ˿�
P_IR_RX         BIT P3.5    ;��������������IO��

IR_SampleCnt    DATA    3AH ;�û����ɲ���, ��������
IR_BitCnt       DATA    3BH ;�û����ɲ���, ����λ��
IR_UserH        DATA    3CH ;�û����ɲ���, �û���(��ַ)���ֽ�
IR_UserL        DATA    3DH ;�û����ɲ���, �û���(��ַ)���ֽ�
IR_data         DATA    3EH ;�û����ɲ���, ����ԭ��
IR_DataShit     DATA    3FH ;�û����ɲ���, ������λ

IR_code         DATA    40H ;�û�ʹ��, �������
UserCodeH       DATA    41H ;�û�ʹ��, �û�����ֽ�
UserCodeL       DATA    42H ;�û�ʹ��, �û�����ֽ�
msecond         DATA    43H

IO_KeyState     DATA    44H ; IO���м�״̬����
IO_KeyState1    DATA    45H
IO_KeyHoldCnt   DATA    46H ; IO�����¼�ʱ
KeyCode         DATA    47H ; ���û�ʹ�õļ���, 1~16ΪADC���� 17~32ΪIO��

/*************  ���ⷢ����ر���    **************/
tx_cntH         DATA    48H ;���ͻ���е��������(����38KHZ������������Ӧʱ��), ����Ƶ��Ϊ38KHZ, ����26.3us
tx_cntL         DATA    49H    
TxTime          DATA    4AH ;����ʱ��


;*******************************************************************
;*******************************************************************
        ORG     0000H               ;reset
        LJMP    F_Main

        ORG     000BH               ;1  Timer0 interrupt
        LJMP    F_Timer0_Interrupt

        ORG     00D3H               ;26  PWMA interrupt
        LJMP    F_PWMA_Interrupt

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
    MOV     PSW, #0     ;ѡ���0��R0~R7
    USING   0       ;ѡ���0��R0~R7

;================= �û���ʼ������ ====================================

    MOV     display_index, #0
    MOV     R0, #LED8
    MOV     R2, #8
L_ClearLoop:
    MOV     @R0, #DIS_
    INC     R0
    DJNZ    R2, L_ClearLoop

    MOV     R0, #LED8+2
    MOV     @R0, #DIS_BLACK     ;�ϵ�����
    INC     R0
    MOV     @R0, #DIS_BLACK     ;�ϵ�����
    
    CLR     TR0
    ORL     AUXR, #(1 SHL 7)    ; Timer0_1T();
    ANL     TMOD, #NOT 04H      ; Timer0_AsTimer();
    ANL     TMOD, #NOT 03H      ; Timer0_16bitAutoReload();
    MOV     TH0, #Timer0_Reload / 256   ;Timer0_Load(Timer0_Reload);
    MOV     TL0, #Timer0_Reload MOD 256
    SETB    ET0         ; Timer0_InterruptEnable();
    SETB    TR0         ; Timer0_Run();
    
    LCALL   F_PWM_Init      ;��ʼ��PWM
    SETB    P_IR_TX
    SETB    EA              ;�����ж�
    
    MOV     KeyCode, #0
    MOV     cnt_1ms, #10

;=====================================================

;=====================================================
L_Main_Loop:

    JNB     B_1ms,  L_Main_Loop     ;1msδ��
    CLR     B_1ms
    
    JNB     B_IR_Press, L_Main_KeyScan ;δ��⵽�յ��������

    CLR     B_IR_Press      ;��⵽�յ��������
    MOV     A, UserCodeH
    SWAP    A
    ANL     A, #0FH
    MOV     LED8+7, A       ;�û�����ֽڵĸ߰��ֽ�
    MOV     A, UserCodeH
    ANL     A, #0FH
    MOV     LED8+6, A       ;�û�����ֽڵĵͰ��ֽ�

    MOV     A, UserCodeL
    SWAP    A
    ANL     A, #0FH
    MOV     LED8+5, A       ;�û�����ֽڵĸ߰��ֽ�
    MOV     A, UserCodeL
    ANL     A, #0FH
    MOV     LED8+4, A       ;�û�����ֽڵĵͰ��ֽ�
    
    MOV     A, IR_code
    SWAP    A
    ANL     A, #0FH
    MOV     LED8+1, A       ;ң�����ݵĸ߰��ֽ�
    MOV     A, IR_code
    ANL     A, #0FH
    MOV     LED8+0, A       ;ң�����ݵĵͰ��ֽ�

L_Main_KeyScan:
;=================== ���30ms�Ƿ� ==================================
    INC     msecond       ;msecond + 1
    CLR     C
    MOV     A, msecond    ;msecond - 30
    SUBB    A, #30
    JC      L_Main_Loop     ;if(msecond < 30), jmp
    
;================= 30ms�� ====================================
    MOV     msecond, #0

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

L_ClearKeyCode:
    MOV KeyCode, #0

    LJMP    L_Main_Loop

;**********************************************/


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
    PUSH    AR0     ;R0 ��ջ
    
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
    POP     AR0     ;R0 ��ջ
    POP     DPL     ;DPL��ջ
    POP     DPH     ;DPH��ջ
    RET


;*******************************************************************
;**************** �жϺ��� ***************************************************

F_Timer0_Interrupt: ;Timer0 1ms�жϺ���
    PUSH    PSW     ;PSW��ջ
    PUSH    ACC     ;ACC��ջ
    PUSH    AR7     ;SampleTime

    LCALL   F_IR_RX_NEC

    DJNZ    cnt_1ms, L_Quit_1ms
    MOV     cnt_1ms, #10
    LCALL   F_DisplayScan   ; 1msɨ����ʾһλ
    SETB    B_1ms           ; 1ms��־
L_Quit_1ms:

    POP     AR7
    POP     ACC     ;ACC��ջ
    POP     PSW     ;PSW��ջ
    RETI
    

;*******************************************************************
;*********************** IR Remote Module **************************
;*********************** By  (Coody) 2002-8-24 *********************
;*********************** IR Remote Module **************************
;this programme is used for Receive IR Remote (NEC Code).

;data format: Synchro, AddressH, AddressL, data, /data, (total 32 bit).

;send a frame(85ms), pause 23ms, send synchro of continue frame, pause 94ms

;data rate: 108ms/Frame


;Synchro: low=9ms, high=4.5 / 2.25ms, low=0.5626ms
;Bit0: high=0.5626ms, low=0.5626ms
;Bit1: high=1.6879ms, low=0.5626ms
;frame rate = 108ms ( pause 23 ms or 96 ms)

;******************** �������ʱ��궨��, �û���Ҫ�����޸�  *******************

D_IR_sample         EQU 100                 ;��ѯʱ����, 100us, �������Ҫ����60us~250us֮��
D_IR_SYNC_MAX       EQU (15000/D_IR_sample) ;SYNC max time
D_IR_SYNC_MIN       EQU (9700 /D_IR_sample) ;SYNC min time
D_IR_SYNC_DIVIDE    EQU (12375/D_IR_sample) ;decide data 0 or 1
D_IR_DATA_MAX       EQU (3000 /D_IR_sample) ;data max time
D_IR_DATA_MIN       EQU (600  /D_IR_sample) ;data min time
D_IR_DATA_DIVIDE    EQU (1687 /D_IR_sample) ;decide data 0 or 1
D_IR_BIT_NUMBER     EQU 32                  ;bit number

;*******************************************************************************************
;**************************** IR RECEIVE MODULE ********************************************

F_IR_RX_NEC:
    INC     IR_SampleCnt        ;Sample + 1

    MOV     C, P_IR_RX_temp     ;Save Last sample status
    MOV     F0, C
    MOV     C, P_IR_RX          ;Read current status
    MOV     P_IR_RX_temp, C

    JNB     F0, L_QuitIrRx              ;Pre-sample is high
    JB      P_IR_RX_temp, L_QuitIrRx    ;and current sample is low, so is fall edge

    MOV     R7, IR_SampleCnt            ;get the sample time
    MOV     IR_SampleCnt, #0            ;Clear the sample counter

    MOV     A, R7       ; if(SampleTime > D_IR_SYNC_MAX)    B_IR_Sync = 0
    SETB    C
    SUBB    A, #D_IR_SYNC_MAX
    JC      L_IR_RX_1
    CLR     B_IR_Sync       ;large than the Maxim SYNC time, then error
    RET
    
L_IR_RX_1:
    MOV     A, R7       ; else if(SampleTime >= D_IR_SYNC_MIN)
    CLR     C
    SUBB    A, #D_IR_SYNC_MIN
    JC      L_IR_RX_2

    MOV     A, R7       ; else if(SampleTime >= D_IR_SYNC_MIN)
    SUBB    A, #D_IR_SYNC_DIVIDE
    JC      L_QuitIrRx
    SETB    B_IR_Sync           ;has received SYNC
    MOV     IR_BitCnt, #D_IR_BIT_NUMBER     ;Load bit number
    RET

L_IR_RX_2:
    JNB     B_IR_Sync, L_QuitIrRx   ;else if(B_IR_Sync), has received SYNC
    MOV     A, R7       ; if(SampleTime > D_IR_DATA_MAX)    B_IR_Sync = 0, data samlpe time too large
    SETB    C
    SUBB    A, #D_IR_DATA_MAX
    JC      L_IR_RX_3
    CLR     B_IR_Sync       ;data samlpe time too large
    RET

L_IR_RX_3:
    MOV     A, IR_DataShit      ;data shift right 1 bit
    CLR     C
    RRC     A
    MOV     IR_DataShit, A
    
    MOV     A, R7
    CLR     C
    SUBB    A, #D_IR_DATA_DIVIDE
    JC      L_IR_RX_4
    ORL     IR_DataShit, #080H  ;if(SampleTime >= D_IR_DATA_DIVIDE) IR_DataShit |= 0x80;    //devide data 0 or 1
L_IR_RX_4:
    DEC     IR_BitCnt
    MOV     A, IR_BitCnt
    JNZ     L_IR_RX_5           ;bit number is over?
    CLR     B_IR_Sync           ;Clear SYNC
    MOV     A, IR_DataShit      ;if(~IR_DataShit == IR_data)        //�ж�����������
    CPL     A
    XRL     A, IR_data
    JNZ     L_QuitIrRx
    
    MOV     UserCodeH, IR_UserH
    MOV     UserCodeL, IR_UserL
    MOV     IR_code, IR_data
    SETB    B_IR_Press          ;������Ч
    RET

L_IR_RX_5:
    MOV     A, IR_BitCnt        ;else if((IR_BitCnt & 7)== 0)   one byte receive
    ANL     A, #07H
    JNZ     L_QuitIrRx
    MOV     IR_UserL, IR_UserH      ;Save the User code high byte
    MOV     IR_UserH, IR_data       ;Save the User code low byte
    MOV     IR_data,  IR_DataShit   ;Save the IR data byte
L_QuitIrRx:
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

