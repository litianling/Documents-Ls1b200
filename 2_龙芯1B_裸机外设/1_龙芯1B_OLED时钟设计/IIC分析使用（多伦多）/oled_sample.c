void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
  if(hrtc->Instance==RTC)
  {
    RCC_OscInitTypeDef        RCC_OscInitStruct = {0};
    RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct = {0};
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    HAL_RCCEx_GetPeriphCLKConfig(&PeriphClkInitStruct);
    if (PeriphClkInitStruct.RTCClockSelection == RtcClockSource)
    { }  
    else
    {
      PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
      if (PeriphClkInitStruct.RTCClockSelection != RCC_RTCCLKSOURCE_NONE)
      {
        PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_NONE;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
          Error_Handler();
      }
      RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | 
RCC_OSCILLATORTYPE_LSE;
      RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
      RCC_OscInitStruct.LSIState = RCC_LSI_ON;
      RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
      if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
        Error_Handler();
      PeriphClkInitStruct.RTCClockSelection = RtcClockSource;
      if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        Error_Handler();
    }
    __HAL_RCC_RTC_ENABLE();
    __HAL_RCC_RTCAPB_CLK_ENABLE();
    HAL_NVIC_SetPriority(RTC_TAMP_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(RTC_TAMP_IRQn);
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* hrtc)
{
  if(hrtc->Instance==RTC)
  {
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();
    __HAL_RCC_RTCAPB_CLK_DISABLE();
    /* RTC interrupt DeInit */
    HAL_NVIC_DisableIRQ(RTC_TAMP_IRQn);
  }
}

  （6）在Keil左侧的Project窗口中，找到并双击main.c，打开该设计文件。在该文件中，添加设计代码。
   ○1添加变量定义及函数声明，如代码清单15-2所示
                代码清单15-2 main.c文件中添加变量定义及函数声明
//******************************************************************RTC相关变量
#define Init_Time_Year 0x21
#define Init_Time_Month 0x06
#define Init_Time_Date 0x014
#define Init_Time_Hours 0x08
#define Init_Time_Minutes 0x20
#define Init_Time_Seconds 0x00
#define Init_Time_SubSeconds 0x00

#define Alarm_A_Hours 0x08
#define Alarm_A_Minutes 0x20
#define Alarm_A_Seconds 0x10
#define Alarm_A_SubSeconds 0x00

#define Alarm_B_Hours 0x08
#define Alarm_B_Minutes 0x20
#define Alarm_B_Seconds 0x20
#define Alarm_B_SubSeconds 0x00

#include <stdio.h>
uint8_t Flag_Alarm = 0; //闹钟标志置位，判定是否处于闹钟模式
uint8_t Flag_Second_Old = 0; //降低OLED刷新速率--------------------避免卡BUG
uint8_t aShowTime[8] = "hh:ms:ss";
uint8_t aShowDate[10] = "dd-mm-yyyy";
static void RTC_CalendarShow(uint8_t *showtime, uint8_t *showdate);

//****************************************************IIC的数据缓冲区与基层函数

#define TXBUFFERSIZE  2 
unsigned char aTxBuffer_Command[TXBUFFERSIZE] = {0x00,0x00};
unsigned char aTxBuffer_Data[TXBUFFERSIZE] = {0x40,0x00};

void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Initial_M096128x64_ssd1306(void);

//******************************************************************IIC的图片显示

const unsigned char biaoqinbao[][128] = 
{ 
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0X80,0XC0,0XC0,0X60,0X60,0X30,0X30,0X30,
0X30,0X30,0X30,0X30,0X30,0X30,0X30,0X30,0XB0,0X30,0X20,0X60,0X60,0XC0,0XC0,0X80,
0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X80,0XE0,0X78,0X1C,0X0E,0X07,0X03,0X01,0X00,0X80,0X03,0X03,0X01,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X80,0X80,0X01,0X03,0X03,0X00,0X00,0X00,0X00,0X00,0X01,
0X03,0X07,0X0E,0X1C,0X70,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XF0,0XF8,
0X0E,0X07,0X01,0X08,0X1C,0X1C,0X1C,0X1C,0X1C,0X0F,0X07,0X33,0X30,0X60,0X60,0X60,
0X60,0X60,0X60,0X30,0X33,0X03,0X0F,0X0D,0X0C,0X0C,0X0C,0X0C,0X0C,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X87,0XFF,0X30,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0X1F,
0X38,0X70,0X60,0XC0,0XC0,0X80,0X80,0X80,0X80,0X80,0X20,0X60,0X40,0XC0,0XC0,0X80,
0XC0,0XC0,0XC0,0X60,0X30,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0X80,0X80,
0XC0,0X60,0X70,0X38,0X1C,0X0F,0X03,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X80,0XC0,0XE0,0X70,0X1C,0X8F,0XC3,0X61,0X31,0X19,0X08,0X00,0X80,0XC0,0X61,0X39,
0X1D,0X01,0X81,0XE0,0X70,0X38,0X00,0X00,0XF0,0XFE,0X0F,0X03,0X03,0X01,0X01,0X01,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XE0,0XE0,0XF0,0XF8,0XFE,0XE6,
0XC3,0XC7,0XC6,0X83,0X83,0XF9,0XFC,0X86,0X86,0X82,0X82,0X07,0X0D,0X0C,0X9C,0X9E,
0X9F,0X9B,0X99,0XB0,0XF0,0XF0,0XF8,0XFF,0XF7,0XF0,0XE0,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X01,0X03,0X03,0X03,
0X07,0X07,0X07,0X07,0X07,0X07,0X07,0X07,0X07,0X07,0X07,0X07,0X07,0X07,0X07,0X07,
0X07,0X07,0X07,0X07,0X03,0X03,0X03,0X03,0X01,0X01,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};

unsigned char show[][128]=
{
	{0x00,0x00,0x06,0x0A,0xFE,0x0A,0xC6,0x00,0xE0,0x00,0xF0,0x00,0xF8,0x00,0x00,0x00,
	0x00,0x00,0x00,0xFE,0x7D,0xBB,0xC7,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xC7,0xBB,
	0x7D,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x08,0x0C,0xFE,0xFE,0x0C,0x08,0x20,0x60,0xFE,0xFE,0x60,0x20,0x00,0x00,0x00,0x78,
	0x48,0xFE,0x82,0xBA,0xBA,0x82,0xBA,0xBA,0x82,0xBA,0xBA,0x82,0xBA,0xBA,0x82,0xFE},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
	0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0xFE,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0xFF,0xFF,0x00,0x00,0xFE,
	0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0xFF,0xFE,0x00,0x00,0x00,0x00,
	0xC0,0xC0,0xC0,0x00,0x00,0x00,0x00,0xFE,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
	0x03,0x03,0xFF,0xFE,0x00,0x00,0xFE,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
	0x03,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0xFF,
	0xFF,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0xFF,0xFF,0x00,0x00,0x00,0x00,
	0xE1,0xE1,0xE1,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,
	0x0C,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x0F,0x1F,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1F,0x0F,0x00,0x00,0x0F,
	0x1F,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1F,0x0F,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x1F,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
	0x18,0x18,0x1F,0x0F,0x00,0x00,0x0F,0x1F,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
	0x18,0x1F,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE2,0x92,0x8A,0x86,0x00,0x00,0x7C,0x82,0x82,
	0x82,0x7C,0x00,0xFE,0x00,0x82,0x92,0xAA,0xC6,0x00,0x00,0xC0,0xC0,0x00,0x7C,0x82,
	0x82,0x82,0x7C,0x00,0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0xC0,0xC0,0x00,0x7C,0x82,
	0x82,0x82,0x7C,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x24,0xA4,0x2E,0x24,0xE4,0x24,0x2E,0xA4,0x24,0x00,0x00,0x00,0xF8,
	0x4A,0x4C,0x48,0xF8,0x48,0x4C,0x4A,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x20,
	0x10,0x10,0x10,0x10,0x20,0xC0,0x00,0x00,0xC0,0x20,0x10,0x10,0x10,0x10,0x20,0xC0},
	{0x00,0x00,0x00,0x12,0x0A,0x07,0x02,0x7F,0x02,0x07,0x0A,0x02,0x00,0x00,0x00,0x0B,
	0x0A,0x0A,0x0A,0x7F,0x0A,0x0A,0x0A,0x0B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,
	0x40,0x40,0x40,0x50,0x20,0x5F,0x80,0x00,0x1F,0x20,0x40,0x40,0x40,0x50,0x20,0x5F}
};
void fill_picture(unsigned char fill_Data);
void Picture(int i);

