#ifndef __LED8_H
#define __LED8_H

#include <STC8HX.h>

#define LED8_CONTROL_PIN        P40
#define LED8_CONTROL_MODE       {P4M1&=~0x01;P4M0|=0x01;}//�������


void led8_enable();     //ʹ��led
void led8_disable();    //����led

#endif