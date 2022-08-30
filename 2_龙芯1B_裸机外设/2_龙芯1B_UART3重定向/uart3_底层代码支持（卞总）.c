/*
 * uart3.c
 *
 * created: 2021-10-08
 *  author: 
 */

/*
 * 本代码实现一个 UART3 的驱动程序
 */

#include "bsp.h"
#include "mips.h"
#include "cpu.h"

#include "ls1b.h"
#include "ls1b_irq.h"

/*
 * UART3 寄存器定义
 */

#define UART_FIFO_SIZE  16

typedef struct
{
    union
	{
    	volatile unsigned char dat;     // 0x00 数据寄存器          // when DLAB=0, read as receive buffer
                                                                    // when DLAB=0, write as send buffer
    	volatile unsigned char dll;     // 0x00 分频值低字节寄存器  // when DLAB=1, rw as dll
	} R0;
    union
	{
    	volatile unsigned char ier;     // 0x01 中断使能寄存器      // when DLAB=0, rw as ier
    	volatile unsigned char dlh;     // 0x01 分频值高字节寄存器  // when DLAB=1, rw as dlh
	} R1;
    union
	{
		volatile unsigned char isr;     // 0x02 中断状态寄存器      // when DLAB=X, read as isr
		volatile unsigned char fcr;     // 0x02 FIFO控制寄存器      // when DLAB=X, write as fcr
	} R2;
    volatile unsigned char lcr;         // 0x03 线路控制寄存器      // XXX bit(7) is DLAB
    volatile unsigned char mcr;         // 0x04 MODEM控制寄存器
    volatile unsigned char lsr;         // 0x05 线路状态寄存器
    volatile unsigned char msr;         // 0x06 MODEM状态寄存器
} HW_UART_t;

static HW_UART_t *pUART3 = NULL;        // UART3 设备指针

//-------------------------------------------------------------------------------------------------

#define UART3_USE_INTERRUPT     0       // 是否使用中断

#if UART3_USE_INTERRUPT

/*
 * 数据接收缓冲区
 */

//-----------------------------------------------------------------------------
// buffer: cycle mode, drop the most oldest data when add
//-----------------------------------------------------------------------------

/*
 * 保存 UART_BUF_SIZE 个字符.
 *
 * 字节: 0  1  2  3  4  5        ...
 *       __ __ xx xx xx xx __ __ ...
 *             ^           ^
 *             pHead       |
 *                         pTail
 *
 * if full or empty: pHead==pTail;
 */

#define UART_BUF_SIZE   256         /* 缓冲区大小 */

typedef struct
{
    char  Buf[UART_BUF_SIZE];
    int   Count;
    char *pHead;
    char *pTail;
} UART_buf_t;

static UART_buf_t s_RxBuf;          /* 接收缓冲区 */
static UART_buf_t s_TxBuf;          /* 发送缓冲区 */

/*
 * If oveflow, overwrite always
 */
static int enqueue_to_buffer(UART_buf_t *data, char *buf, int len)
{
    int i;

    for (i=0; i<len; i++)
    {
        *data->pTail = buf[i];
        data->Count++;
        data->pTail++;
        if (data->pTail >= data->Buf + UART_BUF_SIZE)
            data->pTail = data->Buf;
    }

    /*
     * if overflow, override the lastest data
     */
    if (data->Count > UART_BUF_SIZE)    // overflow
    {
        data->Count = UART_BUF_SIZE;
        data->pHead = data->pTail;
    }

    return len;
}

static int dequeue_from_buffer(UART_buf_t *data, char *buf, int len)
{
    int i, count;

    count = len < data->Count ? len : data->Count;

    for (i=0; i<count; i++)
    {
        buf[i] = *data->pHead;
        data->Count--;
        data->pHead++;
        if (data->pHead >= data->Buf + UART_BUF_SIZE)
            data->pHead = data->Buf;
    }

    return count;
}

/*
 * UART3 中断句柄
 */
static void uart3_isr(int vector, void *arg)
{
    LS1x_INTC_IEN(LS1x_INTC0_BASE) &= ~INTC0_UART3_BIT;
    
    do
    {
        int i, count = 0;
        char buf[UART_FIFO_SIZE+1];
    
        if (pUART3->R2.isr & 0x04)          /* 收到数据 receive ready */
        {
            for (i=0; i<UART_FIFO_SIZE; ++i)
            {
                if (pUART3->lsr & 0x01)     /* receiver ready */
                    buf[i] = (char)pUART3->R0.dat;
                else
                    break;
            }

            enqueue_to_buffer(&s_RxBuf, buf, i);
        }

        /* 有等待发送的数据
         */
        if ((s_TxBuf.Count > 0) && (pUART3->lsr & 0x20)) 	/* transmitter ready */
        {
            /* Dequeue transmitted characters from buffer
             */
            count = dequeue_from_buffer(&s_TxBuf, buf, UART_FIFO_SIZE);
            for (i=0; i<count; ++i)
                pUART3->R0.dat = buf[i];
        }

        if (count > 0)
            pUART3->R1.ier = 0x03;          /* interrupt on rx & tx */
        else
            pUART3->R1.ier = 0x01;		    /* interrupt on rx */

    } while (!(pUART3->R2.isr & 0x01));     /* nothing */
    
    LS1x_INTC_IEN(LS1x_INTC0_BASE) |= INTC0_UART3_BIT;
}

