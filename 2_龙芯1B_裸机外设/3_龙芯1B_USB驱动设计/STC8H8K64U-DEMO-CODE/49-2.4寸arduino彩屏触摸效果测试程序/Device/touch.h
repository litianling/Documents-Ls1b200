#ifndef __TOUCH_H
#define __TOUCH_H	

#include "sys.h"


#define TOUCH_EEPROM_ADDR				0x01F3		//触摸校准的数据在内部eeprom中的存储地址，一共需12个字节

#define TOUCH_READ_TIMES 				5 	//读取次数
#define TOUCH_LOST_VAL 					1	//丢弃值

//引脚定义

#define SM245_TOUCH_YP_PIN            P15//11        //AD
#define SM245_SET_YP_OUT            { P1M1&=~0x20;P1M0|=0x20; }   //P11推挽输出
#define SM245_SET_YP_IN             { P1M1|=0x20;P1M0&=~0x20; }   //INPUT高阻输入
#define SM245_YP_ADC_PIN              ADC_P15//11  

#define SM245_TOUCH_XM_PIN            P16//03        //AD
#define SM245_SET_XM_OUT            { P1M1&=~0x40;P1M0|=0x40;}   //P03推挽输出
#define SM245_SET_XM_IN             { P1M1|=0x40;P1M0&=~0x40;}   //INPUT高阻输入
#define SM245_XM_ADC_PIN              ADC_P16//03   

#define SM245_TOUCH_YM_PIN            P07//67
#define SM245_SET_YM_OUT            { P0M1&=~0x80;P0M0|=0x80;}   //P67推挽输出
#define SM245_SET_YM_IN             { P0M1|=0x80;P0M0&=~0x80;}   //INPUT高阻输入

#define SM245_TOUCH_XP_PIN           P06//66
#define SM245_SET_XP_OUT            { P0M1&=~0x40;P0M0|=0x40;}   //P66推挽输出
#define SM245_SET_XP_IN             { P0M1|=0x40;P0M0&=~0x40;}   //INPUT高阻输入 

#define LCD9341_TOUCH_YP_PIN            P16//03        //A2
#define LCD9341_SET_YP_OUT            { P1M1&=~0x40;P1M0|=0x40;}   //P03推挽输出
#define LCD9341_SET_YP_IN             { P1M1|=0x40;P1M0&=~0x40;}   //INPUT高阻输入
#define LCD9341_YP_ADC_PIN              ADC_P16//03 

#define LCD9341_TOUCH_XM_PIN            P17//13        //A3
#define LCD9341_SET_XM_OUT            { P1M1&=~0x80;P1M0|=0x80; }   //P11推挽输出
#define LCD9341_SET_XM_IN             { P1M1|=0x80;P1M0&=~0x80; }   //INPUT高阻输入
#define LCD9341_XM_ADC_PIN             ADC_P17//13 

#define LCD9341_TOUCH_YM_PIN           P00//60		//8
#define LCD9341_SET_YM_OUT            { P0M1&=~0x01;P0M0|=0x01;}   //P67推挽输出
#define LCD9341_SET_YM_IN             { P0M1|=0x01;P0M0&=~0x01;}   //INPUT高阻输入

#define LCD9341_TOUCH_XP_PIN            P01//61		//9
#define LCD9341_SET_XP_OUT            { P0M1&=~0x02;P0M0|=0x02;}   //P66推挽输出
#define LCD9341_SET_XP_IN             { P0M1|=0x02;P0M0&=~0x02;}   //INPUT高阻输入 

void touch_init(uint8 t); //触摸初始化程序
uint16* touch_get_point();
uint16 touch_read_x();	//读取x校准过的坐标
uint16 touch_read_y();	//读取y校准过的坐标
uint16 touch_read_pressure(); //读取触摸的压力值.

#endif      //touch.h