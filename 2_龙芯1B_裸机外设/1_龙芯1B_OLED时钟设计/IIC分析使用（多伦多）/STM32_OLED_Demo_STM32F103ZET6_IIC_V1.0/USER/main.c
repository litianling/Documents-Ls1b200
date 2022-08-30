/****************************************************************************************************
//=========================================电源接线================================================//
//      5V  接DC 5V电源
//     GND  接地
//======================================OLED屏数据线接线==========================================//
//本模块数据总线类型为IIC
//     SCL  接PB13    // IIC时钟信号
//     SDA  接PB14    // IIC数据信号
//======================================OLED屏控制线接线==========================================//
//本模块数据总线类型为IIC，不需要接控制信号线    
//=========================================触摸屏接线=========================================//
//本模块本身不带触摸，不需要接触摸屏线
//============================================================================================//
**************************************************************************************************/	

#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "delay.h"
#include "usart.h"
#include "bsp_i2c.h"
 int main(void)
  {
		u8 a[]={'6','3','1','7','0','7','0','3','0','4','3','0','\0'};
		delay_init();	    	 //延时函数初始化	  
		NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	
		OLED_Init();			//初始化OLED  
		OLED_Clear(0)  	; 
		uart_init(115200);	 //串口初始化为115200
	  IIC_Init();
		
		
		
    OLED_WR_Byte(0x2e,OLED_CMD);//关闭滚动
    OLED_WR_Byte(0x2a,OLED_CMD);//选择滚动方式
    OLED_WR_Byte(0x00,OLED_CMD);//A:空字节
    OLED_WR_Byte(0x00,OLED_CMD);//B:滚动起始页
    OLED_WR_Byte(0x00,OLED_CMD);//C:水平滚动速度
    OLED_WR_Byte(0x01,OLED_CMD);//D:滚动结束页
    OLED_WR_Byte(0x03,OLED_CMD);//E:每次垂直滚动位移
		
		OLED_ShowCHinese(0,0,0);//张
		OLED_ShowCHinese(20,0,1);//大
		OLED_ShowCHinese(40,0,2);//炮
		
	  OLED_WR_Byte(0x2f,OLED_CMD);//开始滚动
		delay_ms(1000);delay_ms(1000);delay_ms(1000);delay_ms(1000);delay_ms(1000);

		OLED_Clear(0);  //清屏
		OLED_WR_Byte(0x2e,OLED_CMD);//关闭滚动
	while(1) 
	{
		read_AHT20_once();   
		OLED_ShowCHinese(0,0,0);//张
		OLED_ShowCHinese(20,0,1);//大
		OLED_ShowCHinese(40,0,2);//炮
		OLED_ShowString(0,2,a,16);
		delay_ms(1000);
	}	  	
}