#endif

/*
 * UART3 初始化
 *
 * 参数:    baudrate: 通信波特率
 *          databits: 数据位数
 *          eccmode:  校验模式, 'O': 奇校验; 'E': 偶校验; 'N': 无
 *          stopbits: 结束位数
 *
 * 返回:    0
 */
int uart3_initialize(int baudrate, int databits, char eccmode, int stopbits)
{
    unsigned int divisor, bus_freq;
    unsigned char lcr;
    
    if (baudrate < 2400)
        baudrate = 115200;
        
    pUART3 = (HW_UART_t *)LS1B_UART3_BASE;

    pUART3->lcr = 0;
    pUART3->R1.ier = 0;

    bus_freq = LS1x_BUS_FREQUENCY(CPU_XTAL_FREQUENCY);  // 总线频率
    divisor =  bus_freq / 16 / baudrate;                /* 计算分频系数, 总线频率/baudrate/16 */
    pUART3->lcr = 0x80;                                 // 设置 DLAB
    pUART3->R0.dll = divisor & 0xFF;                    // 分频值低字节
    pUART3->R1.dlh = (divisor >> 8) & 0xFF;             // 分频值高字节
    pUART3->R2.fcr = 0x07;                              /* reset fifo */
    
    switch (databits)
    {
        case 5:  lcr = 0x00; break;
        case 6:  lcr = 0x01; break;
        case 7:  lcr = 0x02; break;
        case 8:
        default: lcr = 0x03; break;
    }

    switch (eccmode)
    {
        case 'O': lcr |= 0x08; break;
        case 'E': lcr |= 0x18; break;
    }
    
    if (stopbits == 2)
        lcr |= 0x04;

    pUART3->lcr = lcr;

#if UART3_USE_INTERRUPT

    /* 初始化数据缓冲区
     */
    s_RxBuf.Count = 0;
    s_RxBuf.pHead = s_RxBuf.pTail = s_RxBuf.Buf;
    s_TxBuf.Count = 0;
    s_TxBuf.pHead = s_TxBuf.pTail = s_TxBuf.Buf;
    
    /* 安装中断 */
    ls1x_install_irq_handler(LS1B_UART3_IRQ, uart3_isr, NULL);
    
    /* 开中断 */
    LS1x_INTC_EDGE(LS1x_INTC0_BASE) &= ~INTC0_UART3_BIT;
    LS1x_INTC_POL( LS1x_INTC0_BASE) |=  INTC0_UART3_BIT;
    LS1x_INTC_CLR( LS1x_INTC0_BASE)  =  INTC0_UART3_BIT;
    LS1x_INTC_IEN( LS1x_INTC0_BASE) |=  INTC0_UART3_BIT;

    pUART3->R1.ier = 0x01;		    /* interrupt on rx */

#endif

    return 0;
}

/*
 * UART3 读数据
 *
 * 参数:    buf:    数据缓冲区
 *          size:   读字节数
 *
 * 返回:    本次读的字节数
 */
int uart3_read(unsigned char *buf, int size)
{
    int count = 0;
    
    if ((pUART3 == NULL) || (buf == NULL))
        return -1;

    if (size < 0)
        return 0;

#if UART3_USE_INTERRUPT

    mips_interrupt_disable();
    count = dequeue_from_buffer(&s_RxBuf, buf, size);
    mips_interrupt_enable();
    
    return count;
    
#else

    unsigned char *p = buf;

    while (count < size)
    {
        if (pUART3->lsr & 0x01)         /* receiver ready */
        {
            *p++ = pUART3->R0.dat;      /* receive character */
            count++;
        }
        else
            delay_us(100);
    }

    return size;

#endif
}

/*
 * UART3 写数据
 */
int uart3_write(unsigned char *buf, int size)
{
    if ((pUART3 == NULL) || (buf == NULL))
        return -1;

    if (size < 0)
        return 0;
        
#if UART3_USE_INTERRUPT

    int i, sent = 0;

    /* if idle, send immediately
     */
    if (pUART3->lsr & 0x20)             /* transmitter ready */
    {
        sent = size <= UART_FIFO_SIZE ? size : UART_FIFO_SIZE;
        
        for (i=0; i<sent; ++i)          /* write data to transmit buffer */
            pUART3->R0.dat = buf[i];

        if (sent > 0)
            pUART3->R1.ier = 0x03;      /* interrupt on rx & tx */
        else
            pUART3->R1.ier = 0x01;		/* interrupt on rx */
    }

    /* add remain data to transmit cached buffer
     */
    if (sent < size)
    {
        mips_interrupt_disable();
        sent += enqueue_to_buffer(&s_TxBuf, buf + sent, size - sent);
        mips_interrupt_enable();
    }

    return sent;

#else

    int count = 0;
    unsigned char *p = buf;

    while (count < size)
    {
        if (pUART3->lsr & 0x20)         /* transmitter ready */
        {
            pUART3->R0.dat = *p++;      /* Transmit character */
            count++;
        }
        else
            delay_us(100);
    }

    return size;

#endif
}

 