//******************************************************************IIC的字符显示

const unsigned char L8H16[][8]=
{
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     //space 0
  {0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00},     //! 1
  {0x00,0x10,0x0C,0x06,0x10,0x0C,0x06,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     //" 2
  {0x40,0xC0,0x78,0x40,0xC0,0x78,0x40,0x00},
	{0x04,0x3F,0x04,0x04,0x3F,0x04,0x04,0x00},     //# 3
  {0x00,0x70,0x88,0xFC,0x08,0x30,0x00,0x00},
	{0x00,0x18,0x20,0xFF,0x21,0x1E,0x00,0x00},     //$ 4
  {0xF0,0x08,0xF0,0x00,0xE0,0x18,0x00,0x00},
	{0x00,0x21,0x1C,0x03,0x1E,0x21,0x1E,0x00},     //% 5
  {0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00},
	{0x1E,0x21,0x23,0x24,0x19,0x27,0x21,0x10},     //& 6
  {0x10,0x16,0x0E,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     //' 7
  {0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00},
	{0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00},     //( 8
  {0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00},
	{0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00},     //) 9
  {0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00},
	{0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00},     //* 10
  {0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00},
	{0x01,0x01,0x01,0x1F,0x01,0x01,0x01,0x00},     //+ 11
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x80,0xB0,0x70,0x00,0x00,0x00,0x00,0x00},     //, 12
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01},     //- 13
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00},     //. 14
  {0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x04},
	{0x00,0x60,0x18,0x06,0x01,0x00,0x00,0x00},     /// 15
  {0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00},
	{0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00},     //0 16
  {0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00},
	{0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},     //1 17
  {0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00},
	{0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00},     //2 18
  {0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00},
	{0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00},     //3 19
  {0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00},
	{0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00},     //4 20
  {0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00},
	{0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00},     //5 21
  {0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00},
	{0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00},     //6 22
  {0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00},
	{0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00},     //7 23
  {0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00},
	{0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00},    //8 24
  {0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00},
	{0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00},    //9 25
  {0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00},    //: 26
  {0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x80,0x60,0x00,0x00,0x00,0x00},    //; 27
  {0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x00},
	{0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x00},    //< 28
  {0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00},
	{0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00},    //= 29
  {0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00},
	{0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00},    //> 30
  {0x00,0x70,0x48,0x08,0x08,0x08,0xF0,0x00},
	{0x00,0x00,0x00,0x30,0x36,0x01,0x00,0x00},    //? 31
  {0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00},
	{0x07,0x18,0x27,0x24,0x23,0x14,0x0B,0x00},    //@ 32
  {0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00},
	{0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20},    //A 33
  {0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00},
	{0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00},    //B 34
  {0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00},
	{0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00},    //C 35
  {0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00},
	{0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00},    //D 36
  {0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00},
	{0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00},    //E 37
  {0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00},
	{0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00},    //F 38
  {0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00},
	{0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00},    //G 39
  {0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08},
	{0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20},    //H 40
  {0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00},
	{0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},    //I 41
  {0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00},
	{0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00},    //J 42
  {0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00},
	{0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00},    //K 43
  {0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00},
	{0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00},    //L 44
  {0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00},
	{0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00},    //M 45
  {0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08},
	{0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00},    //N 46
  {0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00},
	{0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00},    //O 47
  {0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00},
	{0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00},    //P 48
  {0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00},
	{0x0F,0x18,0x24,0x24,0x38,0x50,0x4F,0x00},    //Q 49
  {0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00},
	{0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20},    //R 50
  {0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00},
	{0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00},    //S 51
  {0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00},
	{0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00},    //T 52
  {0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08},
	{0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00},    //U 53
  {0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08},
	{0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00},    //V 54
  {0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00},
	{0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00},    //W 55
  {0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08},
	{0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20},    //X 56
  {0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00},
	{0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00},    //Y 57
  {0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00},
	{0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00},    //Z 58
  {0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00},
	{0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00},    //[ 59
  {0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00},    //\ 60
  {0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00},
	{0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00},    //] 61
  {0x00,0x00,0x04,0x02,0x02,0x02,0x04,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},    //^ 62
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80},    //_ 63
  {0x00,0x02,0x02,0x04,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},    //` 64
  {0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00},
	{0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20},    //a 65
  {0x08,0xF8,0x00,0x80,0x80,0x00,0x00,0x00},
	{0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00},    //b 66
  {0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00},
	{0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00},    //c 67
  {0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00},
	{0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20},    //d 68
  {0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00},
	{0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00},    //e 69
  {0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x18},
	{0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},    //f 70
  {0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00},
	{0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00},    //g 71
  {0x08,0xF8,0x00,0x80,0x80,0x80,0x00,0x00},
	{0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20},     //h 72
  {0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00},
	{0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},    //i 73
  {0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00},
	{0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00},    //j 74
  {0x08,0xF8,0x00,0x00,0x80,0x80,0x80,0x00},
	{0x20,0x3F,0x24,0x02,0x2D,0x30,0x20,0x00},    //k 75
  {0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00},
	{0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},     //l 76
  {0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00},
	{0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F},     //m 77
  {0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00},
	{0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20},     //n 78
  {0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00},
	{0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00},     //o 79
  {0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00},
	{0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00},     //p 80
  {0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00},
	{0x00,0x0E,0x11,0x20,0x20,0xA0,0xFF,0x80},     //q 81
  {0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00},
	{0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00},     //r 82
  {0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00},
	{0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00},     //s 83
  {0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00},
	{0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00},     //t 84
  {0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00},
	{0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20},     //u 85
  {0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80},
	{0x00,0x01,0x0E,0x30,0x08,0x06,0x01,0x00},     //v 86
  {0x80,0x80,0x00,0x80,0x00,0x80,0x80,0x80},
	{0x0F,0x30,0x0C,0x03,0x0C,0x30,0x0F,0x00},     //w 87
  {0x00,0x80,0x80,0x00,0x80,0x80,0x80,0x00},
	{0x00,0x20,0x31,0x2E,0x0E,0x31,0x20,0x00},     //x 88
  {0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80},
	{0x80,0x81,0x8E,0x70,0x18,0x06,0x01,0x00},     //y 89
  {0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00},
	{0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00},     //z 90
  {0x00,0x00,0x00,0x00,0x80,0x7C,0x02,0x02},
	{0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40},     //{ 91
  {0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00},     //| 92
  {0x00,0x02,0x02,0x7C,0x80,0x00,0x00,0x00},
	{0x00,0x40,0x40,0x3F,0x00,0x00,0x00,0x00},     //} 93
  {0x00,0x06,0x01,0x01,0x02,0x02,0x04,0x04},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     //~ 94
};
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr);
void OLED_ShowString(unsigned char  x,unsigned char  y, unsigned char  *p);
void OLED_ShowString_Short(unsigned char x,unsigned char  y, unsigned char  *p,unsigned char  l);
   ○2修改main主函数内的代码，如代码清单15-3所示。
               代码清单15-3 修改main主函数内的代码
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_RTC_Init();
	
	Initial_M096128x64_ssd1306();
	HAL_Delay(5);

	Picture(1);                         //显示一张图片--壁纸
	OLED_ShowString(22,2,"          ");  //清空原图片中间部分（显示日期与时间）
	OLED_ShowString(22,4,"          ");
	
	RTC_CalendarShow(aShowTime, aShowDate);              //获取日期
	OLED_ShowString(22,2,aShowDate);                      //字符串显示日期
	
  while (1)
  {
		RTC_CalendarShow(aShowTime, aShowDate);
		if((Flag_Alarm==0)&&(Flag_Second_Old!=aShowTime[7])) //非闹钟模式显示时钟
		{
			OLED_ShowString_Short(28,4,aShowTime,8);        //限定长度8位
			Flag_Second_Old = aShowTime[7];
		}
  }
}
○3修改RTC初始化函数，如代码清单15-4所示。
           代码清单15-4 修改RTC初始化函数
