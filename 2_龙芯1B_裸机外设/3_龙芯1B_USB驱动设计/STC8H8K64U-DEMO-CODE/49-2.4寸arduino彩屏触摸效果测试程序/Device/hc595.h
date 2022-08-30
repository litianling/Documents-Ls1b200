#ifndef __HC595_H
#define __HC595_H	
	
#include "sys.h"

/*���Ŷ���*/
#define 				HC595_DS           P44        //SI
#define         HC595_DS_MODE      {P4M1&=~0x10;P4M0|=0x10;}	//�������

#define 				HC595_STCP          P43        //RCK
#define         HC595_STCP_MODE     {P4M1&=~0x08;P4M0|=0x08;}	//�������

#define 				HC595_SHCP          P42        //SCK
#define         HC595_SHCP_MODE     {P4M1&=~0x04;P4M0|=0x04;}	//�������

void hc595_init();                  //595��ʼ��
void hc595_send_multi_byte(uint8 *ddata,uint16 len);
void hc595_disable();               //595�����ֹ
void hc595_enable_nix();            //595ʹ�������
void hc595_enable_matrix();         //595ʹ�ܵ���
void hc595_bit_select(uint8 index); //595����λѡ
void hc595_bit_disable();	//595��Ӱ
#endif

