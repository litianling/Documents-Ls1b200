/*
 * gpio-i2c.c
 *
 * created: 2021-11-19
 *  author:
 */

#define GPIO_I2C_TEST

#ifdef GPIO_I2C_TEST

#include "bsp.h"
#include "ls1b_gpio.h"

//-------------------------------------------------------------------------------------------------

#define SDA_LINE        33
#define SCL_LINE        32

#define SET_SDA_OUT     gpio_enable(SDA_LINE, DIR_OUT)
#define SET_SDA_IN      gpio_enable(SDA_LINE, DIR_IN)
#define SET_SCL_OUT     gpio_enable(SCL_LINE, DIR_OUT)
#define SET_SCL_IN      gpio_enable(SCL_LINE, DIR_IN)

#define SDA_READ        gpio_read(SDA_LINE)             // 读取SDA电平状态

#define SDA_H           gpio_write(SDA_LINE, 1)         // SDA线置高电平
#define SDA_L           gpio_write(SDA_LINE, 0)         // SDA线置低电平
#define SCL_H           gpio_write(SCL_LINE, 1)         // SCL线置高电平
#define SCL_L           gpio_write(SCL_LINE, 0)         // SCL线置低电平

#define US_PER_BIT      10

#define DELAY           delay_us(US_PER_BIT)
#define DELAY2          delay_us((US_PER_BIT + 1) >> 1)
#define DELAY1          delay_us(1)

#define ACK_WAIT_US     100   /* 100us 内没反应, 出错 */ // 写数据时等待 slave ACK 延时

//-------------------------------------------------------------------------------------------------

/**
 * 产生起始条件: SCL为高电平的时候，SDA由高电平向低电平跳变
 
        ----
 SDA out    \
             -----
        -------
 SCL           \
                ----
 */
void i2c_start(void)
{
    SET_SDA_OUT;
    SET_SCL_OUT;

    SDA_H;
    SCL_H;
    DELAY;
    SDA_L;
    DELAY;
    SCL_L;
    DELAY;
}

/**
 * 产生停止条件: SCL为高电平的时候，SDA由低电平向高电平跳变
               ----
 SDA out      /
        ------
            -------
 SCL       /
        ---
 */
void i2c_stop(void)
{
    SDA_L;
    DELAY;
    SCL_H;
    DELAY;
    SDA_H;
    DELAY;

    SET_SDA_IN;
    SET_SCL_IN;
}

/**
 * 产生应答信号ACK
 
        ----          ----
 SDA out    \        /
             --------
               ---
 SCL          /   \
        ------     ------
 */
static inline void i2c_send_ack(void)
{
    SDA_L;
    DELAY2;
    SCL_H;
    DELAY2;
    SCL_L;
    DELAY2;
}

/**
 * 产生非应答信号NACK

        -----------------
 SDA out

               ---
 SCL          /   \
        ------     ------
 */
static inline void i2c_send_nack(void)
{
    SDA_H;
    DELAY2;
    SCL_H;
    DELAY2;
    SCL_L;
    DELAY2;
}

/**
 * CPU产生一个时钟，读取应答信号ACK
 *
 * 返回     0表示应答信号，返回1表示非应答信号
 *
        -----------------   nack=1
 SDA in    \       /
            -------         ack=0
              ---
 SCL         /   \
        -----     -----
 */
static int i2c_wait_ack(int tmo_us)
{
    int ack;

    SET_SDA_IN;                     // set SDA in
    SCL_H;	                        // send a SCL pulse, return ACK signal
    DELAY2;

    ack = SDA_READ;
    while (ack && (tmo_us-- > 0))
    {
        DELAY1;
        ack = SDA_READ;             // 0: ACK, 1: NACK
    }

    SCL_L;
    SET_SDA_OUT;                    // set SDA out
    DELAY2;

    if (ack != 0)                   // no ACK
    {
        printk("IIC write data but has no ACK.\r\n");
    }
    
    return ack;
}