static void MX_RTC_Init(void)
{
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};
  /** Initialize RTC Only */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initialize RTC and set the Time and Date  */
  sTime.Hours = Init_Time_Hours;
  sTime.Minutes = Init_Time_Minutes;
  sTime.Seconds = Init_Time_Seconds;
  sTime.SubSeconds = Init_Time_SubSeconds;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = Init_Time_Month;  //RTC_MONTH_JUNE;
  sDate.Date = Init_Time_Date;
  sDate.Year = Init_Time_Year;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable the Alarm A  */
  sAlarm.AlarmTime.Hours = Alarm_A_Hours;
  sAlarm.AlarmTime.Minutes = Alarm_A_Minutes;
  sAlarm.AlarmTime.Seconds = Alarm_A_Seconds;
  sAlarm.AlarmTime.SubSeconds = Alarm_A_SubSeconds;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
  sAlarm.AlarmDateWeekDay = RTC_WEEKDAY_MONDAY;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable the Alarm B  */
  sAlarm.AlarmTime.Hours = Alarm_B_Hours;
  sAlarm.AlarmTime.Minutes = Alarm_B_Minutes;
  sAlarm.AlarmTime.Seconds = Alarm_B_Seconds;
	sAlarm.AlarmTime.SubSeconds = Alarm_B_SubSeconds;
  sAlarm.Alarm = RTC_ALARM_B;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
}
○4添加自定义函数，如代码清单15-5所示。
                 代码清单15-5 添加自定义函数
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1);		//点亮LED灯
	Picture(2);                                   //显示二张图片--起床表情包
	Flag_Alarm=1;		                       //闹钟模式标志位启用
}

