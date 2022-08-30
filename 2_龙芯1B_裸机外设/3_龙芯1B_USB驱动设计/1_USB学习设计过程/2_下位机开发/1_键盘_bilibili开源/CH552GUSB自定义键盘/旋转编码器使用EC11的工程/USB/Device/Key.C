/********************************** (C) COPYRIGHT *******************************
CH552e模拟USB多媒体键盘,旋转编码器为  EC11
2020/4/9 
HID报告描述符参考文章：https://www.cnblogs.com/AlwaysOnLines/p/4552840.html
参考书籍：圈圈教你玩USB，HID用途表1.12，HID1.11协议
以下很多都是复制大佬的程序，加了些自己的理解注释
*******************************************************************************/

#include "./Public/CH554.H"                                                      
#include "./Public/DEBUG.H"
#include <string.h>
#include <stdio.h>


#define THIS_ENDP0_SIZE         DEFAULT_ENDP0_SIZE
#define CapsLockLED 0x02

UINT8X  Ep0Buffer[8> (THIS_ENDP0_SIZE+2)? 8:(THIS_ENDP0_SIZE+2)] _at_ 0x0000;  //端点0 OUT&IN缓冲区，必须是偶地址
UINT8X  Ep1Buffer[64>(MAX_PACKET_SIZE+2)?64:(MAX_PACKET_SIZE+2)] _at_ 0x000a;  //端点1 IN缓冲区,必须是偶地址
UINT8X  Ep2Buffer[64<(MAX_PACKET_SIZE+2)?64:(MAX_PACKET_SIZE+2)] _at_ 0x0050;  //端点2 IN缓冲区,必须是偶地址

UINT8   SetupReq,SetupLen,Ready,Count,FLAG,UsbConfig,LED_VALID;
PUINT8  pDescr;   						//USB配置标志
USB_SETUP_REQ   SetupReqBuf;  //暂存Setup包
bit Ep2InKey;
#define UsbSetupBuf     ((PUSB_SETUP_REQ)Ep0Buffer)
#define DEBUG 0
#pragma  NOAREGS

//定义的4个按键以及EC11的A、B脚
sbit key1=P1^4;				// 按键1
sbit key2=P1^5;				// 按键2
sbit key3=P1^6;				// 按键3
sbit key4=P1^7;				// 按键4
sbit EC11_A = P3^1; 	// 两线四相编码器A线
sbit EC11_B = P3^0;		// 两线四相编码器B线

UINT8 EC11_A_State = 0;  // 编码器A线最新状态
UINT8 EC11_A_PState = 0; // 编码器A线上一状态

UINT8 KeyState[6] = {1,1,1,1,1,1}; //按键状态
UINT8 BackState[6] = {1,1,1,1,1,1}; //按键上一次的状态

UINT8 T0RH = 0;	//T0高8位重载值
UINT8 T0RL = 0;	//T0低8位重载值

unsigned long pdata TimeThr[4] = {1000, 1000, 1000, 1000};	// 长按时间临界值
unsigned long pdata KeyDownTime[4]= {0, 0, 0, 0};						// 持续按下某个按键的时间

//按键1,按键2,按键3,按键4,按键5,按键6
UINT8C key_code_map[6] = {0x31,0x32,0x33,0x34,0x35,0x36};   