/**
 * 接收一个字节的数据
 *
 * 返回     接收到的数据
 *
        -----------------   bit=1
 SDA in    \       /
            -------         bit=0
               ---
 SCL          /   \
        ------     ------
 */
static unsigned char i2c_read_1byte(void)
{
    int i;
    unsigned char rdbyte = 0;

    SET_SDA_IN;                   // set SDA in

    for (i=0; i<8; i++)
    {
        rdbyte <<= 1;
        SCL_H;
        DELAY2;

        if (SDA_READ)
        {
            rdbyte++;
        }

        SCL_L;
        DELAY2;
    }

    SET_SDA_OUT;                  // set SDA out

    return rdbyte;
}

/**
 * 发送一个字节的数据
 *
 * 参数     byte：等待发送的字节
 *
        -----------------   bit=1
 SDA out   \       /
            -------         bit=0
               ---
 SCL          /   \
        ------     ------
 */
static void i2c_write_1byte(unsigned char wrbyte)
{
    int i;

    for (i=0; i<8; i++)
    {
        if (wrbyte & 0x80)          // send byte with MSB bit
            SDA_H;
        else
            SDA_L;
        DELAY2;
        
        SCL_H;
        DELAY2;
        SCL_L;

        wrbyte <<= 1;	            // shift left 1 bit to transmit next bit
    }

    DELAY1;                         // make up for the last SCL_L
}

//-------------------------------------------------------------------------------------------------

/**
 * 发送 IIC 地址
 *
 * 参数     address:    7位I2C地址码
 *          read_op:    1=read; 0=write
 *
 * 返回     0: 成功
 */
int i2c_send_address(unsigned char address, int read_op)
{
    int ack;
    
    if (read_op)
        i2c_write_1byte((address << 1) | 1);    // LSB=1: read operation
    else
        i2c_write_1byte((address << 1) | 0);    // LSB=0: write operation

    ack = i2c_wait_ack(ACK_WAIT_US);            // should be ACK

    return ack;
}

/**
 * IIC 读数据
 */
int i2c_read_bytes(unsigned char *buf, int count)
{
    unsigned char ch;
    int rd_bytes = 0;

    while (count-- > 0)
    {
        ch = i2c_read_1byte();          // read 1 byte

        if (count == 0)
            i2c_send_nack();            // if the last byte, send NACK
        else
            i2c_send_ack();             // else send ACK
            
        *buf++ = ch;
        rd_bytes++;
    }

    return rd_bytes;
}

/**
 * IIC 写数据
 */
int i2c_write_bytes(unsigned char *buf, int count)
{
    int ack, wr_bytes = 0;

    while (count-- > 0)
    {
        i2c_write_1byte(*buf++);            // write 1 byte

        ack = i2c_wait_ack(ACK_WAIT_US);    // slave ACK

        if (ack != 0)                       // no ACK
        {
            return wr_bytes;
        }

        wr_bytes++;
    }

    return wr_bytes;
}

//-------------------------------------------------------------------------------------------------
// ADS1015 测试
//-------------------------------------------------------------------------------------------------

#define ADS1015_ADDRESS         0x48

#ifdef ADS1015_ADDRESS

#include "i2c/ads1015.h"

void read_ads1015_config(void)
{
    int rt;
    unsigned char buf[8];
    unsigned int reg_val;

    memset(buf, 0, 8);

    i2c_start();
    rt = i2c_send_address(ADS1015_ADDRESS, 0);      // 0 for write
    if (rt == 0)
    {
        buf[0] = ADS1015_REG_POINTER_CONFIG;
        i2c_write_bytes(buf, 1);
    }
    i2c_stop();
        
    i2c_start();
    rt = i2c_send_address(ADS1015_ADDRESS, 1);  // 1 for read
    if (rt == 0)
    {
        i2c_read_bytes(buf, 2);
            
        reg_val = ((unsigned int)buf[0] << 8) + buf[1];
            
        // printk("read ads1015[%i]=0x%04X\r\n", ADS1015_REG_POINTER_CONFIG, reg_val);
        
        print_ads1015_config_reg((unsigned short)reg_val);
    }

    i2c_stop();
}