void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc)
{
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);		 //关闭LED灯
	Picture(1);                                    //显示一张图片--壁纸
	OLED_ShowString(22,2,"          ");       //清空原图片中间部分（显示日期与时间）
	OLED_ShowString(22,4,"          ");
	OLED_ShowString(22,2,aShowDate);              //字符串显示日期
	Flag_Alarm=0;		                        //闹钟模式标志位关闭
}

static void RTC_CalendarShow(uint8_t *showtime, uint8_t *showdate)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;

  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
  /* Display time Format : hh:mm:ss */
  sprintf((char *)showtime, "%2d:%2d:%2d", stimestructureget.Hours, 
stimestructureget.Minutes, stimestructureget.Seconds);
  /* Display date Format : mm-dd-yy */
  sprintf((char *)showdate, "%2d-%2d-%2d", sdatestructureget.Month, sdatestructureget.Date, 
2000 + sdatestructureget.Year);
}

//****************************************************************IIC基层函数

void Write_IIC_Command(unsigned char IIC_Command)
{
	aTxBuffer_Command[1]=IIC_Command;
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)I2C_ADDRESS,
 (uint8_t *)aTxBuffer_Command, TXBUFFERSIZE, 10000);
}

void Write_IIC_Data(unsigned char IIC_Data)
{
	aTxBuffer_Data[1]=IIC_Data;
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)I2C_ADDRESS, (uint8_t *)aTxBuffer_Data, 
TXBUFFERSIZE, 10000);
}