/*普通键盘数据*/
UINT8 HIDKey[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

/*多媒体按键数据*/
UINT8 HIDKeyMUL[4] = {0x00,0x00,0x00,0x00};

/****字符串描述符****/
/*HID类报表描述符*/
UINT8C KeyRepDesc[62] =
{
/******************************************************************
键盘发送给PC的数据每次8个字节：BYTE1 BYTE2 BYTE3 BYTE4 BYTE5 BYTE6 BYTE7 BYTE8。定义分别是：
BYTE1 --
       |--bit0:   Left Control 
       |--bit1:   Left Shift 
       |--bit2:   Left Alt 
       |--bit3:   Left GUI 
       |--bit4:   Right Control  
       |--bit5:   Right Shift 
       |--bit6:   Right Alt 
       |--bit7:   Right GUI 
BYTE2 -- 暂不清楚，有的地方说是保留位
BYTE3--BYTE8 -- 这六个为普通按键
*******************************************************************/
    0x05,0x01,0x09,0x06,0xA1,0x01,0x05,0x07,
    0x19,0xe0,0x29,0xe7,0x15,0x00,0x25,0x01,
    0x75,0x01,0x95,0x08,0x81,0x02,0x95,0x01,
    0x75,0x08,0x81,0x01,0x95,0x03,0x75,0x01,
    0x05,0x08,0x19,0x01,0x29,0x03,0x91,0x02,
    0x95,0x05,0x75,0x01,0x91,0x01,0x95,0x06,
    0x75,0x08,0x26,0xff,0x00,0x05,0x07,0x19,
    0x00,0x29,0x91,0x81,0x00,0xC0
};
/*多媒体键盘报表描述符*/
UINT8C KeyMULRepDesc[105] =	
{
/**********************************************************************************************
键盘发送给PC的数据每次4个字节：BYTE1 BYTE2 BYTE3 BYTE4
BYTE1 BYTE2 BYTE3 这3个字节分成24位，每个位代表一个按键，1代表按下，0抬起。
BYTE1 --
       |--bit0:  Vol-  
       |--bit1:  Vol+ 
       |--bit2:  Mute  
       |--bit3:  Email 
       |--bit4:  Media   
       |--bit5:  WWW Home 
       |--bit6:  Play/Pause 
       |--bit7:  Scan Pre Track 
BYTE2 BYTE3按下面的顺序排下去，BYTE3 bit7：最后一个Usage( NULL )。
BYTE4 --
    系统功能按键，关机(0x81)，休眠(0x82），唤醒（0x83）
***********************************************************************************************/
		0x05, 0x0C, //USAGE_PAGE 用途页选择0x0c(用户页)
		0x09, 0x01, //USAGE 接下来的应用集合用于用户控制
		0xA1, 0x01, //COLLECTION 开集合
		0x15, 0x00, //LOGICAL_MINIMUM (0)
		0x25, 0x01, //LOGICAL_MAXIMUM (1)
		0x0A, 0xEA, 0x00,		/* Usage( Vol- ) */
		0x0A, 0xE9, 0x00,		/* Usage( Vol+ ) */
		0x0A, 0xE2, 0x00,		/* Usage( Mute ) */
		0x0A, 0x8A, 0x01,		/* Usage( Email ) */
		0x0A, 0x83, 0x01,		/* Usage( Media ) */
		0x0A, 0x23, 0x02,		/* Usage( WWW Home ) */
		0x0A, 0xCD, 0x00,		/* Usage( Play/Pause ) */
		0x0A, 0xB6, 0x00,		/* Usage( Scan Pre Track ) */
		0x0A, 0xB5, 0x00,		/* Usage( Scan Next Track ) */
		0x0A, 0xB7, 0x00,		/* Usage( Stop ) */
		0x0A, 0x00, 0x00,		/* Usage( NULL ) */
		0x0A, 0x00, 0x00,		/* Usage( NULL ) */
		0x0A, 0x00, 0x00,		/* Usage( NULL ) */
		0x0A, 0x00, 0x00,		/* Usage( NULL ) */
		0x0A, 0x00, 0x00,		/* Usage( NULL ) */
		0x0A, 0x00, 0x00,		/* Usage( NULL ) */
		0x0A, 0x00, 0x00,		/* Usage( NULL ) */
		0x0A, 0x00, 0x00,		/* Usage( NULL ) */
		0x0A, 0x00, 0x00,		/* Usage( NULL ) */
		0x0A, 0x00, 0x00,		/* Usage( NULL ) */
		0x0A, 0x00, 0x00,		/* Usage( NULL ) */
		0x0A, 0x00, 0x00,		/* Usage( NULL ) */
		0x0A, 0x00, 0x00,		/* Usage( NULL ) */
		0x0A, 0x11, 0x22,		/* Usage( NULL ) */
		0x75, 0x01, //REPORT_SIZE (1)
		0x95, 0x18, //REPORT_COUNT (24)
		0x81, 0x02, //INPUT (Data,Var,Abs)输入24bit数据
		0x05, 0x01, //USAGE_PAGE 用途页0x01(普通桌面)
		0x19, 0x00, //USAGE_MINIMUM 用途最小值0x00(未定义)
		0x29, 0x83, //USAGE_MAXIMUM 用途最大值0x83(系统唤醒)
		0x15, 0x00, //LOGICAL_MINIMUM (0)
		0x25, 0x83, //LOGICAL_MAXIMUM (83)
		0x75, 0x08, //REPORT_SIZE (8)
		0x95, 0x01, //REPORT_COUNT (1)
		0x81, 0x00, //INPUT (Data,Ary,Abs)输入1字节数据
		0xC0//END_COLLECTION 闭合集合
};


/*设备描述符*/
UINT8C DevDesc[18] = {
   0x12,      //bLength字段。设备描述符的长度为18(0x12)字节
   0x01,	  	//bDescriptorType字段。设备描述符的编号为0x01
   0x10,0x01, //bcdUSB字段。这里设置版本为USB1.1，即0x0110。
              //由于是小端结构，所以低字节在先，即0x10，0x01。
   0x00,	  	//bDeviceClass字段。我们不在设备描述符中定义设备类，
              //而在接口描述符中定义设备类，所以该字段的值为0。
   0x00,	  	//bDeviceSubClass字段。bDeviceClass字段为0时，该字段也为0。
   0x00,	  	//bDeviceProtocol字段。bDeviceClass字段为0时，该字段也为0。
   0x08,	  	//bMaxPacketSize0字段。 的端点0大小的8字节。
   0x3d,0x41, //idVender字段,注意小端模式，低字节在先。
   0x3a,0x55, //idProduct字段 产品ID号。注意小端模式，低字节应该在前。
   0x00,0x00, //bcdDevice字段。注意小端模式，低字节应该在前。
   0x00,	 	 	//iManufacturer字段。厂商字符串的索引
   0x00,		  //iProduct字段。产品字符串的索引值,注意字符串索引值不要使用相同的值。
   0x00,	 	 	//iSerialNumber字段。设备的序列号字符串索引值。
   0x01		  	//bNumConfigurations字段。该设备所具有的配置数。
};
/*配置描述符*/
UINT8C CfgDesc[59] =
{
 /*配置描述符*/
  0x09, //bLength字段。配置描述符的长度为9字节
	0x02, //bDescriptorType字段。配置描述符编号为0x02
	0x3b, //wTotalLength字段。配置描述符集合的总长度0x003b，包括配置描述符本身、接口描述符、类描述符、端点描述符等，LSB
	0x00, 
	0x02, //bNumInterfaces字段。该配置包含的接口数，只有2个接口
	0x01, //bConfiguration字段。该配置的值为1
	0x01, //iConfigurationz字段，该配置的字符串索引。
	0xA0, //bmAttributes字段,bit4-bit7描述设备的特性
	0x64, //bMaxPower字段，该设备需要的最大电流量。每单位电流为 2 mA    
 /*接口描述符*/
	//接口1，普通键盘
  0x09,0x04,0x00,0x00,0x01,0x03,0x01,0x01,0x00, //接口描述符,键盘  HID设备的定义放置在接口描述符中
  0x09,0x21,0x11,0x01,0x00,0x01,0x22,0x3e,0x00, //HID类描述符
  0x07,0x05,0x81,0x03,0x08,0x00,0x0a, //端点描述符
	//接口2，多媒体按键
	0x09,0x04,0x01,0x00,0x01,0x03,0x00,0x00,0x00, // 接口描述符
	0x09,0x21,0x00,0x01,0x00,0x01,0x22,0x69,0x00, // HID类描述符
	0x07,0x05,0x82,0x03,0x04,0x00,0x0a,	// 端点描述符 
};
/*******************************************************************************
* Function Name  : USBDeviceInit()
* Description    : USB设备模式配置,设备模式启动，收发端点配置，中断开启
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBDeviceInit()
{
	  IE_USB = 0;
	  USB_CTRL = 0x00; 			//先设定USB设备模式
    UEP0_DMA = Ep0Buffer; //端点0数据传输地址
    UEP1_DMA = Ep1Buffer; //端点1数据传输地址
		UEP2_DMA = Ep2Buffer; //端点2数据传输地址
    UEP4_1_MOD = ~(bUEP4_RX_EN | bUEP4_TX_EN |bUEP1_RX_EN | bUEP1_BUF_MOD) | bUEP4_TX_EN;//端点1单64字节收发缓冲区,端点0收发
		UEP2_3_MOD = UEP2_3_MOD & ~bUEP2_BUF_MOD | bUEP2_TX_EN;
    UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK; 	//OUT事务返回ACK，IN事务返回NAK
    UEP1_CTRL = bUEP_T_TOG | UEP_T_RES_NAK;			//端点1手动翻转同步标志位，IN事务返回NAK
		UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK; 	//端点2自动翻转同步标志位，IN事务返回NAK
		
	  USB_DEV_AD = 0x00;
	  UDEV_CTRL = bUD_PD_DIS; 	// 禁止DP/DM下拉电阻
	  USB_CTRL = bUC_DEV_PU_EN | bUC_INT_BUSY | bUC_DMA_EN; //启动USB设备及DMA，在中断期间中断标志未清除前自动返回NAK
	  UDEV_CTRL |= bUD_PORT_EN; // 允许USB端口
	  USB_INT_FG = 0xFF; 				// 清中断标志
	  USB_INT_EN = bUIE_SUSPEND | bUIE_TRANSFER | bUIE_BUS_RST;
	  IE_USB = 1;
}
/*******************************************************************************
* Function Name  : Enp1IntIn()
* Description    : USB设备模式端点1的中断上传
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Enp1IntIn( )
{
    memcpy( Ep1Buffer, HIDKey, sizeof(HIDKey)); //加载上传数据（将普通按键数据加载到端点1数据缓存区）
    UEP1_T_LEN = sizeof(HIDKey); 								//上传数据长度
    UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK; //有数据时上传数据并应答ACK
}
/*******************************************************************************
* Function Name  : Enp2IntIn()
* Description    : USB设备模式端点2的中断上传
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Enp2IntIn( )
{
    memcpy( Ep2Buffer, HIDKeyMUL, sizeof(HIDKeyMUL)); //加载上传数据（将多媒体按键数据加载到端点2数据缓存区）
    UEP2_T_LEN = sizeof(HIDKeyMUL); 									//上传数据长度
    UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK; //有数据时上传数据并应答ACK
}

/*******************************************************************************
* Function Name  : DeviceInterrupt()
* Description    : CH559USB中断处理函数
* 触发此中断的四种原因：USB传输完成，USB总线复位，USB总线挂起/唤醒完成，意外触发中断
* USB传输完成支持以下几种事务：
		0号端点采用中断下载模式
		0号端点采用中断上传模式
		1号端点采用中断上传模式
		2号端点采用中断上传模式
		setup事务
* OUT和IN是针对上位机（主机端、电脑而言的）
		OUT：主机端输出，设备端接收
		IN： 主机端输入，设备端传输
*******************************************************************************/
void    DeviceInterrupt( void ) interrupt INT_NO_USB using 1               	//USB中断服务程序,使用寄存器组1
{
    UINT8 len;																															//定义传输长度
    if(UIF_TRANSFER)                                                        //如果USB传输完成触发中断  USB传输完成中断标志，直接位地址清除或写入1来清除  
    {
        switch (USB_INT_ST & (MASK_UIS_TOKEN | MASK_UIS_ENDP))							//通过这个switch来确定USB传输类型（哪个端点，上行还是下行）
        {
					case UIS_TOKEN_OUT | 0:  																					//0号端点采用中断下载模式
            len = USB_RX_LEN;																								//传输长度==只读模式USB接收长度
            if((SetupReq == 0x09)&& (len == 1))															
            {
              LED_VALID = Ep0Buffer[0];							
            }
            else if((SetupReq == 0x09) && (len == 8))
						{
							//SetReport						 
            }							
            UEP0_T_LEN = 0;  																								//0号端点传输长度清零（虽然尚未到状态阶段，但是提前预置上传0长度数据包以防主机提前进入状态阶段）
            UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_ACK;											//默认数据包是DATA0,返回应答ACK
          break;
					
					case UIS_TOKEN_IN | 0:                                            //0号端点采用中断上传模式
            switch(SetupReq)
            {
              case USB_GET_DESCRIPTOR:																			//USB标准设备请求码————USB获取描述符
                len = SetupLen >= 8 ? 8 : SetupLen;                         //本次传输长度（在设置的长度与8之间取一个较小的数值）
                memcpy( Ep0Buffer, pDescr, len );                           //加载上传数据（将端点0数据缓存区长度为len的数据加载到USB配置标志pDescr）
                SetupLen -= len;																						//数据长度变化
                pDescr += len;
                UEP0_T_LEN = len;
                UEP0_CTRL ^= bUEP_T_TOG;                                    //端点0同步标志位翻转（USB端点X传输(IN)的准备数据切换标志:0=DATA0, 1=DATA1）
              break;
              case USB_SET_ADDRESS:																					//USB标准设备请求码————USB设置地址
                USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | SetupLen;						//更改USB设备地址（USB设备地址，低7位为USB设备地址）（通用标记位）
                UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;									//更改端点0控制寄存器做出应答
              break;
              default:																											//其他
                UEP0_T_LEN = 0;                                             //状态阶段完成中断或者是强制上传0长度数据包结束控制传输
                UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;									//更改端点0控制寄存器做出应答
              break;
            }
          break;

					case UIS_TOKEN_IN | 1:                                            //1号端点采用中断上传模式
            UEP1_T_LEN = 0;                                                 //清0，1号端点预计发送数据的长度
//          UEP2_CTRL ^= bUEP_T_TOG;                                        //如果不设置自动翻转则需要手动翻转
            UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;       //默认应答NAK
            FLAG = 1;                                                       /*传输完成标志置一，代表传输结束*/
          break;
						
					case UIS_TOKEN_IN | 2:                                            //2号端点采用中断上传模式
						UEP2_T_LEN = 0;                                                 //清0，2号端点预计发送数据的长度
//          UEP1_CTRL ^= bUEP_T_TOG;                                        //如果不设置自动翻转则需要手动翻转
            UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;       //默认应答NAK
						FLAG = 1; 																											/*传输完成标志置一，代表传输结束*/
          break;

					case UIS_TOKEN_SETUP | 0:                                         //SETUP事务
            len = USB_RX_LEN;																								//此次数据包长度==USB接收数据的长度
            if(len == (sizeof(USB_SETUP_REQ)))															//如果此数据长度==USB设置请求数据包（结构体）长度
            {
                SetupLen = UsbSetupBuf->wLengthL;														//获取设定长度
                if(UsbSetupBuf->wLengthH || SetupLen > 0x7F )								//如果长度过长溢出
                {
                    SetupLen = 0x7F;    																		//长度重新赋值为被允许的最大长度，限制总长度
                }
                len = 0;                                                    //默认为成功并且上传0长度
                SetupReq = UsbSetupBuf->bRequest;														//获取请求码
                if ( ( UsbSetupBuf->bRequestType & USB_REQ_TYP_MASK ) != USB_REQ_TYP_STANDARD )		/* HID类命令 */
                {		//如果请求类型 按位与 请求类型位掩码 不等于 USB标准请求类型
									switch( SetupReq ) 																				//请求码，无论是什么都不执行
									{
										case 0x01:																							//请求是获取报告（GetReport），不执行
												 break;
										case 0x02:																							//请求是获取标识符（GetIdle），不执行
												 break;	
										case 0x03:																							//请求是获取协议（GetProtocol），不执行
												 break;				
										case 0x09:																							//请求是设置报告（SetReport），不执行										
												 break;
										case 0x0A:																							//请求是设置标识符（SetIdle），不执行
												 break;	
										case 0x0B:																							//请求是设置协议（SetProtocol），不执行
												 break;
										default:																								
												 len = 0xFF;  								 					            //不支持的请求码类型，或出错			
												 break;
								  }
                }
                else																												//标准请求
                {
                    switch(SetupReq)                                        //请求码
                    {
											case USB_GET_DESCRIPTOR:															//请求码是获取描述符
                        switch(UsbSetupBuf->wValueH)												//发送什么描述符
                        {
													case 1:                                           //设备描述符
                            pDescr = DevDesc;                               //把设备描述符送到要发送的缓冲区
													len = sizeof(DevDesc);														//待发送数据长度就是设备描述符长度
                          break;
													case 2:                                           //配置描述符
                            pDescr = CfgDesc;                               //把设备描述符送到要发送的缓冲区
                            len = sizeof(CfgDesc);													//待发送数据长度就是配置描述符长度
                          break;
													case 0x22:                                        //报表描述符
                            if(UsbSetupBuf->wIndexL == 0)                   //接口0报表描述符
                            {
                                pDescr = KeyRepDesc;                        //数据准备上传
                                len = sizeof(KeyRepDesc);								
                            }
														if(UsbSetupBuf->wIndexL == 1)                   //接口1报表描述符
                            {
                                pDescr = KeyMULRepDesc;                     //数据准备上传
                                len = sizeof(KeyMULRepDesc);
                                Ready = 1;                                  //如果有更多接口，该标准位应该在最后一个接口配置完成后使能
                                //IE_UART1 = 1;															//开启串口中断															
                            }
                            else
                            {
                                len = 0xff;                                 //本程序只有2个接口，所以接口3及以上视为接口不支持，命令错误
                            }
                          break;
													default:
                            len = 0xff;                                     //不支持的描述符类型或者出错
                          break;
                        }
                        if ( SetupLen > len )																//如果长度过长溢出
                        {
                            SetupLen = len;    															//长度重新赋值为被允许的最大长度，限制总长度
                        }
                        len = SetupLen >= 8 ? 8 : SetupLen;                 //本次传输长度（在设置的长度与8之间取一个较小的数值）
                        memcpy(Ep0Buffer,pDescr,len);                       //加载上传数据（将端点0数据缓存区长度为len的数据加载到USB配置标志pDescr）
                        SetupLen -= len;
                        pDescr += len;
                      break;
												
											case USB_SET_ADDRESS:																	//请求码是设置地址
                        SetupLen = UsbSetupBuf->wValueL;                    //暂存USB设备地址
                      break;
											
											case USB_GET_CONFIGURATION:														//请求码是获取配置
                        Ep0Buffer[0] = UsbConfig;														//将USB配置加载到端点0数据缓存区
                        if ( SetupLen >= 1 )
                        {
                            len = 1;
                        }
                      break;
												
											case USB_SET_CONFIGURATION:														//请求码是设置配置
                        UsbConfig = UsbSetupBuf->wValueL;
                      break;
											
											case USB_GET_INTERFACE:																//请求码是获取接口（不响应）
                      break;
											
											case USB_CLEAR_FEATURE:                               //请求码是清除特性
                        if ( ( UsbSetupBuf->bRequestType & USB_REQ_RECIP_MASK ) == USB_REQ_RECIP_ENDP )// 如果USB请求类型为端点类型
                        {
                            switch( UsbSetupBuf->wIndexL )
                            {
															case 0x82:																		//端点类型为（ 0x82 = USB_ENDP_DIR_MASK | USB_ENDP_TYPE_BULK ）
                                UEP2_CTRL = UEP2_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;	//端点2控制寄存器
                              break;
															case 0x81:																		//端点类型为（ 0x81 = USB_ENDP_DIR_MASK | USB_ENDP_TYPE_ISOCH ）
                                UEP1_CTRL = UEP1_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;	//端点1控制寄存器
                              break;
															case 0x01:																		//端点类型为（ 0x81 = USB_ENDP_TYPE_CTRL | USB_ENDP_TYPE_ISOCH ）
                                UEP1_CTRL = UEP1_CTRL & ~ ( bUEP_R_TOG | MASK_UEP_R_RES ) | UEP_R_RES_ACK;	//端点1控制寄存器
															break;
															default:
                                len = 0xFF;                                 //不支持其他的端点类型，或出错
                              break;
                            }
                        }
                        else
                        {
                            len = 0xFF;                                     //请求码类型不是端点类型，不支持
                        }
                      break;
												
											case USB_SET_FEATURE:                                 //请求码是设置特性
                        if( ( UsbSetupBuf->bRequestType & 0x1F ) == 0x00 )  //如果是设置设备 
                        {
                            if( ( ( ( UINT16 )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x01 ) //如果{wValueH，wValueL}==0x01
                            {
                                if( CfgDesc[ 7 ] & 0x20 )										//如果CfgDesc[ 7 ] == 0x20
                                {
                                    /* 设置唤醒使能标志 */
                                }
                                else
                                {
                                    len = 0xFF;                             //CfgDesc[ 7 ] ！= 0x20，操作失败
                                }
                            }
                            else
                            {
                                len = 0xFF;                                 //如果{wValueH，wValueL}！=0x01，设置设备失败
                            }
                        }
                        else if( ( UsbSetupBuf->bRequestType & 0x1F ) == 0x02 )  //如果是设置端点 
                        {
                            if( ( ( ( UINT16 )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x00 ) //如果{wValueH，wValueL}==0x00
                            {
                                switch( ( ( UINT16 )UsbSetupBuf->wIndexH << 8 ) | UsbSetupBuf->wIndexL )	//判断{wValueH，wValueL}的值
                                {
																	case 0x82:
                                    UEP2_CTRL = UEP2_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* 设置端点2 IN STALL */
                                  break;
																	case 0x02:
                                    UEP2_CTRL = UEP2_CTRL & (~bUEP_R_TOG) | UEP_R_RES_STALL;/* 设置端点2 OUT Stall */
                                  break;
																	case 0x81:
                                    UEP1_CTRL = UEP1_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* 设置端点1 IN STALL */
                                  break;
																	default:
                                    len = 0xFF;                             //操作失败
                                  break;
                                }
                            }
                            else
                            {
                                len = 0xFF;                                 //如果{wValueH，wValueL}！=0x00，设置端点失败
                            }
                        }
                        else
                        {
                            len = 0xFF;                                     //不支持除设置设备与端点之外的其他设置，设置失败
                        }
                      break;
											
											case USB_GET_STATUS:																	//请求码为获取USB状态
                        Ep0Buffer[0] = 0x00;																//端点0数据缓存区第0位清零
                        Ep0Buffer[1] = 0x00;																//端点0数据缓存区第0位清零
                        if ( SetupLen >= 2 )																//长度len = min（SetupLen，2）取一个比较小的值
                        {
                            len = 2;
                        }
                        else
                        {
                            len = SetupLen;
                        }
                      break;
											
											default:
                        len = 0xff;                                         //不支持其他请求码，操作失败
                        break;
                    }
                }
            }
            else
            {
                len = 0xff;                                                 //长度和USB设置请求数据包（结构体）长度不同，则包长度错误
            }
            if(len == 0xff)																									//如果命令不支持或包长度错误，导致len == 0xff
            {
                SetupReq = 0xFF;																						//设置请求==0xFF
                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;//STALL设备挂起，暂停传输
            }
            else if(len <= 8)                                               //上传数据或者状态阶段返回0长度包
            {
                UEP0_T_LEN = len;
                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA1，返回应答ACK
            }
            else
            {
                UEP0_T_LEN = 0;  																						//虽然尚未到状态阶段，但是提前预置上传0长度数据包以防主机提前进入状态阶段
                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA1,返回应答ACK
            }
          break;
						
					default:
          break;
        }
        UIF_TRANSFER = 0;                                                 //写0清空中断
    }
    if(UIF_BUS_RST)                                                       	//如果USB总线复位触发中断  USB设备模式下总线复位事件中断标志，直接位地址清除或写入1来清除
    {
        UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;													//端点0控制寄存器
        UEP1_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK;													//端点1控制寄存器
        UEP2_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK | UEP_T_RES_NAK;					//端点2控制寄存器
        USB_DEV_AD = 0x00;																									//USB设备地址，低7位为USB设备地址  
        UIF_SUSPEND = 0;																										//USB挂起或恢复事件中断标志，直接位地址清除或写入1来清除
        UIF_TRANSFER = 0;																										//USB传输完成中断标志，直接位地址清除或写入1来清除 
        UIF_BUS_RST = 0;                                                 		//清中断标志++++++++++++++++++++++++++++这里有疑问，不因该是写1清零吗？
    }
    if (UIF_SUSPEND)                                                     		//如果USB总线挂起触发中断  USB挂起或恢复事件中断标志，直接位地址清除或写入1来清除  
    {
        UIF_SUSPEND = 0;																										//清除中断的方式有问题吧+++++++++++++++++++++++++++++++++++++++++++++++
    }
    else 																																		//意外的中断,不可能发生的情况
		{                                                               		
        USB_INT_FG = 0xFF;                                               		//清中断标志+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    }
}
/**键盘HID值上传函数**/
void HIDValueHandle1()
{
  TR0 = 0; //发送前关定时器中断
	FLAG = 0; //清空USB中断传输完成标志，准备发送按键按下数据
	Enp1IntIn(); //USB设备模式端点1的中断上传
	while(FLAG == 0); //等待USB中断数据传输完成
	FLAG = 0; //清空USB中断传输完成标志，准备发送按键抬起数据	
	memset(&HIDKey[0],0,8);	//把HIDkey置0，发送0表示按键抬起
	Enp1IntIn(); //USB设备模式端点1的中断上传		
	while(FLAG == 0); //等待USB中断数据传输完成
	TR0 = 1; //发送完打开定时器中断		
}
/**多媒体按键HID值上传函数**/
void HIDValueHandle2()
{
  TR0 = 0; 							//发送前关定时器中断
	
	FLAG = 0; 						//清空USB中断传输完成标志，准备发送按键按下数据
	Enp2IntIn(); 					//USB设备模式端点2的中断上传
	while(FLAG == 0); 		//等待USB中断数据传输完成
	
	FLAG = 0; 						//清空USB中断传输完成标志，准备发送按键抬起数据	
	memset(&HIDKeyMUL[0],0,4); //把HIDKeyMUL置0，发送0表示按键抬起
	Enp2IntIn(); 					//USB设备模式端点2的中断上传
	while(FLAG == 0); 		//等待USB中断数据传输完成
	
	TR0 = 1; 							//发送完打开定时器中断
}


/**按键行为函数**/
/*找到按键的HID值自由发挥部分*/
/*普通按键
  例如ctrl + c :
  HIDKey[0] = 0x01;
  HIDKey[2] = 0x06;
  if(Ready) //枚举成功
	{
	    HIDValueHandle1();
	}
*/
/* 按键动作函数 */
void KeyAction(unsigned char keyCode)
{   
	if(keyCode == 0x31)//按键1
	{
	    HIDKeyMUL[0] = 0x10;  //打开媒体播放器
		if(Ready) //枚举成功
        {
            HIDValueHandle2(); //多媒体按键HID值上传
        }
	}
	if(keyCode == 0x32)//按键2
	{
		HIDKeyMUL[1] = 0x01;  //下一曲
		if(Ready) //枚举成功
        {
            HIDValueHandle2(); //多媒体按键HID值上传
        }
	}
	if(keyCode == 0x33)	//按键3	
	{
		HIDKeyMUL[0] = 0x80;  //上一曲
		if(Ready) //枚举成功
        {
            HIDValueHandle2(); //多媒体按键HID值上传
        }
	}
	if(keyCode == 0x34)	//按键4	
	{
		HIDKeyMUL[0] = 0x40; //播放/暂停

		if(Ready) //枚举成功
        {
            HIDValueHandle2(); //多媒体按键HID值上传
        }
	}
	if(keyCode == 0x35)	//按键5	
	{
		HIDKeyMUL[0] = 0x02; //音量+

		if(Ready) //枚举成功
        {
            HIDValueHandle2(); //多媒体按键HID值上传
        }
	}
	if(keyCode == 0x36)	//按键6	
	{
		HIDKeyMUL[0] = 0x01; //音量-

		if(Ready) //枚举成功
        {
            HIDValueHandle2(); //多媒体按键HID值上传
        }
	}
}
/**按键驱动**/
void KeyDrive()
{
	unsigned char j;

	for(j=0;j<4;j++)  //1-4是触发按键
	{
		if(KeyState[j] != BackState[j]) // 按键发生变化，当前按键状态不是之前的状态
		{
	    	if(BackState[j] != 0)				// 之前的状态不是0，按键状态从1变成了0，有按键按下
			{
				KeyAction(key_code_map[j]);	// 通过数组查找，找到发送的数据（按下的是什么按键）
			}
			BackState[j] = KeyState[j]; 	// 更新按键当前状态
		}
		if(KeyDownTime[j] > 0)					// 检测到持续按下按键
		{
			if(KeyDownTime[j] >= TimeThr[j])	// 持续按下按键的时间，大于临界值
			{
				KeyAction(key_code_map[j]);	// 做出一次新的按键反应
				TimeThr[j] += 100;					// 对应按键临界时间值+100ms，准备持续刷新
				// 例如长按a，第一个a和第二个a时间间隔长（此处间隔1000ms），后边连续出现a（此处间隔100ms）
			}
		}
		else // 长安结束后，将长按时间临界值复位
		{
			TimeThr[j] = 1000;
		}
	}
}
/**EC11驱动**/   // 两线四相霍尔编码器
/*顺时针旋转按键5，逆时针旋转按键6*/
// A线 1001100110
// B线 0011001100
// 此程序有瑕疵，只检测A线信号的跳变（若B线跳变，A线不变无法检测）
void EC11Drive()      
{
	EC11_A_State = KeyState[4];					// 读取编码器A线状态
	if(EC11_A_State != EC11_A_PState)		// A线状态发生改变
	{
	   if(EC11_A_State == 1) 						// A线新状态为1（从0到1发生跳变）
	   {
	      if(KeyState[5] == 0) 					// 编码器B线状态为0
		  {
		  	  KeyAction(key_code_map[4]);	// A线0到1，B线为0，向左旋转编码器
		  }
		  else
		  {
			  KeyAction(key_code_map[5]);		// A线0到1，b线为1，向右旋转编码器
		  }
	   }
	   else															// A线新状态为0（从1到0发生跳变）
	   {
		  if(KeyState[5]  == 1)						// B线状态为1
		  {
		  	  KeyAction(key_code_map[4]);	// A线1到0，b线为1，向左旋转编码器
		  }
		  else
		  {
			  KeyAction(key_code_map[5]);		// A线1到0，b线为0，向右旋转编码器
		  }
	   }
	   EC11_A_PState = EC11_A_State;		// 编码器A线上一状态更新
	}
}

/**定时器0初始化配置函数**/
void ConfigT0(UINT8 ms)
{
    unsigned long tmp = 0;

	tmp = 12000000/12;
	tmp = (tmp * ms)/1000;
	tmp = 65536 - tmp;
	tmp = tmp + 1;
  T0RH = (UINT8)(tmp >> 8);
	T0RL = (UINT8)tmp;

	TMOD = ( TMOD & ~( bT0_GATE | bT0_CT | bT0_M1 ) ) | bT0_M0;//* 模式1，16 位定时/计数器
	TH0 = T0RH;
	TL0 = T0RL;
	TF0 = 0;
	ET0 = 1;
	TR0 = 1;
}
/*****************主函数**********************/
main()
{
  CfgFsys(); 			//CH552时钟选择12M配置
  mDelaymS(5); 		//修改主频等待内部晶振稳定,必加	
	ConfigT0(8); 		//配置8ms T0中断
	USBDeviceInit();//USB设备模式初始化
  EA = 1; 				//允许单片机中断
  UEP1_T_LEN = 0; //清空 端点1预计发送长度
  UEP2_T_LEN = 0;	//清空 端点2预计发送长度
  FLAG = 0; 			//清空 USB中断传输完成标志
  Ready = 0;
	LED_VALID = 1;  //给一个LED默认值

	key1 = 1;
	key2 = 1;
	key3 = 1;
	key4 = 1;
	EC11_A = 1;
	EC11_B = 1;

	while(1)
	{
	    KeyDrive();	//按键驱动
   		EC11Drive();//EC11驱动
	}
}

/**按键扫描函数**/
// 更新按键按下的状态与对应按键按下的持续时间
void KeyScan() 
{
	UINT8 i;
	static UINT8 keybuffer[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

	keybuffer[0] = (keybuffer[0] <<1) | key1;//按键扫描
	keybuffer[1] = (keybuffer[1] <<1) | key2;
	keybuffer[2] = (keybuffer[2] <<1) | key3;
	keybuffer[3] = (keybuffer[3] <<1) | key4;
	keybuffer[4] = (keybuffer[4] <<1) | EC11_A;//EC11扫描
	keybuffer[5] = (keybuffer[5] <<1) | EC11_B;
	
	for(i=0;i<6;i++)
	{
		if((keybuffer[i] & 0x0F) == 0x00) // 第i个按键按下，等价于keyi==0
		{
			KeyState[i] = 0;	// 改写按键状态
			if(i>=3)					// 如果是旋转编码器则跳出，不计算按键长按时间
			    break;
			KeyDownTime[i] += 4;  //按下的是触点按键，则累计按键时长
		}
		else if((keybuffer[i] & 0x0F) == 0x0F) //第i个按键松开，等价于keyi==1
		{
			KeyState[i] = 1;	// 改写按键状态
			if(i>=3)					// 如果是旋转编码器则跳出，不计算按键长按时间
			    break;
			KeyDownTime[i] = 0;   //触点按键累计按键时长归零
		}
	} 
}



/**定时器0中断服务程序**/
void InterruptTimer0() interrupt INT_NO_TMR0 using 1 
{
	TH0 = T0RH;  // 定时器中断触发后，定时器重载
	TL0 = T0RL;
	KeyScan(); //按键扫描
}

