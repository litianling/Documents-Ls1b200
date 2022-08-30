/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序        */
/*---------------------------------------------------------------------*/


/*************  技术支持与购买说明    **************
产品主页：http://tw51.haohaodada.com
淘宝搜索：天问51，可购买。目前基础版99元，带彩屏标准备版149元
技术支持QQ群一：1138055784
******************************************/


/*************  本程序功能说明  **************

TFT彩屏触摸校准加简易手写板程序
下载时, 选择时钟 24MHZ (用户可自行修改频率).

******************************************/
#include "sys.h"
#include "hc595.h"
#include "led8.h"
#include "delay.h"
#include "tftlcd.h"
#include "touch.h"

uint8 BOXSIZE=40;
void wrsb()
{
    tft_lcd_fill(0, 0, BOXSIZE, BOXSIZE, TFT_LCD_RED);
    tft_lcd_fill(BOXSIZE, 0, BOXSIZE*2, BOXSIZE, TFT_LCD_YELLOW);
    tft_lcd_fill(BOXSIZE*2, 0, BOXSIZE*3, BOXSIZE, TFT_LCD_GREEN);
    tft_lcd_fill(BOXSIZE*3, 0, BOXSIZE*4, BOXSIZE, TFT_LCD_CYAN);
    tft_lcd_fill(BOXSIZE*4, 0, BOXSIZE*5, BOXSIZE, TFT_LCD_BLUE);
    tft_lcd_fill(BOXSIZE*5, 0, BOXSIZE*6, BOXSIZE, TFT_LCD_MAGENTA);
}

uint16 bicolor=TFT_LCD_RED;
uint16 *t;
uint16 pp_x=0,pp_y=0,pp_z=0;
/********************** 主函数 ************************/
void main(void)
{
//	  led8_disable();					//LED Power OFF
//    hc595_init(); 
//   	hc595_disable();
	
		tft_lcd_init();		//彩屏初始化
		tft_lcd_set_windows(0,0,239,319);
	
    touch_init(1);
    wrsb();
	
    while(1)
    {
			  t = touch_get_point();
				pp_x = *t;
				pp_y = *(t+1);
				pp_z = *(t+2);
				if(bicolor==TFT_LCD_RED){
					tft_lcd_draw_rectangle(1,1,39,39,TFT_LCD_RED); 
				}
				else if(bicolor==TFT_LCD_YELLOW)
				{
					tft_lcd_draw_rectangle(41,1,79,39,TFT_LCD_YELLOW);  
				}
				else if(bicolor==TFT_LCD_GREEN)
				{
					tft_lcd_draw_rectangle(81,1,119,39,TFT_LCD_YELLOW);  
				}
				else if(bicolor==TFT_LCD_CYAN)
				{
					tft_lcd_draw_rectangle(121,1,159,39,TFT_LCD_CYAN);  
				}
				else if(bicolor==TFT_LCD_BLUE)
				{
					tft_lcd_draw_rectangle(161,1,199,39,TFT_LCD_BLUE);  
				}
				else if(bicolor==TFT_LCD_MAGENTA)
				{
					tft_lcd_draw_rectangle(201,1,239,39,TFT_LCD_MAGENTA);  
				}
				if(pp_x>0&&pp_x<40&&pp_y>0&&pp_y<40)
				{
						wrsb();
						bicolor=TFT_LCD_RED;
				}
				else if(pp_x>40&&pp_x<80&&pp_y>0&&pp_y<40)
				{
						wrsb();
						bicolor=TFT_LCD_YELLOW;        
				}
				else if(pp_x>80&&pp_x<120&&pp_y>0&&pp_y<40)
				{
						wrsb();
						bicolor=TFT_LCD_GREEN;           
				}
				else if(pp_x>120&&pp_x<160&&pp_y>0&&pp_y<40)
				{
						wrsb();
						bicolor=TFT_LCD_CYAN;           
				}
				else if(pp_x>160&&pp_x<200&&pp_y>0&&pp_y<40)
				{
						wrsb();
						bicolor=TFT_LCD_BLUE;           
				}
				else if(pp_x>200&&pp_x<240&&pp_y>0&&pp_y<40)
				{
						wrsb();
						bicolor=TFT_LCD_MAGENTA;           
				}
				else if(pp_y>40&&pp_y<319&&pp_x>20&&pp_x<239&&pp_z > 1000)
				{
						tft_lcd_fill(pp_x, pp_y, pp_x+2,pp_y+2, bicolor);
				}
    }
} 