void Initial_M096128x64_ssd1306()       //该段命令的含义参考15.3.8一节内容
{ 
	Write_IIC_Command(0xAE);       
	Write_IIC_Command(0x20);	    
	Write_IIC_Command(0x10);	
	Write_IIC_Command(0xb0);	 
	Write_IIC_Command(0xc8);	
	Write_IIC_Command(0x00);
	Write_IIC_Command(0x10);
	Write_IIC_Command(0x40);
	Write_IIC_Command(0x81);
	Write_IIC_Command(0xdf);
	Write_IIC_Command(0xa1);
	Write_IIC_Command(0xa6);
	Write_IIC_Command(0xa8);
	Write_IIC_Command(0x3F);
	Write_IIC_Command(0xa4);
	Write_IIC_Command(0xd3);
	Write_IIC_Command(0x00);
	Write_IIC_Command(0xd5);
	Write_IIC_Command(0xf0);
	Write_IIC_Command(0xd9);
	Write_IIC_Command(0x22);
	Write_IIC_Command(0xda);
	Write_IIC_Command(0x12);
	Write_IIC_Command(0xdb);
	Write_IIC_Command(0x20);
	Write_IIC_Command(0x8d);
	Write_IIC_Command(0x14);
	Write_IIC_Command(0xaf); 
}

