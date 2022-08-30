;/*---------------------------------------------------------------------*/
;/* --- STC MCU Limited ------------------------------------------------*/
;/* --- STC 1T Series MCU Demo Programme -------------------------------*/
;/* --- Mobile: (86)13922805190 ----------------------------------------*/
;/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
;/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
;/* --- Web: www.STCMCU.com --------------------------------------------*/
;/* --- Web: www.STCMCUDATA.com ----------------------------------------*/
;/* --- QQ:  800003751 -------------------------------------------------*/
;/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序        */
;/*---------------------------------------------------------------------*/

;/************* 功能说明    **************

;单纯的IO口初始化，端口模式配置参考程序.

;******************************************/

P0M0        DATA    094H
P0M1        DATA    093H
P1M0        DATA    092H
P1M1        DATA    091H
P2M0        DATA    096H
P2M1        DATA    095H
P3M0        DATA    0B2H
P3M1        DATA    0B1H
P4M0        DATA    0B4H
P4M1        DATA    0B3H
P5M0        DATA    0CAH
P5M1        DATA    0C9H
P6M0        DATA    0CCH
P6M1        DATA    0CBH
P7M0        DATA    0E2H
P7M1        DATA    0E1H

            ORG     0000H
            LJMP    MAIN

            ORG     0100H
MAIN:
            MOV     SP,#3FH

            MOV     P0M0,#00H                   ;设置P0.0~P0.7为双向口模式
            MOV     P0M1,#00H

            MOV     P1M0,#0FFH                  ;设置P1.0~P1.7为推挽输出模式
            MOV     P1M1,#00H

            MOV     P2M0,#00H                   ;设置P2.0~P2.7为高阻输入模式
            MOV     P2M1,#0FFH

            MOV     P3M0,#0FFH                  ;设置P3.0~P3.7为开漏模式
            MOV     P3M1,#0FFH

            JMP     $

            END