#else

void read_ads1015_config(void)
{
    //
}

#endif

//-------------------------------------------------------------------------------------------------
// gp7101 测试
//-------------------------------------------------------------------------------------------------

//#define GP7101_ADDRESS          0x58

#ifdef GP7101_ADDRESS

int set_lcd_brightness(void)
{
    int i, rt;
    unsigned char brightness;

    for (i=0; i<100; i++)
    {
        brightness = (unsigned char)(i * 255 / 100);
        
        i2c_start();

        rt = i2c_send_address(GP7101_ADDRESS, 0);      // 0 for write
        if (rt == 0)
        {
            i2c_write_bytes(&brightness, 1);
        }

        i2c_stop();
        
        delay_ms(100);
    }
    
    // 
}

#else

int set_lcd_brightness(void)
{
    //
}

#endif

//-------------------------------------------------------------------------------------------------
// mcp4725 测试
//-------------------------------------------------------------------------------------------------

#define MCP4725_ADDRESS			0x62 // 0x60 //

#ifdef MCP4725_ADDRESS

#include "i2c/mcp4725.h"

static void print_mcp4725_config_reg(unsigned char *buf, int nlen)
{
	if (nlen != 5)
		return;

	printk("read mcp4725 result: \r\n");
	printk("  EEPROM Write Status = %s\r\n", buf[0] & 0x80 ? "Completed" : "Incomplete");
	printk("  Power On Reset      = %s\r\n", buf[0] & 0x40 ? "ON" : "OFF");
	printk("  Power Down Mode     = ");
	switch ((buf[0] & 0x06) >> 1)
	{
		case 0:	printk("Normal Mode\r\n"); break;
		case 1:	printk("1 kΩ resistor to ground\r\n"); break;
		case 2:	printk("100 kΩ resistor to ground\r\n"); break;
		case 3:	printk("500 kΩ resistor to ground\r\n"); break;
	}

	printk("  DAC register Data   = 0x%04X\r\n",
			((unsigned int)buf[1] << 4) + ((unsigned int)buf[2] >> 4));

	printk("  EEPROM Data: \r\n");
	printk("    Power Down Mode   = ");
	switch ((buf[3] & 0x60) >> 1)
	{
		case 0:	printk("Normal Mode\r\n"); break;
		case 1:	printk("1 kΩ resistor to ground\r\n"); break;
		case 2:	printk("100 kΩ resistor to ground\r\n"); break;
		case 3:	printk("500 kΩ resistor to ground\r\n"); break;
	}

	printk("    Saved DAC Data    = 0x%04X\r\n",
			(((unsigned int)buf[3] << 8) + ((unsigned int)buf[4])) & 0x0FFF);
}

int mcp4725_read_config(void)
{
    int rt;
    unsigned char buf[8];

    memset(buf, 0, 8);

    i2c_start();
    
    rt = i2c_send_address(MCP4725_ADDRESS, 1);      // 1 for read
    if (rt == 0)
    {
        rt = i2c_read_bytes(buf, 5);
        if (rt == 5)
        {
            print_mcp4725_config_reg(buf, 5);
        }
    }

    i2c_stop();

    return 0;
}

int mcp4725_write(unsigned int val)
{
    int rt;
    unsigned char buf[2];

    buf[0] = (val >> 8) & 0x0F;
    buf[1] = val & 0xFF;

    i2c_start();
    
    rt = i2c_send_address(MCP4725_ADDRESS, 0);      // 0 for write
    if (rt == 0)
    {
        i2c_write_bytes(buf, 2);
    }

    i2c_stop();

    return 0;
}

#else

int mcp4725_read_config(void)
{
    return -1;
}

int mcp4725_write(unsigned int val)
{
    return -1;
}

#endif


#endif // #ifdef GPIO_I2C_TEST