//************************IIC的图片显示

void Picture(int i)
{
  for(unsigned char y=0;y<8;y++)
    {
      Write_IIC_Command(0xb0+y);
      Write_IIC_Command(0x0);
      Write_IIC_Command(0x10);
      for(unsigned char x=0;x<128;x++)
	  {	
		if(i==1)
			Write_IIC_Data(show[y][x]);
		else if(i==2)
			Write_IIC_Data(biaoqinbao[y][x]);
		else ;
	   }
    }
}


void fill_picture(unsigned char fill_Data)
{
	for(unsigned char m=0;m<8;m++)
	{
	 Write_IIC_Command(0xb0+m);	//页面0-页面1
	 Write_IIC_Command(0x00);		//低列起始地址
	 Write_IIC_Command(0x10);		//高列起始地址
	   for(unsigned char n=0;n<128;n++)
		{
		 Write_IIC_Data(fill_Data);
		}
	 }
}

//***************************************************************IIC的字符显示

void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr)
{
	unsigned char nomber=0;	
	nomber=chr-' ';           //得到偏移后的值即ASC码偏移量  设置空格为0号字符
	if(x>127)	                        //如果超出这一行自动跳转到下一行（+2）
	{
		x=0;
		y=y+2;
	} 
   Write_IIC_Command(0xb0+y);
   Write_IIC_Command(0x00+x%16);     //低四位横坐标
   Write_IIC_Command(0x10+x/16);     //高四位横坐标	
	for(int i=0;i<8;i++)
		Write_IIC_Data(L8H16[nomber*2][i]);
	
   Write_IIC_Command(0xb0+y+1);
   Write_IIC_Command(0x00+x%16);
   Write_IIC_Command(0x10+x/16);
	for(int i=0;i<8;i++)
		Write_IIC_Data(L8H16[nomber*2+1][i]);
	
}

void OLED_ShowString(unsigned char x,unsigned char y,unsigned char *chr) 
{
	unsigned char i=0;
	while (chr[i]!='\0')	                   //不是字符串的结束则一直循环
	{	
		OLED_ShowChar(x,y,chr[i]);	       //在x，y处显示字符
		x+=8;						   //x=x+8 列地址加8准备显示下一字符
		if(x>120)						   //位置不够显示当前字符，去下一行显示
		{
		  x=0;
		  y+=2;
		}
		i++;	                           //扫描下一字符
	}
}

void OLED_ShowString_Short(unsigned char  x,unsigned char  y, unsigned char  
*chr,unsigned char  l)
{
	unsigned char i=0;
	while (chr[i]!='\0'&&i<l)	         //不是字符串并且小于长度
	{	
		OLED_ShowChar(x,y,chr[i]);	     //在x，y处显示字符
		x+=8;					   	 //x=x+8 列地址加8准备显示下一字符
		if(x>120)					  	 //位置不够显示当前字符，去下一行显示
		{
			x=0;
			y+=2;
		}
		 i++;	                     //扫描下一字符
	}
}
